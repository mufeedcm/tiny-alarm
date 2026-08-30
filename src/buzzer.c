#include <avr/io.h>
#include <avr/delay.h>
#include <stdint.h>
#include <util/delay.h>
#include "buzzer.h"

void buzz_init(void){
  DDRB |= ( 1 << DDB1);
  PORTB &= (1 << PORTB);

}
void buzz_stop(void){
  PORTB &= ~(1 << PORTB1);
}

void buzz_click_beep(void){
  PORTB |= (1 << PORTB1);  
  _delay_ms(40);
  PORTB &= ~(1 << PORTB1); 
}

void buzz_alarm_beep(void){
    PORTB |= (1 << PORTB1);
    _delay_ms(120);
    PORTB &= ~(1 << PORTB1);
    _delay_ms(100);

    PORTB |= (1 << PORTB1);
    _delay_ms(120);
    PORTB &= ~(1 << PORTB1);
    _delay_ms(300);

    PORTB |= (1 << PORTB1);
    _delay_ms(120);
    PORTB &= ~(1 << PORTB1);
    _delay_ms(100);

    PORTB |= (1 << PORTB1);
    _delay_ms(120);
    PORTB &= ~(1 << PORTB1);
    _delay_ms(300);

    PORTB |= (1 << PORTB1);
    _delay_ms(80);
    PORTB &= ~(1 << PORTB1);
    _delay_ms(70);

    PORTB |= (1 << PORTB1);
    _delay_ms(80);
    PORTB &= ~(1 << PORTB1);
    _delay_ms(70);

    PORTB |= (1 << PORTB1);
    _delay_ms(160);
    PORTB &= ~(1 << PORTB1);
    _delay_ms(800);
}
