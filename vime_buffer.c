#include "vime_buffer.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

VimeBuffer VimeBuffer_new() {
    // Use oalloc to allocate the new VimeBuffer in the heap.
    // Defined in util.h.
    VimeBuffer vb = oalloc(struct VimeBuffer);
    vb->start = VB_Block_new();
    vb->cursor = VB_Cursor_new(vb->start, 0);
    return vb;
}

void VimeBuffer_dispose(VimeBuffer vb) {
    VB_Block b = vb->start;
    while (b) {
        VB_Block temp = b->next;
        VB_Block_dispose(b);
        b = temp;
    }
    VB_Cursor_dispose(vb->cursor);
    free(vb);
}

void VB_validate(VimeBuffer vb) {
	for (VB_Block b = vb->start; b; b = b->next) {
		char buffer[VB_BLOCK_CAPACITY];
		strncpy(buffer, b->payload, b->size);
		buffer[b->size] = '\0';
        printf("%s", buffer);
//		printf("There are %d chars in this block: %s\n", b->size, buffer);
	}
    printf("\n");
    uint i;
    for(i = 0; VB_forward(vb); i++);
    for(i = 0; VB_backward(vb); i++);
    printf("Length: %d\n", i);
}

bool VB_insert(VimeBuffer vb, char ch) {

    // First, get the cursor, so we can get access to the current block.
	VB_Cursor c = vb->cursor;

    VB_Block b = c->block;
    char *insert_pos = b->payload + c->index;

    memmove(insert_pos + 1, insert_pos, b->size - c->index);
    *insert_pos = ch;
    c->index += 1;
    b->size += 1;

    // There must always be space at the end of a block. If we just fillied up
    // this block, split it in half.
	if (b->size == VB_BLOCK_CAPACITY) {
        VB_Block new = VB_Block_split(b);
        if (c->index > b->size) {
            c->block = new;
            c->index -= b->size;
        }
    }
    return true;
}

bool VB_delete(VimeBuffer vb) {
    VB_Cursor c = vb->cursor;
    VB_Block b = c->block;
    char *delete_loc = b->payload + c->index - 1;

    // If the index is at 0, we are at the beginning of the first block, and
    // cannot delete any further.
    // Note: we know this because we only ever allow `index` to become 0 if the
    // cursor is in the first block (otherwise we would have moved to the end
    // of the previous block). See below.
    if (c->index == 0) return false;

    memmove(delete_loc, delete_loc + 1, b->size - c->index);
    c->index -= 1;
    b->size -= 1;

    // If we have just made the block empty, remove it from the linked list.
    // Exception: if this is the first block and it has no successor, we leave
    // it there. The buffer will always contain at least one block.
    if (b->size == 0) {

        // If we are the `start` and have no next, do nothing.
        if (b == vb->start && !b->next) return true;

        // Conditional linked list updates.
        //   1. If the empty block is the `start` of the entire buffer,
        //      update the start pointer to b->next.
        //   2. If the empty block has a previous, bridge its `next` pointer
        //      across to b->next.
        //   3. If the empty block has a next, bridge its `previous` pointer
        //      across to b->previous.
        if (b == vb->start) vb->start = b->next;
        if (b->previous) b->previous->next = b->next;
        if (b->next) b->next->previous = b->previous;
        c->block = b->previous;
        c->index = c->block->size;

        // We don't need the empty block any more, and deallocate it.
        VB_Block_dispose(b);
    }
    return true;
}

bool VB_load(VimeBuffer vb, char *filename) {

    // Attempt to open the file for reading. If we cannot, return false.
	FILE *file = fopen(filename, "r");
    if (!file) return false;

    // Read each character from the file in succession, inserting it into the
    // buffer.
    //
    // TODO: Implement block-level reading/insertion to improve performance on
    // large files.
	while (true){
		char ch = fgetc(file);
		if (feof(file)) break;
        VB_insert(vb, ch);
    }
	fclose(file);
    return true;
}

bool VB_forward(VimeBuffer vb) {
    VB_Cursor c = vb->cursor;
    VB_Block b = c->block;

    // If we are at the end of the document, return false.
    if (c->index == b->size && !b->next) return false;

    c->index += 1;
    if (c->index > b->size) {
        c->block = b->next;
        c->index = 1;
    }
    return true;
}

bool VB_backward(VimeBuffer vb) {
    VB_Cursor c = vb->cursor;
    VB_Block b = c->block;
    
    // If we are at the beginning of the document, return false.
    if (c->index == 0) return false;

    c->index -= 1;
    if (c->index < 1 && b->previous) {
        c->block = b->previous;
        c->index = b->size;
    }
    return true;
}

/*
bool jump(VimeBuffer *vb, int line, int column) {
    Chunk *block = start;
    int lineNumber = 0;
    int columnNumber = 0;
    while(block) {
        for (int i=0; i < block->numChars; i++) {
            if(lineNumber == line) {
                if(columnNumber == column) {
                    vb->c.block = block;
                    vb->c.index = i;
                }
            }
            if(block->buffer[i] == '\n') lineNumber++;
        }
        block = block->next;
    }
}
*/

/*
 * MARK: VB_Block methods.
 */

/*
 * CONSTRUCTOR: VB_Block_new()
 *
 * Creates and returns a new instance of VB_Block, or a single payload block
 * for the vime buffer. Fields will be initialized as follows:
 *     `next`     NULL
 *     `previous` NULL
 *     `size`     0
 *     `payload`  undefined
 *
 * USAGE    VB_Block b = VB_Block_new();
 *
 * RETURN   (VB_Block) The created buffer block.
 */
VB_Block VB_Block_new() {
    VB_Block b = oalloc(struct VB_Block);
    b->next = NULL;
    b->previous = NULL;
    b->size = 0;
    return b;
}

/* METHOD: VB_Block_split(VB_Block old)
 *
 * Splits an existing VB_Block instance into two, each containing half of the
 * original payload. The new block is automatically spliced into the containing
 * linked list, *after* the old block, using the old block's `next` and
 * `previous` fields.
 *
 * USAGE    VB_Block new = VB_Block_split(old);
 *
 * RETURN   (VB_Block) The new (second) block from the split.
 */
VB_Block VB_Block_split(VB_Block old) {

    // Use oalloc to create the new block in the heap. See util.h for
    // implementation.
    VB_Block new = oalloc(struct VB_Block);
    if (!new) return NULL;

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

/* DESTRUCTOR: VB_Block_dispose(VB_Block b)
 *
 * Free any resources associated with the given block.
 *
 * PARAMS
 *     (VB_Block)  The block to dispose.
 * 
 * USAGE    VB_Block_dispose(b);
 */
void VB_Block_dispose(VB_Block b) {
    free(b);
}


VB_Cursor VB_Cursor_new(VB_Block b, uint i) {
    VB_Cursor cursor = oalloc(struct VB_Cursor);
    cursor->block = b;
    cursor->index = i;
    return cursor;
}

void VB_Cursor_dispose(VB_Cursor c) {
    free(c);
}
