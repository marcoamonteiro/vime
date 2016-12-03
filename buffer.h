#define CHUNK_SIZE 100

typedef struct Chunk {
	char buffer[CHUNK_SIZE];
	struct Chunk *next;
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

VimeBuffer *init();

void insert(char c);

void readFile(VimeBuffer *vb, char *filename);
