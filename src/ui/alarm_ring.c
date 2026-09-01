#include <stdint.h>
#include <stdio.h>
#include "../ssd1306.h"
#include "../rtc.h"
#include "../buzzer.h"
#include "ui_internal.h"

static const char *day_names[]={"","MON","TUE","WED","THU","FRI","SAT","SUN"};
static const char *month_names[]={"","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

static void ui_render_time(rtc_time time){
    char time_str[16];

    uint8_t h12 = time.hour % 12;
    if(h12 == 0) h12 = 12;
    char *ampm = (time.hour >= 12 ) ? "PM" : "AM";
    snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d %s", h12, time.minute, time.second, ampm);
    display_set_cur(35, 1);
    display_write_string(time_str);
}

static void ui_render_btn_info(){

    display_set_cur(40, 3);
    display_write_string("[STOP]");
    display_set_cur(80, 3);
    display_write_string("[SNOOZE]");
}
void ui_render_alarm_ring(){
    rtc_time now;
    rtc_get_time(&now);

    ui_render_time(now);
    ui_render_btn_info();
    buzz_alarm_beep();
}

