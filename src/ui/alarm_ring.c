#include <stdint.h>
#include <stdio.h>
#include "../ssd1306.h"
#include "../rtc.h"
#include "../buzzer.h"
#include "ui_internal.h"

static const char *day_names[]={"","MON","TUE","WED","THU","FRI","SAT","SUN"};
static const char *month_names[]={"","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

static void ui_render_time(rtc_time time){
  uint8_t h12 = time.hour % 12;
  if (h12 == 0) h12 = 12;

  uint8_t digits[5];
  digits[0] = (h12 >= 10) ? (h12 / 10) : 11;
  digits[1] = h12 % 10;
  digits[2] = 10; 
  digits[3] = time.minute / 10;
  digits[4] = time.minute % 10;

  uint8_t col = 26;
  for (uint8_t i = 0; i < 5; i++) {
    display_write_big_digits(digits[i], col);
    col += 16;
  }
  char *ampm = (time.hour >= 12) ? "PM" : "AM";
  display_write_string(ampm, 106, 0);
}

static void ui_render_btn_info(){
    display_write_string("[STOP]", 40, 3);
    display_write_string("[SNOOZE]", 80, 3);
}
void ui_render_alarm_ring(){
    rtc_time now;
    rtc_get_time(&now);

    ui_render_time(now);
    ui_render_btn_info();
    buzz_alarm_beep();
}
