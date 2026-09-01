#include "alarm.h"
#include <stdint.h>
#include "rtc.h"
#include "buzzer.h"

static Alarm alarm_data;

void alarm_init(){
  alarm_data.hour = 05;
  alarm_data.minute = 00;
  alarm_data.enabled = 1;
  alarm_data.triggered_today = 0;
}

uint8_t alarm_check(rtc_time *now){
  if(now->hour == 0 && now->minute == 0 && now->second==0){
    alarm_data.triggered_today = 0;
  }
  if (alarm_data.enabled && !alarm_data.triggered_today && (now->hour == alarm_data.hour) && (now->minute == alarm_data.minute)) {
    alarm_data.triggered_today = 1;
    return 1;
  }
  return 0;
}

void alarm_stop(void){
  buzz_stop();
  alarm_data.triggered_today = 1;
}

void alarm_snooze(uint8_t minutes){
  buzz_stop();
  alarm_data.minute = (alarm_data.minute + 2)%60;
  if(alarm_data.minute < 2){
    alarm_data.hour = (alarm_data.hour + 1) % 24;
  }
  alarm_data.triggered_today = 0;
}
