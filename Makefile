MCU         = atmega328p
TARGET_ARCH = -mmcu=$(MCU)
TARGET      = main
CC          = avr-gcc
CPPFLAGS    = -mmcu=$(MCU)
CFLAGS      = -Os -g -Wall -I. -DF_CPU=16000000UL
LDFLAGS     = -g -mmcu=$(MCU) -lm -Wl,--gc-sections -Os
PGMER       = -c arduino -b 115200 -P /dev/ttyACM0
PGMERISP    = -c avrispv2 -P /dev/ttyACM0
DUDE        = /usr/bin/avrdude -V -p $(MCU)

C_SRCS      = $(wildcard *.c)
OBJ_FILES   = $(C_SRCS:.c=.o)

all:    $(TARGET).hex

clean:
		rm -f $(TARGET).elf *.o *.hex

%.o: %.c
		$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJ_FILES)
		$(CC) $(LDFLAGS) -o $@ $(OBJ_FILES)

$(TARGET).hex: $(TARGET).elf
		avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
		avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex main.elf eeprom.hex

upload: $(TARGET).hex
		$(DUDE) $(PGMER) -U flash:w:main.hex

size: main.elf
		avr-size --format=avr --mcu=$(MCU) main.elf
