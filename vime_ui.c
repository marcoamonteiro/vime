#include "vime_ui.h"
#include "util.h"
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

VimeUI VimeUI_new() {
    // Setup ncurses.
    initscr();
    cbreak();
    noecho();

    // Setup struct.
    VimeUI vui = oalloc(struct VimeUI);
    vui->line = 1;
    vui->column = 1;
    vui->top = 1;
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
    // The bottom row is saved for status.
    const uint payloadLines = LINES - 1;
    const uint payloadCols = COLS;
    const uint leftCol = 0;
    
    uint l = vui->top;
    string line = NULL;
    for (uint i = 0; i < payloadLines; i += 1) {
        if (!line)
            line = vui->stringForLine(l++);
        if (!line) break;
        mvprintw(i, leftCol, "%.*s", payloadCols, line);
        if (strnlen(line, payloadCols + 1) == payloadCols + 1) {
            line = line + payloadCols;
        } else {
            line = NULL;
        }
    }

    //uint ld = floor(log10(vui->line)) + 1;
    //uint cd = floor(log10(vui->column)) + 1;
    //move(LINES - 1, COLS - ld - cd - 2);
    //printw("%u,%u", ld, cd);

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

