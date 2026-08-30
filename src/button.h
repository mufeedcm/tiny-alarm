#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

#define BTN_NONE 0x00
#define BTN_MODE_CLICK   (1<<0)
#define BTN_SELECT_CLICK (1<<1)
#define BTN_SET_CLICK    (1<<2)

void btn_init(void);
uint8_t btn_get_event(void);

#endif
