#ifndef RTC_H
#define RTC_H 

#include <stdint.h>

#define DS3231_RTC_ADDR  0x68

typedef struct{
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t date;
  uint8_t month;
  uint8_t year;
}rtc_time;

void rtc_init(void);
void rtc_get_time(rtc_time *time);
void rtc_set_time(rtc_time *time);

#endif

