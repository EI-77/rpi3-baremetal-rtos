#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#define GPIO_BASE    0x3F200000
#define GPFSEL0      0x00
#define GPSET0       0x1C
#define GPCLR0       0x28
#define GPLEV0       0x34
#define GPPUD        0x94
#define GPPUDCLK0    0x98

typedef enum {
    GPIO_INPUT  = 0b000,
    GPIO_OUTPUT = 0b001,
    GPIO_ALT0   = 0b100,
    GPIO_ALT2   = 0b110,
    GPIO_ALT3   = 0b111,
    GPIO_ALT4   = 0b011,
    GPIO_ALT5   = 0b010
} gpio_func_t;


void mmio_write(uint32_t addr, uint32_t val);
uint32_t mmio_read(uint32_t addr);
void gpio_init(int pin, gpio_func_t mode);
void gpio_set(int pin);
void gpio_clear(int pin);
void gpio_toggle(int pin);

#endif
