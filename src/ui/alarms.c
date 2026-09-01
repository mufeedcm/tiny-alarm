#include "../ssd1306.h"
#include "../buzzer.h"
#include "ui_internal.h"

void ui_render_alarms(){
  display_set_cur(40, 1);
  display_write_string("Alarm list");
  display_set_cur(60, 3);
  display_write_string("wip");
  // buzz_alarm_beep();
}
