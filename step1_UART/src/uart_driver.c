#include <stdint.h>
#include "uart_driver.h"
#include "gpio_driver.h"

#define UART0_BASE   0x3F201000

#define UART0_DR     0x00
#define UART0_FR     0x18
#define UART0_IBRD   0x24
#define UART0_FBRD   0x28
#define UART0_LCRH   0x2C
#define UART0_CR     0x30
#define UART0_IMSC   0x38

void uart_init(void) {
    mmio_write(UART0_BASE + UART0_CR, 0);

    gpio_init(14, GPIO_ALT0);
    gpio_init(15, GPIO_ALT0);

    mmio_write(GPIO_BASE + GPPUD, 0);
    for (volatile int i = 0; i < 150; i++);
    mmio_write(GPIO_BASE + GPPUDCLK0, (1 << 14) | (1 << 15));
    for (volatile int i = 0; i < 150; i++);
    mmio_write(GPIO_BASE + GPPUDCLK0, 0);

    mmio_write(UART0_BASE + UART0_IBRD, 26);
    mmio_write(UART0_BASE + UART0_FBRD, 3);

    mmio_write(UART0_BASE + UART0_LCRH, (3 << 5) | (1 << 4));
    mmio_write(UART0_BASE + UART0_IMSC, 0);
    mmio_write(UART0_BASE + UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(char c) {
    while (mmio_read(UART0_BASE + UART0_FR) & (1 << 5));
    mmio_write(UART0_BASE + UART0_DR, (uint32_t)c);
}

void uart_puts(const char* s) {
    while (*s) uart_putc(*s++);
}
