#ifndef UART_DRIVER_H
#define UART_DRIVER_H

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char* s);

#endif
