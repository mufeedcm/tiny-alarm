#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

void buzz_init(void);
void buzz_stop(void);
void buzz_click_beep(void);
void buzz_alarm_beep(void);

#endif
