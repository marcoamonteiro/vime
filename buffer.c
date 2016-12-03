#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

VimeBuffer *init(){
	VimeBuffer *vb = malloc(sizeof(VimeBuffer));
	vb->start = malloc(sizeof(Chunk));
	vb->start->next = NULL;
    vb->start->previous = NULL;
	vb->start->numChars = 0;

	vb->c.block = vb->start;
	vb->c.index = 0;

	return vb;
}

void ValidateBuffer(VimeBuffer *vb) {
	Chunk *c = vb->start;
	while(c) {
		char buffer[CHUNK_SIZE+1];
		strncpy(buffer, c->buffer, c->numChars);
		buffer[c->numChars] = '\0';
        printf("%s", buffer);
//		printf("There are %d chars in this block: %s\n", c-> numChars, buffer);
		c = c->next;
	}
    printf("\n");
    int i;
    for(i=0; recedeCursor(vb); i++);
    for(i = 0; advanceCursor(vb); i++);
    printf("Length: %d\n", i);
}

void insert(VimeBuffer *vb, char c) {
	Cursor *cur = &(vb->c);
    
    memmove(cur->block->buffer + cur->index+1, cur->block->buffer + cur->index, cur->block->numChars - cur->index);
    cur->block->buffer[cur->index] = c;
    cur->index++;
    cur->block->numChars++;
    
    int numChars = cur->block->numChars;
    int index = cur->index;
	if(numChars == CHUNK_SIZE) {
	    Chunk *newChunk = malloc(sizeof(Chunk));
        newChunk->next = cur->block->next;
        newChunk->previous = cur->block;
        if(cur->block->next)
            cur->block->next->previous = newChunk;
        cur->block->next = newChunk;
        newChunk->numChars = numChars / 2;
        cur->block->numChars -= newChunk->numChars;
        memcpy(newChunk->buffer, cur->block->buffer + cur->block->numChars, newChunk->numChars);
	    if (index >= cur->block->numChars) {
            cur->block = newChunk;
            cur->index = index - cur->block->numChars;
        }
    }
}

void readFile(VimeBuffer * vb, char *filename) {
	FILE *fp = fopen(filename, "r");
	char ch;
	Cursor *c = &(vb->c);
	c->block = vb->start;
	c->index = 0;
	while (true){
		ch = fgetc(fp);
		if (feof(fp)) break;
        insert(vb, ch);
        /*
		if (c->index == CHUNK_SIZE) {
			c->block->next = malloc(sizeof(Chunk));
			c->block = c->block->next;
			c->block->numChars = 0;
			c->block->next = NULL;
			c->index = 0;
		}
		//printf("%c", ch);
		c->block->buffer[c->index] = ch;
		c->index++;
		c->block->numChars++;
	    */
    }
	fclose(fp);
}

bool advanceCursor(VimeBuffer *vb) {
    Cursor *c = &(vb->c);
    if (c->index == c->block->numChars && !c->block->next) return false;
    c->index++;
    if (c->index > c->block->numChars) {
        c->block = c->block->next;
        c->index = 1;
    }
    return true;
}

bool recedeCursor(VimeBuffer *vb) {
    Cursor *c = &(vb->c);
    if (c->index == 0 && !c->block->previous) return false;
    c->index--;
    if (c->index < 1 && c->block->previous) {
        c->block = c->block->previous;
        c->index = c->block->numChars;
    }
    return true;
}

bool delete(VimeBuffer *vb) {
    Cursor *c = &(vb->c);
    if(c->block == vb->start && c->index == 0) return false;
    
    memmove(c->block->buffer + c->index-1, c->block->buffer + c->index, c->block->numChars - c->index);
    c->index--;
    c->block->numChars--;

    if(c->block->numChars == 0) {
        if (c->block == vb->start) {
            if (c->block->next) {
                vb->start = c->block->next;
                vb->start->previous = NULL;
                free(c->block);
                c->block = vb->start;
            }
        } else {
            c->block->previous->next = c->block->next;
            if (c->block->next)
                c->block->next->previous = c->block->previous;
            Chunk *previous = c->block->previous;
            free(c->block);
            c->block = previous;
            c->index = c->block->numChars;
        }
    }
    return true;   
}

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
