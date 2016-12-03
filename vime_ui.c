#include "vime_ui.h"
#include <ncurses.h>

VimeUI VimeUI_new() {
    // Setup ncurses.
    initscr();
    cbreak();
    noecho();
    printf("%d %d\n", COLS, LINES);

    // Setup struct.
    VimeUI vui = malloc(sizeof(*VimeUI));
    vui->line = 1;
    vui->column = 1;
    return vui;
}

void VimeUI_clear() {
    
}

void VimeUI_render() {
    
}

void VimeUI_setLine(VimeUI ui, size_t line) {
    ui->line = line;
}

void VimeUI_setColumn(VimeUI ui, size_t col) {
    ui->column = col;
}

