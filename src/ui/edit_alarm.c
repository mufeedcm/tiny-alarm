#include <stdint.h>
#include <stdio.h>
#include "../ssd1306.h"
#include "../alarm.h"
#include "ui_internal.h"

static uint8_t current_idx = 0;

void ui_edit_alarm_init(uint8_t idx) {
  current_idx = idx;
}

void ui_render_edit_alarm(uint8_t sel_field) {
  if (current_idx >= alarm_count) return;

  uint8_t h12 = alarms[current_idx].hour % 12;
  if (h12 == 0) h12 = 12;
  const char *ampm = (alarms[current_idx].hour >= 12) ? "PM" : "AM";
  const char *status_str = alarms[current_idx].enabled ? "ON" : "OFF";

  char line[20];

  if (sel_field == 0) {
    snprintf(line, sizeof(line), "[%02d]:%02d %s %s [DEL]", h12, alarms[current_idx].minute, ampm, status_str);
  } else if (sel_field == 1) {
    snprintf(line, sizeof(line), "%02d:[%02d] %s %s [DEL]", h12, alarms[current_idx].minute, ampm, status_str);
  } else if (sel_field == 2) {
    snprintf(line, sizeof(line), "%02d:%02d %s [%s] [DEL]", h12, alarms[current_idx].minute, ampm, status_str);
  } else {
    snprintf(line, sizeof(line), "%02d:%02d %s %s [[DEL]]", h12, alarms[current_idx].minute, ampm, status_str);
  }
  display_write_string(line, 0, 2);
}
