#ifndef __VIME_BUF_H
#define __VIME_BUF_H

#include <stdbool.h>
#include "util.h"

#define VB_BLOCK_CAPACITY 100

typedef struct VBBlock {
    struct VBBlock *next;
    struct VBBlock *previous;
	char payload[VB_BLOCK_CAPACITY];
    uint size;
} *VBBlock;

typedef struct VBCursor {
    // File level. Tracks absolute location in the file as a byte number.
    uint pos;

    // Block level. Tracks internal block pointer and index into block.
	VBBlock block;
	uint index;

    // Text level. Stores line and column number.
    uint line;
    uint column;
} *VBCursor;

VBCursor vbOpen(char *filename);
VBCursor vbDup(VBCursor c);
void vbClose(VBCursor c);
void vbDispose(VBCursor c);

VBBlock vbBlock(void);
VBBlock vbSplit(VBBlock old);

bool vbEOF(VBCursor c);
bool vbSOF(VBCursor c);

#define END 0

bool vbForward(VBCursor c);
bool vbBackward(VBCursor c);
bool vbTo(VBCursor c, uint pos);
bool vbToLine(VBCursor c, uint line);
bool vbToCol(VBCursor c, uint col);
bool vbMove(VBCursor c, int delta);
bool vbMoveLine(VBCursor c, int delta);
bool vbMoveCol(VBCursor c, int delta);

char vbGet(VBCursor c);
void vbPut(VBCursor c, char ch);
char vbDelete(VBCursor c);
void vbInsert(VBCursor c, char ch);

uint vbDistance(VBCursor c1, VBCursor c2);

#endif
