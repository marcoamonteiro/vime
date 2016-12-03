#ifndef __VIME_UI_H
#define __VIME_UI_H

#include "util.h"

typedef struct VimeUI {
    uint line;
    uint column;
    char *(*stringForLine)(uint line);
} *VimeUI;

VimeUI VimeUI_new(void);
void VimeUI_dispose(VimeUI vui);

void VUI_up(VimeUI vui);
void VUI_down(VimeUI vui);
void VUI_left(VimeUI vui);
void VUI_right(VimeUI vui);
void VUI_toLine(VimeUI vui, uint line);
void VUI_toColumn(VimeUI vui, uint column);

void VUI_clear(VimeUI vui);
void VUI_render(VimeUI vui);

#endif
