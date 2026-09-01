#include <stdint.h>
#include <stdio.h>
#include "../ssd1306.h"
#include "../rtc.h"
#include "../button.h"
#include "ui_internal.h"

#define CHANGED_HOUR    (1 << 0)
#define CHANGED_MINUTE  (1 << 1)
#define CHANGED_SECOND  (1 << 2)
#define CHANGED_DAY     (1 << 3)
#define CHANGED_DATE    (1 << 4)
#define CHANGED_MONTH   (1 << 5)
#define CHANGED_YEAR    (1 << 6)

static const char *day_names[]={"","MON","TUE","WED","THU","FRI","SAT","SUN"};
static const char *month_names[]={"","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

static rtc_time draft_time;
static uint8_t changed_fields;
static uint8_t selected_field;

void ui_render_edit_clock(uint8_t btn){

  if(btn & BTN_SELECT_CLICK){
    display_clear();
    selected_field = (selected_field + 1) % 8;
  }

  uint8_t max_days = (draft_time.month == 2) ? ((draft_time.year % 4 == 0) ? 29 : 28) : ((draft_time.month == 4 ||  draft_time.month == 6 || draft_time.month == 9 || draft_time.month == 11) ? 30 : 31);

  if(btn & BTN_SET_CLICK){
    switch (selected_field) {
      case 0: 
        draft_time.hour++;
        if(draft_time.hour >= 24) draft_time.hour = 0;
        changed_fields |= CHANGED_HOUR;
        break;
      case 1: 
        draft_time.minute++;
        if(draft_time.minute >= 60) draft_time.minute = 0;
        changed_fields |= CHANGED_MINUTE;
        break;
      case 2: 
        draft_time.second = 0;
        changed_fields |= CHANGED_SECOND;
        break;
      case 3: 
        draft_time.hour = (draft_time.hour + 12)%24;
        changed_fields |= CHANGED_HOUR;
        break;
      case 4: 
        draft_time.day++;
        if(draft_time.day > 7) draft_time.day = 1;
        changed_fields |= CHANGED_DAY;
        break;
      case 5: 
        draft_time.date++;
        if(draft_time.date > max_days) draft_time.date = 1;
        changed_fields |= CHANGED_DATE;
        break;
      case 6: 
        draft_time.month++;
        if(draft_time.month > 12) draft_time.month = 1;
        changed_fields |= CHANGED_MONTH;
        break;
      case 7: 
        draft_time.year++;
        if(draft_time.year >= 99) draft_time.year = 01;
        changed_fields |= CHANGED_YEAR;
        break;
    }
  }
  if(draft_time.date > max_days ) draft_time.date = max_days; // clamp date


  uint8_t h12 = draft_time.hour % 12;
  if(h12 == 0) h12 = 12;
  char *ampm = (draft_time.hour >= 12 ) ? "PM" : "AM";

  uint8_t day_i = (draft_time.day >= 1 && draft_time.day <= 7 ) ? draft_time.day : 1;
  uint8_t month_i = (draft_time.month >= 1 && draft_time.month <= 12 ) ? draft_time.month : 1;
  uint8_t yr = draft_time.year % 100;

  char line1[20];
  char line2[20];

  if (selected_field == 0) {
    snprintf(line1, sizeof(line1), "[%02d]:%02d:%02d %s", h12, draft_time.minute, draft_time.second, ampm);
  } else if (selected_field == 1) {
    snprintf(line1, sizeof(line1), "%02d:[%02d]:%02d %s", h12, draft_time.minute, draft_time.second, ampm);
  } else if (selected_field == 2) {
    snprintf(line1, sizeof(line1), "%02d:%02d:[%02d] %s", h12, draft_time.minute, draft_time.second, ampm);
  } else if (selected_field == 3) {
    snprintf(line1, sizeof(line1), "%02d:%02d:%02d [%s]", h12, draft_time.minute, draft_time.second, ampm);
  } else {
    snprintf(line1, sizeof(line1), "%02d:%02d:%02d %s", h12, draft_time.minute, draft_time.second, ampm);
  }

  if (selected_field == 4) {
    snprintf(line2, sizeof(line2), "[%s] %02d %s 20%02d", day_names[day_i], draft_time.date, month_names[month_i], yr);
  } else if (selected_field == 5) {
    snprintf(line2, sizeof(line2), "%s [%02d] %s 20%02d", day_names[day_i], draft_time.date, month_names[month_i], yr);
  } else if (selected_field == 6) {
    snprintf(line2, sizeof(line2), "%s %02d [%s] 20%02d", day_names[day_i], draft_time.date, month_names[month_i], yr);
  } else if (selected_field == 7) {
    snprintf(line2, sizeof(line2), "%s %02d %s [20%02d]", day_names[day_i], draft_time.date, month_names[month_i], yr);
  } else {
    snprintf(line2, sizeof(line2), "%s %02d %s 20%02d", day_names[day_i], draft_time.date, month_names[month_i], yr);
  }

  display_set_cur(20, 1);
  display_write_string(line1);
  display_set_cur(15, 3);
  display_write_string(line2);
}

void ui_edit_clock_init(){
  rtc_get_time(&draft_time);
  changed_fields = 0;
  selected_field =0;
}

void ui_edit_clock_exit(){
  if(changed_fields == 0) return;

  rtc_time now;
  rtc_get_time(&now);

  rtc_time final_time;
  final_time.second = (changed_fields & CHANGED_SECOND) ? draft_time.second : now.second;
  final_time.minute = (changed_fields & CHANGED_MINUTE) ? draft_time.minute : now.minute;
  final_time.hour   = (changed_fields & CHANGED_HOUR)   ? draft_time.hour   : now.hour;
  final_time.day    = (changed_fields & CHANGED_DAY)    ? draft_time.day    : now.day;
  final_time.date   = (changed_fields & CHANGED_DATE)   ? draft_time.date   : now.date;
  final_time.month  = (changed_fields & CHANGED_MONTH)  ? draft_time.month  : now.month;
  final_time.year   = (changed_fields & CHANGED_YEAR)   ? draft_time.year   : now.year;

  rtc_set_time(&final_time);
}
