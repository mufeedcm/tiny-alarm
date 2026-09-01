#ifndef UI_INTERNAL_H
#define UI_INTERNAL_H 

#include <stdint.h>

void ui_render_clock();
void ui_render_alarms();
void ui_render_edit_clock(uint8_t btn);

void ui_edit_clock_init();
void ui_edit_clock_exit();

#endif


