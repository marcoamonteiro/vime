#include <stdio.h>
#include "vime_ui.h"
#include "buffer.h"
#include "util.h"

static VimeUI ui;
static VimeUIDelegate uiDelegate;
static VimeBuffer buffer;

char *getLine(uint line);

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("Usage: vime <filename>\n");
        return 0;
    }

    buffer = VimeBuffer_new();
    VimeBuffer_readFile(buffer, argv[1]);
    ui = VimeUI_new();

    uiDelegate = VimeUIDelegate_new();
    uiDelegate->stringForLine = &getLine;
    VimeUI_render(ui);
    VimeUI_dispose(ui);
    return 0;
}

/*
 * MARK: Vime UI Delegate Methods
 */

// Get string for given line number. This returns the string to the UI when
// it asks for it. It gets the string from the VimeBuf.
char *getLine(uint line) {
    return VimeBuffer_getLine(buffer, line);
}

