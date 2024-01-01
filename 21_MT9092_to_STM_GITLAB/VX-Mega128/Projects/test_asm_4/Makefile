#MCU=attiny13a
MCU=atmega128
OBJCOPY=avr-objcopy
CC=avr-gcc
LD=avr-ld
SIZE=avr-size
CFLAGS=-mmcu=$(MCU) -Wall
LDFLAGS=-g -mavr51
OBJ=test_asm_3.o
TARGET=test_asm_3
.PHONY: all clean

%.o:	%.S
		$(CC) -c -o $@ $< $(CFLAGS)
all:	$(OBJ)
		$(LD) -o $(TARGET).elf $(OBJ)  $(LDFLAGS)
		$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
		$(SIZE) -C --mcu=$(MCU) $(TARGET).elf
install:
		avrdude -p m128 -c usbasp -B 4 -P usb -U flash:w:"$(TARGET).hex":a
clean:
		@rm -v $(TARGET).elf $(OBJ)
#		@rm -v $(TARGET).hex
