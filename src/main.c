#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "ssd1306.h"

int main(void){
  i2c_init();
  display_init();

  display_set_cur(40, 2);
  display_write_string("Booting...");

  _delay_ms(2000);

  display_clear();
  display_set_cur(0, 2);
  display_write_string("Display is Working...");

  while (1) {
  }
  return 0;

}

