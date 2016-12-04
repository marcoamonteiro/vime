#include "vime_buffer.h"
#include <stdio.h>
#include <string.h>

void buffer_dump(VBCursor c);

int main() {
    VBCursor vc = vbOpen("happy.txt");
    buffer_dump(vc);
    vbClose(vc);
}

void buffer_dump(VBCursor c) {
    uint i;
    for (i = 0; vbBackward(c); i += 1);

    uint line = 0;
    uint col = 0;
    while (true) {
        if (c->line != line) {
            line = c->line;
            col = 0;
            printf("%d: ", line);
        }
        if (vbGet(c) == '\n') {
            printf(" (%d)", col);
        }
        if (vbEOF(c)) break;
        printf("%c", vbGet(c));
        vbForward(c);
        col += 1;
    }
    printf("\n\n%d\n", c->line);
    for (i = 0; vbBackward(c); i += 1);
	for (VBBlock b = c->block; b; b = b->next) {
		char buffer[VB_BLOCK_CAPACITY];
		strncpy(buffer, b->payload, b->size);
		buffer[b->size] = '\0';
        printf("%s", buffer);
    }
    printf("\n");
    for (i = 0; vbBackward(c); i += 1);
    for (i = 0; vbForward(c); i += 1);
    for (i = 0; vbBackward(c); i += 1);
    for (i = 0; vbForward(c); i += 1);
    for (i = 0; vbBackward(c); i += 1);
    for (i = 0; vbForward(c); i += 1);
    for (i = 0; vbBackward(c); i += 1);
    for (i = 0; vbForward(c); i += 1);
    printf("length: %d\n", i);

    VBCursor cp = vbDup(c);
    for (i = 0; vbBackward(cp); i += 1);
    printf("length: %d\n", vbDistance(cp, c));
    printf("lines: %d\n", c->line);
    vbDispose(cp);

    const uint l = 18;
    vbToLine(c, l);
    printf("Line %d is '%s'.\n", l, vbLine(c));
}

