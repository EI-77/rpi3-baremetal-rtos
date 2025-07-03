#include <stdint.h>
#include "uart_driver.h"
#include "gpio_driver.h"
#include "irq.h"

/* debug func */
void print_hex(uint32_t val) {
    char hex_chars[] = "0123456789ABCDEF";
    for (int i = 28; i >= 0; i -= 4) {
        uart_putc(hex_chars[(val >> i) & 0xF]);
    }
    uart_putc('\r');
    uart_putc('\n');
}

void check_cpsr(void) {
    uint32_t cpsr;
    __asm__ volatile("mrs %0, cpsr" : "=r"(cpsr));
    uart_puts("CPSR = 0x");
    print_hex(cpsr);
}


void kernel_main(void) {
    uart_init();
    uart_puts("<CHECK> uart init finish\r\n");
    gpio_init(17, GPIO_OUTPUT);
    gpio_set(17);
    uart_puts("<CHECK> gpio17 init finish\r\n");

    check_cpsr();

    timer_init();
    uart_puts("<CHECK> timer init finish\r\n");

    while(1) {
        uart_puts("loop\r\n");
        for (volatile int i = 0; i < 1000000; i++);
    }
}
