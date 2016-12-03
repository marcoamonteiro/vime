#ifndef __VIME_BUF_H
#define __VIME_BUF_H

#include <stdbool.h>
#include "util.h"

#define VB_BLOCK_CAPACITY 100

typedef struct VB_Block {
    struct VB_Block *next;
    struct VB_Block *previous;
	char payload[VB_BLOCK_CAPACITY];
    uint size;
} *VB_Block;

typedef struct VB_Cursor {
	VB_Block block;
	uint index;
} *VB_Cursor;

typedef struct VimeBuffer {
	VB_Block start;
	VB_Cursor cursor;
} *VimeBuffer;

void ValidateBuffer(VimeBuffer *vb);

VimeBuffer VimeBuffer_new(void);
void VimeBuffer_dispose(VimeBuffer vb);
bool VB_insert(VimeBuffer vb, char c);
bool VB_delete(VimeBuffer vb);
bool VB_load(VimeBuffer vb, char *filename);
bool VB_forward(VimeBuffer vb);
bool VB_backward(VimeBuffer vb);
// bool VB_jump(VimeBuffer vb, uint line, uint column);

VB_Block VB_Block_new(void);
VB_Block VB_Block_split(VB_Block old);
void VB_Block_dispose(VB_Block b);

VB_Cursor VB_Cursor_new(VB_Block b, uint i);

#endif
