//ref page 183 - https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf 
#include <avr/io.h>
#include <stdint.h>
#include "i2c.h"  

void i2c_init(void){
  PRR &= ~(1 << PRTWI);
  TWBR = 2; // bit rate
  TWSR = 0; // prescaler
}

void i2c_wait_for_complete(void){
  while (!(TWCR &(1<<TWINT)));
}

void i2c_start(void){
  TWCR =(1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
  i2c_wait_for_complete();
}

uint8_t i2c_read_ack(void){
  TWCR =(1<<TWINT) | (1<<TWEN) | (1<<TWEA);
  i2c_wait_for_complete();
  return TWDR;
}

uint8_t i2c_read_no_ack(void){
  TWCR =(1<<TWINT) | (1<<TWEN);
  i2c_wait_for_complete();
  return TWDR;

}

void i2c_write(uint8_t data){
  TWDR = data;
  TWCR = (1<<TWINT) | (1<<TWEN);
  i2c_wait_for_complete();
}

void i2c_stop(void){
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); 
}
