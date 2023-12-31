MCU=atmega128
OBJCOPY=avr-objcopy
CC=avr-gcc
SIZE=avr-size
CFLAGS=-mmcu=$(MCU) -Wall
LDFLAGS=-mmcu=$(MCU)
OBJ=main.o
TARGET=test_asm_2
.PHONY: all clean

%.o:	%.S
	$(CC) -c -o $@ $< $(CFLAGS)
all:	$(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET).elf  $(OBJ)
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(SIZE) -C --mcu=$(MCU) $(TARGET).elf
#install:
#        avrdude  -p m128 -c usbasp -B 4 -P usb   -U flash:w:./$(TARGET).hex:i
#clean:
#	@rm -v $(TARGET).elf $(TARGET).hex $(OBJ)
