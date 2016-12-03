#include <stdio.h>
#include "vime_ui.h"

static VimeUI ui;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: vime <filename>\n");
        return 0;
    }
    ui = VimeUI_new();
    VimeUI_render(ui);
    VimeUI_dispose(ui);
    return 0;
}
