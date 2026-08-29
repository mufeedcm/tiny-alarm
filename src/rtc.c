#include <stdint.h>
#include "i2c.h"
#include "rtc.h"

static uint8_t bcd_to_dec(uint8_t val){
  return ((val / 16 * 10 ) + (val % 16));
}

static uint8_t dec_to_bcd(uint8_t val){
  return ((val / 10 * 16 ) + (val % 10));
}

void rtc_init(){
  i2c_start();
  i2c_write(DS3231_RTC_ADDR << 1); //0x68 << 1 = 0xD0
  i2c_write(0x0E); //set control registor
  i2c_write(0x00); //E0SC = 0;
  i2c_stop();
}

void rtc_get_time(rtc_time *time){
  i2c_start();
  i2c_write(DS3231_RTC_ADDR << 1); //0x68 << 1 = 0xD0
  i2c_write(0x00);

  i2c_start();
  i2c_write((DS3231_RTC_ADDR << 1) | 1); //read mode = 0xD1
  
  time->second = bcd_to_dec(i2c_read_ack());
  time->minute = bcd_to_dec(i2c_read_ack());
  time->hour   = bcd_to_dec(i2c_read_ack()  & 0x3F);
  time->day    = bcd_to_dec(i2c_read_ack());
  time->date   = bcd_to_dec(i2c_read_ack());
  time->month  = bcd_to_dec(i2c_read_ack());
  time->year   = bcd_to_dec(i2c_read_no_ack());

  i2c_stop();
}

void rtc_set_time(rtc_time *time){
  i2c_start();
  i2c_write(DS3231_RTC_ADDR << 1); //0x68 << 1 = 0xD1
  i2c_write(0x00);

  i2c_write(dec_to_bcd(time->second));
  i2c_write(dec_to_bcd(time->minute));
  i2c_write(dec_to_bcd(time->hour) & 0x3F);
  i2c_write(dec_to_bcd(time->day));
  i2c_write(dec_to_bcd(time->date));
  i2c_write(dec_to_bcd(time->month));
  i2c_write(dec_to_bcd(time->year));

  i2c_stop();
}
