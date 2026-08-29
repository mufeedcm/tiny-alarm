#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "rtc.h"
#include "ssd1306.h"

const char *day_names[]={"","MON","TUE","WED","THU","FRI","SAT","SUN"};
const char *month_names[]={"","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};

int main(void){
  i2c_init();
  display_init();
  rtc_init();

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
  char time_str[16];
  char date_str[20];

  while (1) {
    rtc_get_time(&now);
    
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
    _delay_ms(500);
  }

  return 0;

}

