#include "vime_buffer.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void vbClose(VBCursor c) {
    VBBlock l = c->block;
    VBBlock r = c->block->next;
    while (l) {
        VBBlock temp = l->previous;
        free(l);
        l = temp;
    }
    while (r) {
        VBBlock temp = r->next;
        free(r);
        r = temp;
    }
    free(c);
}

VBCursor vbOpen(char *filename) {
	FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    VBCursor c = oalloc(struct VBCursor);
    c->pos = 0;
    c->index = 0;
    c->block = vbBlock();
    c->line = 1;
    c->column = 1;

    // Read each character from the file in succession, inserting it into the
    // buffer.
    //
    // TODO: Implement block-level reading/insertion to improve performance on
    // large files.
    while (true) {
		char ch = fgetc(file);
		if (feof(file)) break;
        vbInsert(c, ch);
        vbForward(c);
    }
	fclose(file);
    return c;
}

/*
 * MARK: VBBlock methods.
 */

/*
 * CONSTRUCTOR: vbBlock()
 *
 * Creates and returns a new instance of VBBlock, or a single payload block
 * for the vime buffer. Fields will be initialized as follows:
 *     `next`     NULL
 *     `previous` NULL
 *     `size`     0
 *     `payload`  undefined
 *
 * USAGE    VBBlock b = VBBlock_new();
 *
 * RETURN   (VBBlock) The created buffer block.
 */
VBBlock vbBlock() {
    VBBlock b = oalloc(struct VBBlock);
    b->next = NULL;
    b->previous = NULL;
    b->size = 0;
    return b;
}

/* METHOD: vbSplit(VBBlock old)
 *
 * Splits an existing VBBlock instance into two, each containing half of the
 * original payload. The new block is automatically spliced into the containing
 * linked list, *after* the old block, using the old block's `next` and
 * `previous` fields.
 *
 * USAGE    VBBlock new = vbSplit(old);
 *
 * RETURN   (VBBlock) The new (second) block from the split.
 */
VBBlock vbSplit(VBBlock old) {

    // Use oalloc to create the new block in the heap. See util.h for
    // implementation.
    VBBlock new = oalloc(struct VBBlock);

    // Splice the new block into the doubly-linked list of blocks, *after* the
    // old block.
    new->next = old->next;
    new->previous = old;
    if (old->next) old->next->previous = new;
    old->next = new;

    // Give half of the characters to the new block.
    new->size = old->size / 2;
    old->size -= new->size;

    // Copy the characters over and return.
    memcpy(new->payload, old->payload + old->size, new->size);
    return new;
}

void vbDispose(VBCursor c) {
    free(c);
}

VBCursor vbDup(VBCursor c) {
    VBCursor cp = oalloc(struct VBCursor);
    *cp = *c;
    return cp;
}

uint vbDistance(VBCursor c1, VBCursor c2) {
    VBBlock b1 = c1->block;
    VBBlock b2 = c2->block;
    uint d = 0;
    while (b1 != b2) {
        d += b1->size;
        b1 = b1->next;
    }
    d += c2->index - c1->index;
    return d;
}

/*
 * MARK: Unique Cases
 */

bool vbEOF(VBCursor c) {
    return c->index >= c->block->size;
}

bool vbSOF(VBCursor c) {
    return c->pos == 0;
}

/*
 * MARK: Navigation
 */

static bool vbForwardRaw(VBCursor c) {
    if (vbEOF(c)) return false;
    c->index += 1;
    c->pos += 1;
    VBBlock b = c->block;
    if (c->index >= b->size && b->next) {
        c->block = b->next;
        c->index = 0;
    }
    return true;
}

static bool vbBackwardRaw(VBCursor c) {
    if (vbSOF(c)) return false;
    c->pos -= 1;
    VBBlock b = c->block;
    if (c->index == 0) {
        c->block = b->previous;
        c->index = c->block->size - 1;
    } else {
        c->index -= 1;
    }
    return true;
}

bool vbForward(VBCursor c) {
    // Update line/column tracking, accounting for newlines.
    if (vbGet(c) == '\n') {
        c->column = 1;
        c->line += 1;
    } else {
        c->column += 1;
    }
    return vbForwardRaw(c);
}

bool vbBackward(VBCursor c) {
    if (!vbBackwardRaw(c)) return false;

    // Update line/column tracking, accounting for newlines.
    if (vbGet(c) == '\n') {
        c->line -= 1;

        VBCursor cp = vbDup(c);
        while (vbBackwardRaw(cp)) {
            if (vbGet(cp) == '\n') {
                vbForwardRaw(cp);
                break;
            }
        }
        c->column = vbDistance(cp, c) + 1;
        vbDispose(cp);
    } else {
        c->column -= 1;
    }
    return true;
}

bool vbMove(VBCursor c, int delta) {
    if (delta < 0) {
        for (int i = delta; i < 0; i += 1) {
            if (!vbBackward(c)) return false;
        }
    } else {
        for (uint i = 0; i < delta; i += 1) {
            if (!vbForward(c)) return false;
        }
    }
    return true;
}

bool vbMoveLine(VBCursor c, int delta) {
    uint targetLine = c->line + delta;

    if (delta <= 0) {
        while (true) {
            if (!vbBackward(c)) return false;
            if (c->line == targetLine && c->column == 0) return true;
        }
    } else {
        while (true) {
            if (!vbForward(c)) return false;
            if (c->line == targetLine && c->column == 0) return true;
        }
    }
}

bool vbMoveCol(VBCursor c, int delta) {
    if (delta < 0) {
        for (int i = delta; i < 0; i += 1) {
            if (c->column == 0) return false;
            vbBackwardRaw(c);
            c->column -= 1;
        }
    } else {
        for (uint i = 0; i < delta; i += 1) {
            if (vbGet(c) == '\n') return false;
            if (!vbForwardRaw(c)) return false;
            c->column += 1;
        }
    }
    return true;
}

bool vbTo(VBCursor c, uint pos) {
    return vbMove(c, pos - c->pos);
}

bool vbToLine(VBCursor c, uint line) {
    return vbMoveLine(c, line - c->line);
}

bool vbToCol(VBCursor c, uint column) {
    return vbMoveCol(c, column - c->column);
}


/*
 * MARK: Text Manipulation
 */

char vbGet(VBCursor c) {
    if (vbEOF(c)) return -1;
    return c->block->payload[c->index];
}

void vbPut(VBCursor c, char ch) {
    if (vbEOF(c)) return;
    c->block->payload[c->index] = ch;
}

char vbDelete(VBCursor c) {
    if (vbEOF(c)) return -1;

    char ch = vbGet(c);

    VBBlock b = c->block;
    char *target = b->payload + c->index;
    memmove(target, target + 1, b->size - c->index - 1);
    b->size -= 1;

    // If the block has become empty, delete it and bridge the linked list.
    if (b->size == 0) {
        if (b->previous) b->previous->next = b->next;
        if (b->next) b->next->previous = b->previous;
        c->block = b->next;
        c->index = 0;
        free(b);
    }
    return ch;
}

void vbInsert(VBCursor c, char ch) {

    VBBlock b = c->block;
    char *target = b->payload + c->index;
    memmove(target + 1, target, b->size - c->index);
    b->size += 1;

    vbPut(c, ch);

    // There must always be space at the end of a block. If we just fillied up
    // this block, split it in half.
	if (b->size == VB_BLOCK_CAPACITY) {
        VBBlock new = vbSplit(b);
        if (c->index >= b->size) {
            c->block = new;
            c->index -= b->size;
        }
    }
}


