#include <stdint.h>
#include <stdio.h>
#include "ssd1306.h"
#include "rtc.h"
#include "button.h"
#include "ui.h"
#include "buzzer.h"

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

    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d %s", h12, now.minute, now.second, ampm);

    uint8_t day_i = (now.day >= 1 && now.day <= 7 ) ? now.day : 1;
    uint8_t month_i = (now.month >= 1 && now.month <= 12 ) ? now.month : 1;
    uint8_t yr = now.year % 100;

    snprintf(date_str, sizeof(date_str), "%s %02d %s 20%02d", day_names[day_i], now.date, month_names[month_i],yr);
    display_set_cur(35, 1);
    display_write_string(time_str);
    display_set_cur(20, 3);
    display_write_string(date_str);
}

static void ui_render_alarm_list(){
  display_set_cur(40, 1);
  display_write_string("Alarm list");
  display_set_cur(60, 3);
  display_write_string("wip");
  buzz_alarm_beep();
}

static uint8_t edit_field = 0;
static rtc_time edit_time;
static uint8_t sec_edited = 0;


static void ui_render_edit_clock(uint8_t btn){

    if(btn & BTN_SELECT_CLICK){
      display_clear();
      edit_field = (edit_field + 1) % 8;
    }

    uint8_t max_days = (edit_time.month == 2) ? ((edit_time.year % 4 == 0) ? 29 : 28) : ((edit_time.month == 4 ||  edit_time.month == 6 || edit_time.month == 9 || edit_time.month == 11) ? 30 : 31);
    

    if(btn & BTN_SET_CLICK){
      switch (edit_field) {
        case 0: 
          edit_time.hour++;
          if(edit_time.hour >= 24) edit_time.hour = 0;
          break;
        case 1: 
          edit_time.minute++;
          if(edit_time.minute >= 60) edit_time.minute = 0;
          break;
        case 2: 
          edit_time.second = 0;
          sec_edited = 1;
          break;
        case 3: 
          edit_time.hour = (edit_time.hour + 12)%24;
          break;
        case 4: 
          edit_time.day++;
          if(edit_time.day > 7) edit_time.day = 1;
          break;
        case 5: 
          edit_time.date++;
          if(edit_time.date > max_days) edit_time.date = 1;
          break;
        case 6: 
          edit_time.month++;
          if(edit_time.month > 12) edit_time.month = 1;
          break;
        case 7: 
          edit_time.year++;
          if(edit_time.year >= 99) edit_time.year = 01;
          break;
      }
    }
    if(edit_time.date > max_days ) edit_time.date = max_days; // clamp date


    uint8_t h12 = edit_time.hour % 12;
    if(h12 == 0) h12 = 12;
    char *ampm = (edit_time.hour >= 12 ) ? "PM" : "AM";

    uint8_t day_i = (edit_time.day >= 1 && edit_time.day <= 7 ) ? edit_time.day : 1;
    uint8_t month_i = (edit_time.month >= 1 && edit_time.month <= 12 ) ? edit_time.month : 1;
    uint8_t yr = edit_time.year % 100;

    char line1[20];
    char line2[20];

    if (edit_field == 0) {
        snprintf(line1, sizeof(line1), "[%02d]:%02d:%02d %s", h12, edit_time.minute, edit_time.second, ampm);
    } else if (edit_field == 1) {
        snprintf(line1, sizeof(line1), "%02d:[%02d]:%02d %s", h12, edit_time.minute, edit_time.second, ampm);
    } else if (edit_field == 2) {
        snprintf(line1, sizeof(line1), "%02d:%02d:[%02d] %s", h12, edit_time.minute, edit_time.second, ampm);
    } else if (edit_field == 3) {
        snprintf(line1, sizeof(line1), "%02d:%02d:%02d [%s]", h12, edit_time.minute, edit_time.second, ampm);
    } else {
        snprintf(line1, sizeof(line1), "%02d:%02d:%02d %s", h12, edit_time.minute, edit_time.second, ampm);
    }

    if (edit_field == 4) {
        snprintf(line2, sizeof(line2), "[%s] %02d %s 20%02d", day_names[day_i], edit_time.date, month_names[month_i], yr);
    } else if (edit_field == 5) {
        snprintf(line2, sizeof(line2), "%s [%02d] %s 20%02d", day_names[day_i], edit_time.date, month_names[month_i], yr);
    } else if (edit_field == 6) {
        snprintf(line2, sizeof(line2), "%s %02d [%s] 20%02d", day_names[day_i], edit_time.date, month_names[month_i], yr);
    } else if (edit_field == 7) {
        snprintf(line2, sizeof(line2), "%s %02d %s [20%02d]", day_names[day_i], edit_time.date, month_names[month_i], yr);
    } else {
        snprintf(line2, sizeof(line2), "%s %02d %s 20%02d", day_names[day_i], edit_time.date, month_names[month_i], yr);
    }

    display_set_cur(20, 1);
    display_write_string(line1);
    display_set_cur(15, 3);
    display_write_string(line2);
}

void ui_update(void){
    uint8_t btn = btn_get_event();

    if(btn & BTN_MODE_CLICK){
      display_clear();
      if(current_state == UI_STATE_EDIT_CLOCK){
        if(!sec_edited){
        rtc_time live;
        rtc_get_time(&live);
        edit_time.second = live.second;
        }
        rtc_set_time(&edit_time);
      }

      current_state = (current_state + 1 ) % 3; //loop back
      buzz_click_beep();

      if(current_state == UI_STATE_EDIT_CLOCK){
        rtc_get_time(&edit_time);
        edit_field = 0;
        sec_edited = 0;
      }
    }
    switch (current_state) {
      case UI_STATE_CLOCK :      ui_render_clock(); break;
      case UI_STATE_ALARM_LIST : ui_render_alarm_list(); break;
      case UI_STATE_EDIT_CLOCK : ui_render_edit_clock(btn); break;
    }
}


