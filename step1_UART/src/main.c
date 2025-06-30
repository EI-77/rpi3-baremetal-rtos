#include <stdint.h>
#include "uart_driver.h"
#include "gpio_driver.h"

void kernel_main(void) {
    uart_init();
    while(1) {
        uart_puts("Hello world\r\n");
        for (volatile int i = 0; i < 1000000; i++);
    }
}
