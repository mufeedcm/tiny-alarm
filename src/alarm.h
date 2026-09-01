#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>
#include "rtc.h"

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t enabled;
  uint8_t triggered_today;
} Alarm;

void alarm_init(void);
uint8_t alarm_check(rtc_time *now); 
void alarm_stop(void);
void alarm_snooze(uint8_t minutes);

#endif
