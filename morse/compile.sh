avr-gcc -g -Os -mmcu=atmega328 -c morse.c
avr-gcc -g -mmcu=atmega328 -o morse.elf morse.o
avr-objcopy -j .text -j .data -O ihex morse.elf morse.hex
avr-size --format=avr --mcu=atmega328 morse.elf
