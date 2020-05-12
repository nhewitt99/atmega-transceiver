avr-gcc -g -Os -mmcu=atmega328 -c irq.c
avr-gcc -g -mmcu=atmega328 -o irq.elf irq.o
avr-objcopy -j .text -j .data -O ihex irq.elf irq.hex
avr-size --format=avr --mcu=atmega328 irq.elf
