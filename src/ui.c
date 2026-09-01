#include <stdint.h>
#include "ssd1306.h"
#include "button.h"
#include "ui.h"
#include "ui/ui_internal.h"
#include "buzzer.h"

static UI_State current_state = UI_STATE_CLOCK;

void ui_init(void){
  current_state = UI_STATE_CLOCK;
  display_clear();
}

void ui_update(void){
  uint8_t btn = btn_get_event();

  if(btn & BTN_MODE_CLICK){
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
  switch (current_state) {
    case UI_STATE_CLOCK :      ui_render_clock(); break;
    case UI_STATE_ALARMS :     ui_render_alarms(); break;
    case UI_STATE_EDIT_CLOCK : ui_render_edit_clock(btn); break;
  }
}


