#include <stdint.h>
#include "uart_driver.h"
#include "gpio_driver.h"
#include "irq.h"

void kernel_main(void) {
    uart_init();
    uart_puts("<CHECK> uart init finish\r\n");
    gpio_init(17, GPIO_OUTPUT);
    uart_puts("<CHECK> gpio17 init finish\r\n");
    timer_init();
    uart_puts("<CHECK> timer init finish\r\n");

    while(1) {
        uart_puts("loop\r\n");
        for (volatile int i = 0; i < 1000000; i++);
    }
}
