MCU = atmega328p
F_CPU = 1000000UL
PORT = /dev/ttyUSB0
SRC = src/i2c.c src/ssd1306.c src/rtc.c src/main.c
VERSION = 0.0.0

all:	flash

flash:
	avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Isrc $(SRC) -o main.elf && avr-objcopy -O ihex -R .eeprom main.elf main.hex
	avrdude -c stk500v1 -p $(MCU) -P $(PORT) -b 19200 -U flash:w:main.hex:i

clean: 
	rm -rf main.elf main.hex

.PHONY: all flash clean
