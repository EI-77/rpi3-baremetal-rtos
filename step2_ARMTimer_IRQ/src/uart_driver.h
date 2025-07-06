#ifndef UART_DRIVER_H
#define UART_DRIVER_H

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char* s);
void uart_put_hex(uint32_t value);

#endif
