#include <stdint.h>
#include "alarm.h"
#include "ssd1306.h"
#include "button.h"
#include "ui.h"
#include "rtc.h"
#include "ui/ui_internal.h"
#include "buzzer.h"

static UI_State current_state = UI_STATE_CLOCK;


void ui_init(void){
  current_state = UI_STATE_CLOCK;
  alarm_init();
  display_clear();
}

void ui_update(void){
  uint8_t btn = btn_get_event();

  rtc_time now;
  rtc_get_time(&now);

  //brightness control
  static uint8_t is_night = 0;
  uint8_t night = (now.hour >= 22 || now.hour < 6);
  if (night != is_night) {
    is_night = night;
    display_set_brightness(is_night ? 0x01 : 0xFF);
  }

  if(current_state == UI_STATE_ALARM_RING){

    if (btn & BTN_SELECT_CLICK) {
      alarm_stop();
      display_clear();
      current_state = UI_STATE_CLOCK;
    }else if (btn & BTN_SET_CLICK) {
      alarm_snooze(5);
      display_clear();
      current_state = UI_STATE_CLOCK;
    }
  }else{
    if(alarm_check(&now)){
      display_clear();
      current_state = UI_STATE_ALARM_RING;
    }else if(btn & BTN_MODE_CLICK){
      display_clear();
      if(current_state == UI_STATE_EDIT_CLOCK){
        ui_edit_clock_exit();
      }

      current_state = (current_state + 1 ) % 3; //loop back
      buzz_click_beep();

      if(current_state == UI_STATE_EDIT_CLOCK){
        ui_edit_clock_init();
      }
    }
  }

  switch (current_state) {
    case UI_STATE_CLOCK :      ui_render_clock(); break;
    case UI_STATE_ALARM_RING:  ui_render_alarm_ring();break;
    case UI_STATE_ALARMS :     ui_render_alarms(); break;
    case UI_STATE_EDIT_CLOCK : ui_render_edit_clock(btn); break;
  }
}


