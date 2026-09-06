#include <stdio.h>
#include <stdint.h>
#include "../ssd1306.h"
#include "../alarm.h"
#include "../buzzer.h"
#include "ui_internal.h"

void ui_render_alarms(uint8_t sel_idx, uint8_t top_idx){
  uint8_t total_items = (alarm_count < MAX_ALARMS) ? (alarm_count + 1) : MAX_ALARMS;
  for(uint8_t i =0; i < 2; i++){
    uint8_t idx = top_idx + i;
    if(idx >= total_items) break;
    char str[20];
    uint8_t page = (i == 0) ? 0 : 2;

    if (idx < alarm_count) {
      uint8_t h12 = alarms[idx].hour % 12;
      if (h12 == 0) h12 = 12;
      const char *ampm = (alarms[idx].hour >= 12) ? "PM" : "AM";
      char symbol = alarms[idx].enabled ? '*' : 'o';

      if (idx == sel_idx) {
        snprintf(str, sizeof(str), "[ %d:%02d%s  %c ]", h12, alarms[idx].minute, ampm, symbol);
      } else {
        snprintf(str, sizeof(str), "%d:%02d%s  %c ",  h12, alarms[idx].minute, ampm, symbol);
      }
    } else {
      if (idx == sel_idx) {
        snprintf(str, sizeof(str), "[ + new ]");
      } else {
        snprintf(str, sizeof(str), " + new ");
      }
    }

    display_write_string(str, 0, page);
  }
}
