#include <stdint.h>
#include "uart_driver.h"
#include "gpio_driver.h"
#include "irq.h"

void kernel_main(void) {
    uart_init();
    uart_puts("UART initialized successfully.\r\n");
    gpio_init(17, GPIO_OUTPUT);
    uart_puts("GPIO17 initialized successfully.\r\n");
    timer_init();
    uart_puts("Timer initialized successfully.\r\n");

    while(1) {
        uart_puts("loop\r\n");
        for (volatile int i = 0; i < 1000000; i++);
    }
}