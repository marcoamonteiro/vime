#include "buffer.h"

int main() {
	VimeBuffer *vb = init();
	readFile(vb, "happy.txt");
	ValidateBuffer(vb);
	return 0;
}
