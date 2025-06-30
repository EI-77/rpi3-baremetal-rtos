#include <stdint.h>
#include "gpio_driver.h"

void mmio_write(uint32_t addr, uint32_t val) {
    *(volatile uint32_t*)addr = val;
}

uint32_t mmio_read(uint32_t addr) {
    return *(volatile uint32_t*)addr;
}

void gpio_init(int pin, gpio_func_t mode) {
    int reg = pin / 10;             // Each GPFSEL covers 10 GPIOs
    int shift = (pin % 10) * 3;     // Each pin uses 3 bits

    uint32_t fsel_addr = GPIO_BASE + (GPFSEL0 + reg * 4);
    uint32_t val = mmio_read(fsel_addr);
    val &= ~(0b111 << shift);       // Clear current function
    val |= ((mode & 0b111) << shift); // Set new function
    mmio_write(fsel_addr, val);
}

void gpio_set(int pin) {
    if (pin < 32) mmio_write(GPIO_BASE + GPSET0, (1 << pin));
}

void gpio_clear(int pin) {
    if (pin < 32) mmio_write(GPIO_BASE + GPCLR0, (1 << pin));
}

void gpio_toggle(int pin) {
    if (pin < 32) {
        uint32_t val = mmio_read(GPIO_BASE + GPLEV0);
        if (val & (1 << pin))
            gpio_clear(pin);
        else
            gpio_set(pin);
    }
}
