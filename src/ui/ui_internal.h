#ifndef UI_INTERNAL_H
#define UI_INTERNAL_H 

#include <stdint.h>
#include "../rtc.h"

void ui_render_clock();
void ui_render_alarm_ring();
void ui_render_alarms(uint8_t sel_idx, uint8_t top_idx);
void ui_render_edit_clock(uint8_t btn);
void ui_render_edit_alarm(uint8_t sel_field);

void ui_edit_clock_init();
void ui_edit_clock_exit();


void ui_edit_alarm_init(uint8_t idx);
#endif


