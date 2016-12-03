#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

VimeBuffer *init(){
	VimeBuffer *vb = malloc(sizeof(VimeBuffer));
	vb->start = malloc(sizeof(Chunk));
	vb->start->next = NULL;
	vb->start->numChars = 0;

	vb->c.block = vb->start;
	vb->c.index = 0;

	return vb;
}

void insert(char c) {
	
}

void readFile(VimeBuffer * vb, char *filename) {
	FILE *fp = fopen(filename, "r");
	char ch;
	Cursor *c = &(vb->c);
	c->block = vb->start;
	c->index = 0;
	while(true){
		ch = fgetc(fp);
		if (feof(fp))
			break;
		if (c->index == CHUNK_SIZE) {
			c->block->next = malloc(sizeof(Chunk));
			c->block = c->block->next;
			c->block->numChars = 0;
			c->block->next = NULL;
		}
		c->block->buffer[c->index] = ch;
		c->index++;
		c->block->numChars++;
	}
	fclose(fp);
}
