#include "../ssd1306.h"
#include "../buzzer.h"
#include "ui_internal.h"

void ui_render_alarms(){
  display_write_string("Alarm list", 40, 1);
  display_write_string("wip",60, 3);
  // buzz_alarm_beep();
}
