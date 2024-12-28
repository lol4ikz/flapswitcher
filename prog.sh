#!/bin/bash
avr-gcc -g -Os -mmcu=attiny13a -c attiny13.c
avr-gcc -g -mmcu=attiny13a -o attiny13.elf attiny13.o
avr-objcopy -j .text -j .data -O ihex attiny13.elf attiny13.hex
avr-size --format=avr --mcu=attiny13a attiny13.elf
