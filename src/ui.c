#include <stdint.h>
#include "alarm.h"
#include "ssd1306.h"
#include "button.h"
#include "ui.h"
#include "rtc.h"
#include "ui/ui_internal.h"
#include "buzzer.h"

static UI_State current_state = UI_STATE_CLOCK;

static uint8_t alarm_sel = 0;
static uint8_t alarm_top = 0;
static uint8_t alarm_field = 0;


void ui_init(void){
  current_state = UI_STATE_CLOCK;
  alarm_init();
  display_clear();
}

static void ui_handle_clock(uint8_t btn){
  if(btn & BTN_MODE_CLICK){
    display_clear();
    buzz_click_beep();
    current_state = UI_STATE_ALARMS;
    return;
  }
  ui_render_clock();

}
static void ui_handle_alarms(uint8_t btn) {
  uint8_t total = (alarm_count < MAX_ALARMS) ? (alarm_count + 1) : MAX_ALARMS;

  if (btn & BTN_MODE_CLICK) {
    display_clear();
    buzz_click_beep();
    ui_edit_clock_init();
    current_state = UI_STATE_EDIT_CLOCK;
    return;
  }

  if (btn & BTN_SELECT_CLICK) {
    display_clear();
    alarm_sel = (alarm_sel + 1) % total;
    if (alarm_sel >= alarm_top + 2) {
      alarm_top = alarm_sel - 1;
    } else if (alarm_sel == 0) {
      alarm_top = 0;
    }
  }

  if (btn & BTN_SET_CLICK) {
    display_clear();
    alarm_field = 0;
    if (alarm_sel < alarm_count) {
      ui_edit_alarm_init(alarm_sel);
      current_state = UI_STATE_EDIT_ALARM;
    } else {
      if (alarm_add(7, 0)) {
        ui_edit_alarm_init(alarm_count - 1);
        current_state = UI_STATE_EDIT_ALARM;
      }
    }
    return;
  }

  ui_render_alarms(alarm_sel, alarm_top);
}

static void ui_handle_edit_clock(uint8_t btn) {
  if (btn & BTN_MODE_CLICK) {
    display_clear();
    buzz_click_beep();
    ui_edit_clock_exit();
    current_state = UI_STATE_CLOCK;
    return;
  }
  ui_render_edit_clock(btn);
}

static void ui_handle_edit_alarm(uint8_t btn) {
  if (btn & BTN_MODE_CLICK) {
    display_clear();
    buzz_click_beep();
    current_state = UI_STATE_ALARMS; 
    return;
  }

  if (btn & BTN_SELECT_CLICK) {
    display_clear();
    alarm_field = (alarm_field + 1) % 4;
  }

  if (btn & BTN_SET_CLICK) {
    if (alarm_sel < alarm_count) {
      switch (alarm_field) {
        case 0: alarms[alarm_sel].hour = (alarms[alarm_sel].hour + 1) % 24; break;
        case 1: alarms[alarm_sel].minute = (alarms[alarm_sel].minute + 1) % 60; break;
        case 2: alarms[alarm_sel].enabled = !alarms[alarm_sel].enabled; break;
        case 3:
          alarm_delete(alarm_sel);
          display_clear();
          current_state = UI_STATE_ALARMS;
          return;
      }
      alarm_save();
    }
  }

  ui_render_edit_alarm(alarm_field);
}

static void ui_handle_alarm_ring(uint8_t btn) {
  if (btn & BTN_SELECT_CLICK) {
    alarm_stop();
    display_clear();
    current_state = UI_STATE_CLOCK;
    return;
  } else if (btn & BTN_SET_CLICK) {
    alarm_snooze(5);
    display_clear();
    current_state = UI_STATE_CLOCK;
    return;
  }
  ui_render_alarm_ring();
}

void ui_update(void){
  uint8_t btn = btn_get_event();

  rtc_time now;
  rtc_get_time(&now);

  static uint8_t is_night = 0;
  uint8_t night = (now.hour >= 22 || now.hour < 6);
  if (night != is_night) {
    is_night = night;
    display_set_brightness(is_night ? 0x01 : 0xFF);
  }

  if ((current_state != UI_STATE_ALARM_RING && current_state != UI_STATE_EDIT_ALARM) && alarm_check(&now)) {
    display_clear();
    current_state = UI_STATE_ALARM_RING;
  }

  switch (current_state) {
    case UI_STATE_CLOCK:      ui_handle_clock(btn);      break;
    case UI_STATE_ALARMS:     ui_handle_alarms(btn);     break;
    case UI_STATE_EDIT_CLOCK: ui_handle_edit_clock(btn); break;
    case UI_STATE_EDIT_ALARM: ui_handle_edit_alarm(btn); break;
    case UI_STATE_ALARM_RING: ui_handle_alarm_ring(btn); break;
  }
}
