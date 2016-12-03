#ifndef __VIME_UI_H
#define __VIME_UI_H

#include "util.h"

typedef struct VimeUI {
    uint line;
    uint column;
} *VimeUI;

VimeUI VimeUI_new(void);
void VimeUI_dispose(VimeUI vui);

void VimeUI_clear(VimeUI vui);
void VimeUI_render(VimeUI vui);
void VimeUI_setLine(VimeUI vui, uint line);
void VimeUI_setColumn(VimeUI vui, uint column);

#endif
