## Tiny Alarm 
A minimal and simple alarm clock.

> [!Warning]
This project is currently in early development.

---
### Main Parts
- Atmega 328pu.
- ssd1306 128x32 oled display.
- ds3231 rtc.
- buzzer.
- push buttons.
- avr programmer ( I am using Arduino as Isp )

### Build

#### Prerequisites (linux)

- avrdude avr-gcc avr-binutils avr-libc avr-gdb
- make

### Todo

- add touch hold.
- set initial new alarm time to current time.
- improve daily alarm reset.
- create a lot more custom font.
- improve ui.
- add more alarm tones
- automatic day calculation from date.
- 12hr and 24hr toggle.
- add battery charge.
- add rtc power loss warning.
- add snooze limit.
- add alarm day scheduling (daily/mon-fri).
- add stopwatch/timer.
- design pcb & case.
