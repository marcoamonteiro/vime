#include <stdbool.h>

#define CHUNK_SIZE 100

typedef struct Chunk {
	char buffer[CHUNK_SIZE];
	struct Chunk *next;
	struct Chunk *previous;
    int numChars;
} Chunk;

typedef struct Cursor {
	Chunk *block;
	int index;
} Cursor;

typedef struct VimeBuffer {
	Chunk *start;
	Cursor c;

} VimeBuffer;

void ValidateBuffer(VimeBuffer *vb);

VimeBuffer *init();

void insert(VimeBuffer *vb, char c);

bool delete(VimeBuffer *vb);

void readFile(VimeBuffer *vb, char *filename);

bool advanceCursor(VimeBuffer *vb);

bool recedeCursor(VimeBuffer *vb);

bool jump(VimeBuffer *vb, int line, int column);
