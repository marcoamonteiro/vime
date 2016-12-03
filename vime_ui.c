#include "vime_ui.h"
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"

VimeUI VimeUI_new() {
    // Setup ncurses.
    initscr();
    cbreak();
    noecho();
    printf("%d %d\n", COLS, LINES);

    // Setup struct.
    VimeUI vui = malloc(sizeof(struct VimeUI));
    vui->line = 1;
    vui->column = 1;
    return vui;
}

void VimeUI_dispose(VimeUI vui) {
    // Clean up ncurses.
    endwin();
    free(vui);
}

void VimeUI_clear(VimeUI vui) {
    
}

void VimeUI_render(VimeUI vui) {
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

