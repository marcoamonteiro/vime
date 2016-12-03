#ifndef __VIME_UI_H
#define __VIME_UI_H

#include "util.h"

typedef char *(*VimeUILineSource)(uint);

typedef struct VimeUIDelegate {
    VimeUILineSource stringForLine;
} *VimeUIDelegate;

typedef struct VimeUI {
    uint line;
    uint column;
    VimeUIDelegate delegate;
} *VimeUI;

VimeUI VimeUI_new(void);
void VimeUI_dispose(VimeUI vui);

void VimeUI_up(VimeUI vui);
void VimeUI_down(VimeUI vui);
void VimeUI_left(VimeUI vui);
void VimeUI_right(VimeUI vui);
void VimeUI_toLine(VimeUI vui, uint line);
void VimeUI_toColumn(VimeUI vui, uint column);

void VimeUI_clear(VimeUI vui);
void VimeUI_render(VimeUI vui);

VimeUIDelegate VimeUIDelegate_new(void);
void VimeUIDelegate_dispose(VimeUIDelegate d);

#endif
