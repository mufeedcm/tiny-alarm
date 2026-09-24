#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "button.h"


static volatile uint8_t btn_event_flags = BTN_NONE;


void btn_init(void){
  // removed int0 for rtc int
  DDRD &= ~((1 << DDD3) | (1 << DDD4) | (1 << DDD5)); // d3, d4 and d5 as i/p
  DDRC &= ~((1 << DDC0) | (1 << DDC1)) ; // c0 and c1 as i/p
 
  PORTD |= (1 << PORTD3) | (1 << PORTD4); // turns on internal pull up res
  PORTC |= (1 << PORTC0) | (1 << PORTC1); // "
  // touch module pd5 has it's own 5v, so no need for pull up

  EICRA = (1 << ISC11);
  EIMSK = (1 << INT1);

  PCICR  |= (1 << PCIE1)   | (1 << PCIE2); // d4 and d5 as int
  PCMSK2 |= (1 << PCINT20) | (1 << PCINT21) ;
  PCMSK1 |= (1 << PCINT8)  | (1 << PCINT9);

  sei();
}

ISR(INT1_vect){
  if(!(PIND & (1 << PIND3))){
    btn_event_flags |= BTN_MODE_CLICK;
  }
}

ISR(PCINT2_vect){
  if (!(PIND & (1 << PIND4))) {
    btn_event_flags |= BTN_SELECT_CLICK;
  }
  if (PIND & (1 << PIND5)) { // checking if it's high
    btn_event_flags |= BTN_TOUCH_CLICK;
  }
}

ISR(PCINT1_vect){
  if (!(PINC & (1 << PINC0))) {
    btn_event_flags |= BTN_DEC_CLICK;
  }
  if (!(PINC & (1 << PINC1))) {
    btn_event_flags |= BTN_INC_CLICK;
  }
}

uint8_t btn_get_event(void){
  cli();
  uint8_t ev = btn_event_flags;
  btn_event_flags = BTN_NONE;
  sei();
  return ev;
}

