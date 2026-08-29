//ref
//https://github.com/tibounise/SSD1306-AVR/blob/master/SSD1306.h
//https://github.com/adafruit/Adafruit_SSD1306/blob/master/Adafruit_SSD1306.h

#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include "i2c.h"
#include "font.h"
#include "ssd1306.h"

void display_cmd(uint8_t cmd){
  i2c_start();
  i2c_write(SSD1306_DISPLAY_ADDR << 1); // addr to 8 bit
  i2c_write(0x00); //setting next byte as a cmd
  i2c_write(cmd);
  i2c_stop();
}

void display_data(uint8_t data){
  i2c_start();
  i2c_write(SSD1306_DISPLAY_ADDR << 1); //0x3c << 1 = 0x78
  i2c_write(0x40); //setting next byte as pixel data
  i2c_write(data);
  i2c_stop();
}

void display_set_cur(uint8_t column,uint8_t page){
  display_cmd(0xB0 + page);                   //page start location, 0xB0=page1 0xB1=page2.. 
  display_cmd(SSD1306_SETLOWCOLUMN + (column & 0x0F));        
  display_cmd(SSD1306_SETHIGHCOLUMN + ((column >> 4) & 0x0F));
}

void display_clear(void){
  for(uint8_t page = 0; page < (SSD1306_SCREEN_HEIGHT/8) ; page++){
    display_set_cur(0, page);
    for(uint8_t col = 0; col < SSD1306_SCREEN_WIDTH; col++){
      display_data(0x00); // set background 00-FF
    }
  }
}

void display_init(void) {
  _delay_ms(100);

  display_cmd(SSD1306_DISPLAYOFF);                             
  display_cmd(SSD1306_SETDISPLAYCLOCKDIV); display_cmd(0x80);  
  display_cmd(SSD1306_SETMULTIPLEX);       display_cmd(0x1F);  //set display height 1F=31(rows), 0x3F=63(rows)
  display_cmd(SSD1306_SETCOMPINS);         display_cmd(0x02);  //0x02 for 32 and 0x12 for 64
  display_cmd(SSD1306_SETDISPLAYOFFSET);   display_cmd(0x00);  //set display offset
  display_cmd(SSD1306_SETSTARTLINE);                           
  display_cmd(SSD1306_CHARGEPUMP);         display_cmd(0x14);  //boost 5v-7.5v
  display_cmd(SSD1306_SETMEMORYMODE);      display_cmd(0x00);  //0x00 horizotal, 0x01 vertical, 0x02 page mode
  display_cmd(SSD1306_SEGREMAP_ON);        display_cmd(SSD1306_COMSCANDEC); //set mirroring display orientation
  display_cmd(SSD1306_SETCONTRAST);        display_cmd(0xFF);  //set brightness (0x01-0xFF)
  display_cmd(SSD1306_SETPRECHARGE);       display_cmd(0xF1);  //set precharge period (pixel)
  display_cmd(SSD1306_SETVCOMDETECT);      display_cmd(0x40);  //set pixel off voltage 0x40 = 0.77vcc
  display_cmd(SSD1306_DISPLAYALLON_RESUME);                    //resume ram contents
  display_cmd(SSD1306_NORMALDISPLAY);                         
  display_cmd(SSD1306_DISPLAYON);                            

  display_clear();  
}

void display_write_char(char c) {
  if (c < 32 || c > 126) c = ' '; 
  for (uint8_t i = 0; i < 5; i++) {
    uint8_t line = pgm_read_byte(&font[c - 32][i]);
    display_data(line); 
  }
  display_data(0x00); //1px gap b/w letters
}

void display_write_string(const char *str) {
  while (*str) {
    display_write_char(*str++);
  }
}
