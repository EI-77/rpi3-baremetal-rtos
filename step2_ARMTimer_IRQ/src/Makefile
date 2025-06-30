CROSS    := arm-none-eabi-
CC       := $(CROSS)gcc
OBJCOPY  := $(CROSS)objcopy

CFLAGS   := -march=armv6 -mfpu=vfp -mfloat-abi=hard \
            -nostdlib -nostartfiles -ffreestanding -O2
LDFLAGS  := -T linker.ld -Wl,--build-id=none

CSRCS := main.c uart_driver.c gpio_driver.c irq.c
OBJS := $(CSRCS:.c=.o)

.PHONY: all clean

all: kernel.img

kernel.img: kernel.elf
	$(OBJCOPY) -O binary $< $@

kernel.elf: start.o $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ start.o $(OBJS)

start.o: start.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.img
