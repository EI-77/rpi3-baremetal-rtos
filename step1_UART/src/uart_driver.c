#include <stdint.h>
#include "uart_driver.h"
#include "gpio_driver.h"

// UART0 base address on Raspberry Pi 3 (PL011 UART)
#define UART0_BASE   0x3F201000

// UART register offsets
#define UART0_DR     0x00  // Data Register
#define UART0_FR     0x18  // Flag Register
#define UART0_IBRD   0x24  // Integer Baud Rate Divisor
#define UART0_FBRD   0x28  // Fractional Baud Rate Divisor
#define UART0_LCRH   0x2C  // Line Control Register
#define UART0_CR     0x30  // Control Register
#define UART0_IMSC   0x38  // Interrupt Mask Set/Clear Register

// Initialize UART0 (PL011) for basic transmission
void uart_init(void) {
    // Disable UART before configuration
    mmio_write(UART0_BASE + UART0_CR, 0);

    // Set GPIO14 (TXD0) and GPIO15 (RXD0) to ALT0 function (UART0)
    gpio_init(14, GPIO_ALT0);
    gpio_init(15, GPIO_ALT0);

    // Disable GPIO pull-up/down for pins 14 and 15
    mmio_write(GPIO_BASE + GPPUD, 0);
    for (volatile int i = 0; i < 150; i++);  // Wait cycles
    mmio_write(GPIO_BASE + GPPUDCLK0, (1 << 14) | (1 << 15));
    for (volatile int i = 0; i < 150; i++);  // Wait cycles
    mmio_write(GPIO_BASE + GPPUDCLK0, 0);    // Remove clock

    // Set baud rate to 115200:
    // UARTCLK = 48 MHz, BaudDiv = 26 + 3/16
    mmio_write(UART0_BASE + UART0_IBRD, 26); // Integer part
    mmio_write(UART0_BASE + UART0_FBRD, 3);  // Fractional part

    // Configure for 8-bit, no parity, one stop bit (8N1), FIFO enabled
    mmio_write(UART0_BASE + UART0_LCRH, (3 << 5) | (1 << 4));

    // Mask all interrupts (disable them)
    mmio_write(UART0_BASE + UART0_IMSC, 0);

    // Enable UART, TX, and RX
    mmio_write(UART0_BASE + UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

// Transmit a single character over UART
void uart_putc(char c) {
    // Wait until transmit FIFO is not full
    while (mmio_read(UART0_BASE + UART0_FR) & (1 << 5));
    mmio_write(UART0_BASE + UART0_DR, (uint32_t)c);
}

// Transmit a null-terminated string over UART
void uart_puts(const char* s) {
    while (*s) {
        uart_putc(*s++);
    }
}
