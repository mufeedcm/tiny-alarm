#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "rtc.h"
#include "button.h"
#include "ssd1306.h"
#include "ui.h"

int main(void){
  i2c_init();
  display_init();
  rtc_init();
  btn_init();
  ui_init();
  while (1) {
    ui_update();
    _delay_ms(100);
  }

  return 0;

}

