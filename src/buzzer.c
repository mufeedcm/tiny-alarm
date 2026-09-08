#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "buzzer.h"

typedef struct {
  uint16_t freq;
  uint8_t  dur;
} buzz_note_t;

static const buzz_note_t alarm_melody[] = {
  {330, 2},
  {415, 2},
  {370, 2},
  {247, 3},
  {0,   2},
  {330, 2},
  {370, 2},
  {415, 2},
  {330, 4},
  {0,   6}
};

#define PATTERN_STEPS (sizeof(alarm_melody) / sizeof(alarm_melody[0]))

static uint8_t pattern_idx = 0;
static uint8_t tick_counter = 0;

void buzz_init(void) {
  DDRB |= (1 << DDB1);
  PORTB |= (1 << PORTB1); 
  buzz_stop();
}

void buzz_stop(void) {
  TCCR1A = 0;
  TCCR1B = 0;
  PORTB |= (1 << PORTB1);
  pattern_idx = 0;
  tick_counter = 0;
}

void buzz_tone(uint16_t freq_hz) {
  if (freq_hz == 0) {
    buzz_stop();
    return;
  }

  uint16_t ocr_val = (uint16_t)((62500UL / freq_hz) - 1);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0; 
  OCR1A = ocr_val;
  TCCR1A = (1 << COM1A0);
  TCCR1B = (1 << WGM12) | (1 << CS11);
}

void buzz_click_beep(void) {
  buzz_tone(2700);
  _delay_ms(15);
  buzz_stop();
}

void buzz_alarm_beep(void) {
  uint16_t freq = alarm_melody[pattern_idx].freq;

  if (freq > 0) {
    buzz_tone(freq);
  } else {
    buzz_stop();
  }

  tick_counter++;
  if (tick_counter >= alarm_melody[pattern_idx].dur) {
    tick_counter = 0;
    pattern_idx = (pattern_idx + 1) % PATTERN_STEPS;
  }
}
