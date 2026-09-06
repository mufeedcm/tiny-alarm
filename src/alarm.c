#include "alarm.h"
#include <stdint.h>
#include <avr/eeprom.h>
#include "rtc.h"
#include "buzzer.h"

static uint8_t eeprom_valid EEMEM;
static uint8_t eeprom_count EEMEM;
static Alarm   eeprom_alarms[MAX_ALARMS] EEMEM;

Alarm alarms[MAX_ALARMS];
uint8_t alarm_count = 0;

void alarm_save(void){
  eeprom_update_byte(&eeprom_count, alarm_count);
  for (uint8_t i = 0; i < MAX_ALARMS; i++) {
    eeprom_update_byte(&eeprom_alarms[i].hour, alarms[i].hour);
    eeprom_update_byte(&eeprom_alarms[i].minute, alarms[i].minute);
    eeprom_update_byte(&eeprom_alarms[i].enabled, alarms[i].enabled);
  }
  eeprom_update_byte(&eeprom_valid, 1);
}

void alarm_init(){
  if(eeprom_read_byte(&eeprom_valid) == 1){
    alarm_count = eeprom_read_byte(&eeprom_count);
    if(alarm_count > MAX_ALARMS) alarm_count = MAX_ALARMS;
    for(uint8_t i = 0; i < MAX_ALARMS; i++){
      alarms[i].hour  = eeprom_read_byte(&eeprom_alarms[i].hour);
      alarms[i].minute  = eeprom_read_byte(&eeprom_alarms[i].minute);
      alarms[i].enabled  = eeprom_read_byte(&eeprom_alarms[i].enabled);
      alarms[i].triggered_today = 0;
    }
  }else{
    alarm_count = 1;
    alarms[0].hour = 05;
    alarms[0].minute = 0;
    alarms[0].enabled = 1;
    alarms[0].triggered_today = 0;
    alarm_save();
  }
}

uint8_t alarm_check(rtc_time *now){
  if(now->hour == 0 && now->minute == 0 && now->second==0){
    for (uint8_t i = 0; i < alarm_count; i++) {
      alarms[i].triggered_today = 0;
    }
  }
  for (uint8_t i = 0; i < alarm_count; i++) {
    if (alarms[i].enabled && !alarms[i].triggered_today && (now->hour == alarms[i].hour) && (now->minute == alarms[i].minute) && now->second < 2) {
      alarms[i].triggered_today = 1;
      return 1;
    }
  }
  return 0;
}

void alarm_stop(void){
  buzz_stop();
}

void alarm_snooze(uint8_t minutes){
  buzz_stop();
  for (uint8_t i = 0; i < alarm_count; i++) {
    if (alarms[i].triggered_today) {
      alarms[i].minute = (alarms[i].minute + minutes)%60;
      if(alarms[i].minute < minutes){
        alarms[i].hour = (alarms[i].hour + 1) % 24;
      }
      alarms[i].triggered_today = 0;
      alarm_save();
      break;
    }
  }
}

uint8_t alarm_add(uint8_t hour, uint8_t minute) {
  if (alarm_count >= MAX_ALARMS) return 0;
  alarms[alarm_count].hour = hour % 24;
  alarms[alarm_count].minute = minute % 60;
  alarms[alarm_count].enabled = 1;
  alarms[alarm_count].triggered_today = 0;
  alarm_count++;
  alarm_save();
  return 1;
}

void alarm_delete(uint8_t index) {
  if (index >= alarm_count) return;
  for (uint8_t i = index; i < alarm_count - 1; i++) {
    alarms[i] = alarms[i + 1];
  }
  alarm_count--;
  alarm_save();
}
