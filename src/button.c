#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "button.h"

static volatile uint8_t btn_event_flags = BTN_NONE;

void btn_init(void){
  // pd3 = touch, pd4 = select, pc0 = dec, pc1 = inc.

  //set d4, c0 and c1 as i/p
  //removed int0 for rtc int
  DDRD &= ~((1 << DDD3) | (1 << DDD4)); 
  DDRC &= ~((1 << DDC0) | (1 << DDC1)); 
                                        
  // turns on internal pull up res for d4,c0,c1
  PORTD |= (1 << PORTD4); 
  PORTC |= (1 << PORTC0) | (1 << PORTC1);

  EICRA = (1 << ISC11) | (1 << ISC10); //set rising edge for int1
  EIMSK = (1 << INT1); //enable int1

  //set ext int
  PCICR  |= (1 << PCIE1) | (1 << PCIE2);
  PCMSK2 |= (1 << PCINT20);
  PCMSK1 |= (1 << PCINT8)  | (1 << PCINT9);

  sei();
}

ISR(INT1_vect){
  if((PIND & (1 << PIND3))){
    btn_event_flags |= BTN_MODE_TOUCH;
  }
}

ISR(PCINT2_vect){
  if (!(PIND & (1 << PIND4))) {
    btn_event_flags |= BTN_SELECT_CLICK;
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

