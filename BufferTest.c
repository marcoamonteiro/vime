#include "buffer.h"
#include <stdio.h>

int main() {
	VimeBuffer *vb = init();
	readFile(vb, "happy.txt");
	ValidateBuffer(vb);
	//while(recedeCursor(vb));
    //while(advanceCursor(vb)) {
      //  insert(vb, 'a');
    //}
    /*
    insert(vb, '#');
    recedeCursor(vb);
    printf("%c\n", vb->c.block->buffer[vb->c.index]);
    recedeCursor(vb);
    insert(vb, '@');
    */
    while(delete(vb));
    ValidateBuffer(vb);
    return 0;
}
