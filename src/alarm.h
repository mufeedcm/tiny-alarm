#ifndef ALARM_H
#define ALARM_H

#include <stdint.h>
#include "rtc.h"

#define MAX_ALARMS 10

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t enabled;
  uint8_t triggered_today;
} Alarm;

extern Alarm alarms[MAX_ALARMS]; 
extern uint8_t alarm_count;

void alarm_init(void);
uint8_t alarm_check(rtc_time *now); 
void alarm_stop(void);
void alarm_snooze(uint8_t minutes);
void alarm_save(void);
uint8_t alarm_add(uint8_t hour,uint8_t minute);
void alarm_delete(uint8_t idx);

#endif
