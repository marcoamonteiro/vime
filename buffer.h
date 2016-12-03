#define CHUNK_SIZE = 100;

typedef struct Chunk {
	char buffer[CHUNK_SIZE];
	Chunk *next;
	int numChars;
} Chunk;

typedef struct Cursor {
	Chunk *block;
	int index;
} Cursor;

Chunk *start;
Cursor c;

void insert(char c);

void readFile(
