#include <stdint.h>
#include <stdio.h>
#include "../ssd1306.h"
#include "../rtc.h"
#include "../button.h"
#include "../buzzer.h"
#include "ui_internal.h"

static const char *day_names[]={"","MON","TUE","WED","THU","FRI","SAT","SUN"};
static const char *month_names[]={"","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

void ui_render_clock(){
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
