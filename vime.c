#include "vime_ui.h"
#include "vime_buffer.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

static VimeUI ui;
static VBCursor vc;

char *getLine(uint line);

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("Usage: vime <filename>\n");
        return 0;
    }

    vc = vbOpen(argv[1]);

    ui = VimeUI_new();
    ui->stringForLine = &getLine;
    VUI_render(ui);

    VimeUI_dispose(ui);
    return 0;
}

/*
 * MARK: Vime UI Delegate Methods
 */

// Get string for given line number. This returns the string to the UI when
// it asks for it. It gets the string from the VimeBuf.
char *getLine(uint line) {
    char *string = strdup(" ");
    string[0] = vbGet(vc);
    return string;
}

