#ifndef SSD1306_H
#define SSD1306_H 

#include <stdint.h>

#define SSD1306_DISPLAY_ADDR        0x3C
#define SSD1306_SCREEN_WIDTH        128
#define SSD1306_SCREEN_HEIGHT       32

#define SSD1306_SETCONTRAST         0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4 //resume ram contents
#define SSD1306_DISPLAYALLON        0xA5 //ignore ram contents dispaly full white
#define SSD1306_NORMALDISPLAY       0xA6 //bg black
#define SSD1306_INVERTDISPLAY       0xA7 //bg white
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETDISPLAYOFFSET    0xD3 //set display offset
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB //set pixel off voltage
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 //set clock speed
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETMULTIPLEX        0xA8 //set display height
#define SSD1306_SETLOWCOLUMN        0x00  //set lower 4 bits of column
#define SSD1306_SETHIGHCOLUMN       0x10  //set upper 4 bits of column
#define SSD1306_SETSTARTLINE        0x40 //set start line (0x40-0x7F)
#define SSD1306_SETMEMORYMODE       0x20  
#define SSD1306_SETCOLUMNADDR       0x21  //set col start/end range
#define SSD1306_SETPAGEADDR         0x22  //set page start/end range
#define SSD1306_COMSCANINC          0xC0 //set display orientation
#define SSD1306_COMSCANDEC          0xC8 //set display orientation
#define SSD1306_SEGREMAP_OFF        0xA0 //set mirroring off
#define SSD1306_SEGREMAP_ON         0xA1 //set mirroring on
#define SSD1306_CHARGEPUMP          0x8D


void display_init(void);
void display_clear(void);
void display_set_cur(uint8_t column,uint8_t page);
void display_write_char(char c, uint8_t col, uint8_t p);
void display_write_string(const char *str, uint8_t col, uint8_t p);
void display_write_big_digits(uint8_t col, uint8_t digit_idx);

#endif
