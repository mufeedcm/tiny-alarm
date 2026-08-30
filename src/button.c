#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "button.h"


static volatile uint8_t btn_event_flags = BTN_NONE;


void btn_init(void){
  DDRD &= ~((1 << DDD2) | (1 << DDD3)); // d2 and d3 as i/p
  DDRC &= ~(1 << DDC0);
 
  PORTD |= (1 << PORTD2 | 1 << PORTD3); // turns on internal pull up res
  PORTC |= (1 << PORTC0); // "

  EICRA = (1 << ISC01) | (1 << ISC11);
  EIMSK = (1 << INT0) | (1 << INT1);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT8);

  sei();
}

ISR(INT0_vect){
  _delay_ms(20);
  if(!(PIND & (1 << PIND2))){
    btn_event_flags |= BTN_MODE_CLICK;
  }
}

ISR(INT1_vect){
  _delay_ms(20);
  if(!(PIND & (1 << PIND3))){
    btn_event_flags |= BTN_SELECT_CLICK;
  }
}

ISR(PCINT1_vect){
  _delay_ms(20);
  if(!(PINC & (1 << PINC0))){
    btn_event_flags |= BTN_SET_CLICK;
  }
}


uint8_t btn_get_event(void){
  cli();
  uint8_t ev = btn_event_flags;
  btn_event_flags = BTN_NONE;
  sei();
  return ev;
}

