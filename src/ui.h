#ifndef UI_H
#define UI_H 

#include <stdint.h>

typedef enum{
  UI_STATE_CLOCK,
  UI_STATE_ALARMS,
  UI_STATE_EDIT_CLOCK,
  UI_STATE_ALARM_RING,
}UI_State;

void ui_init(void);
void ui_update(void);

#endif

