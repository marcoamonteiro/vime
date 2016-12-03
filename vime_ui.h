#ifndef __VIME_UI_H
#define __VIME_UI_H

typedef struct {
    size_t line;
    size_t column;
} *VimeUI;

VimeUI VimeUI_new(void);
void VimeUI_clear(void);
void VimeUI_render(void);
void VimeUI_setLine(size_t line);
void VimeUI_setColumn(size_t column);

#endif
