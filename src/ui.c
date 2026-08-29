#include <stdint.h>
#include <stdio.h>
#include "ssd1306.h"
#include "rtc.h"
#include "button.h"
#include "ui.h"

static UI_State current_state = UI_STATE_CLOCK;

const char *day_names[]={"","MON","TUE","WED","THU","FRI","SAT","SUN"};
const char *month_names[]={"","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

void ui_init(void){
  current_state = UI_STATE_CLOCK;
  display_clear();
}

static void ui_render_clock(){
    // rtc_time initial_time = {
    //   .second = 0,   
    //   .minute = 49,  
    //   .hour   = 20,  
    //   .day    = 6,   
    //   .date   = 29,  
    //   .month  = 8,   
    //   .year   = 26   
    // };
    // rtc_set_time(&initial_time);
    
    rtc_time now;
    rtc_get_time(&now);

    char time_str[16];
    char date_str[20];

    uint8_t h12 = now.hour % 12;
    if(h12 == 0) h12 = 12;
    char *ampm = (now.hour >= 12 ) ? "PM" : "AM";

    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", h12, now.minute, now.second);

    uint8_t day_i = (now.day >= 1 && now.day <= 7 ) ? now.day : 1;
    uint8_t month_i = (now.month >= 1 && now.day <= 12 ) ? now.month : 1;
    uint8_t yr = now.year % 100;

    snprintf(date_str, sizeof(date_str), "%s %02d %s 20%02d", day_names[day_i], now.date, month_names[month_i],yr);
    display_set_cur(40, 1);
    display_write_string(time_str);
    display_set_cur(20, 3);
    display_write_string(date_str);
}

static void ui_render_alarm_list(){
  display_set_cur(40, 1);
  display_write_string("Alarm list");
  display_set_cur(60, 3);
  display_write_string("wip");
}

static void ui_render_edit_clock(uint8_t btn){
  display_set_cur(40, 1);
  display_write_string("Edit Clock");
  display_set_cur(60, 3);
  display_write_string("wip");
}

void ui_update(void){
    uint8_t btn = btn_get_event();

    if(btn & BTN_MODE_CLICK){
      display_clear();
      current_state = (current_state + 1 ) % 3; //loop back
    }
    switch (current_state) {
      case UI_STATE_CLOCK :      ui_render_clock(); break;
      case UI_STATE_ALARM_LIST : ui_render_alarm_list(); break;
      case UI_STATE_EDIT_CLOCK : ui_render_edit_clock(btn); break;
    }
}


