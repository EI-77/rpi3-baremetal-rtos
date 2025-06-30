#include <stdint.h>
#include "gpio_driver.h"

// Perform a memory-mapped I/O write to a peripheral register
void mmio_write(uint32_t addr, uint32_t val) {
    *(volatile uint32_t*)addr = val;
}

// Perform a memory-mapped I/O read from a peripheral register
uint32_t mmio_read(uint32_t addr) {
    return *(volatile uint32_t*)addr;
}

// Initialize a GPIO pin with the specified function mode
void gpio_init(int pin, gpio_func_t mode) {
    int reg = pin / 10;             // Each GPFSEL register controls 10 GPIOs
    int shift = (pin % 10) * 3;     // Each GPIO uses 3 bits in the GPFSEL register

    uint32_t fsel_addr = GPIO_BASE + (GPFSEL0 + reg * 4);  // Address of the target GPFSEL register
    uint32_t val = mmio_read(fsel_addr);
    val &= ~(0b111 << shift);       // Clear the 3 bits for the target pin
    val |= ((mode & 0b111) << shift); // Set the desired mode
    mmio_write(fsel_addr, val);
}

// Set (drive high) a GPIO pin
void gpio_set(int pin) {
    if (pin < 32) mmio_write(GPIO_BASE + GPSET0, (1 << pin));
}

// Clear (drive low) a GPIO pin
void gpio_clear(int pin) {
    if (pin < 32) mmio_write(GPIO_BASE + GPCLR0, (1 << pin));
}

// Toggle the state of a GPIO pin (high ↔ low)
void gpio_toggle(int pin) {
    if (pin < 32) {
        uint32_t val = mmio_read(GPIO_BASE + GPLEV0);  // Read current pin level
        if (val & (1 << pin)) {
            gpio_clear(pin);  // If high, set low
        }
        else {
            gpio_set(pin);    // If low, set high
        }
    }
}
