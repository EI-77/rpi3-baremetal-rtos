#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

// Base address of GPIO peripheral (for Raspberry Pi 3 with BCM2837)
#define GPIO_BASE    0x3F200000

// GPIO register offsets
#define GPFSEL0      0x00  // Function Select 0 (GPIO 0–9)
#define GPSET0       0x1C  // Pin Output Set 0 (GPIO 0–31)
#define GPCLR0       0x28  // Pin Output Clear 0 (GPIO 0–31)
#define GPLEV0       0x34  // Pin Level 0 (read current state)
#define GPPUD        0x94  // GPIO Pull-up/down Enable
#define GPPUDCLK0    0x98  // GPIO Pull-up/down Enable Clock 0

// Enum for GPIO pin function modes
typedef enum {
    GPIO_INPUT  = 0b000,  // Input mode
    GPIO_OUTPUT = 0b001,  // Output mode
    GPIO_ALT0   = 0b100,  // Alternate function 0
    GPIO_ALT2   = 0b110,  // Alternate function 2
    GPIO_ALT3   = 0b111,  // Alternate function 3
    GPIO_ALT4   = 0b011,  // Alternate function 4
    GPIO_ALT5   = 0b010   // Alternate function 5
} gpio_func_t;

// Memory-mapped I/O write
void mmio_write(uint32_t addr, uint32_t val);

// Memory-mapped I/O read
uint32_t mmio_read(uint32_t addr);

// Configure the function of a GPIO pin (input/output/alt)
void gpio_init(int pin, gpio_func_t mode);

// Set the output level of a GPIO pin to high (1)
void gpio_set(int pin);

// Clear the output level of a GPIO pin to low (0)
void gpio_clear(int pin);

// Toggle the output level of a GPIO pin (high ↔ low)
void gpio_toggle(int pin);

#endif
