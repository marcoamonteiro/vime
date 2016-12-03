#include "vime_ui.h"
#include "util.h"
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

VimeUI VimeUI_new() {
    // Setup ncurses.
    initscr();
    cbreak();
    noecho();

    // Setup struct.
    VimeUI vui = oalloc(struct VimeUI);
    vui->line = 1;
    vui->column = 1;
    return vui;
}

void VimeUI_dispose(VimeUI vui) {
    // Clean up ncurses.
    endwin();
    free(vui);
}

void VUI_clear(VimeUI vui) {
    
}

void VUI_render(VimeUI vui) {
    uint ld = floor(log10(vui->line)) + 1;
    uint cd = floor(log10(vui->column)) + 1;
    move(LINES - 1, COLS - ld - cd - 2);
    printw("%u,%u", ld, cd);

    while (true) {
        char ch = getch();
        if (ch == 'q') break;
        printw("%c", ch);
    }
}

void VimeUI_setLine(VimeUI ui, uint line) {
    ui->line = line;
}

void VimeUI_setColumn(VimeUI ui, uint col) {
    ui->column = col;
}

VimeUIDelegate VimeUIDelegate_new() {
    VimeUIDelegate delegate = malloc(sizeof(struct VimeUIDelegate));
    return delegate;
}

void VimeUIDelegate_dispose(VimeUIDelegate d) {
    free(d);
}
