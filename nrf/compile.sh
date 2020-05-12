avr-gcc -g -Os -mmcu=atmega328 -c nrf.c
avr-gcc -g -mmcu=atmega328 -o nrf.elf nrf.o
avr-objcopy -j .text -j .data -O ihex nrf.elf nrf.hex
avr-size --format=avr --mcu=atmega328 nrf.elf
