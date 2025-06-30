#include <stdint.h>
#include "uart_driver.h"
#include "gpio_driver.h"

// Address of ARM Timer registers (for Raspberry Pi 3)
#define ARM_TIMER_BASE     0x3F00B400
#define ARM_TIMER_LOAD     (ARM_TIMER_BASE + 0x00)  // Timer Load register
#define ARM_TIMER_VALUE    (ARM_TIMER_BASE + 0x04)  // Current timer value
#define ARM_TIMER_CONTROL  (ARM_TIMER_BASE + 0x08)  // Timer control register
#define ARM_TIMER_IRQ_CLR  (ARM_TIMER_BASE + 0x0C)  // Interrupt clear register
#define ARM_TIMER_RELOAD   (ARM_TIMER_BASE + 0x18)  // Reload value
#define ARM_TIMER_DIVIDER  (ARM_TIMER_BASE + 0x1C)  // Timer prescaler

// Interrupt controller registers
#define IRQ_ENABLE_BASIC   0x3F00B218  // Basic IRQ enable register
#define IRQ_ENABLE_IRQ1    0x3F00B210  // IRQ1 enable register
#define IRQ_ARM_TIMER      (1 << 0)    // Bit to enable ARM Timer IRQ in basic IRQs

// Initializes the ARM Timer to generate periodic interrupts (~0.5 sec)
void timer_init(void) {
    // Load and reload values (500,000 ticks at 1MHz = 0.5 sec)
    mmio_write(ARM_TIMER_LOAD, 500000);
    mmio_write(ARM_TIMER_RELOAD, 500000);

    // Set timer prescaler: 250MHz / (249 + 1) = 1MHz
    mmio_write(ARM_TIMER_DIVIDER, 249);

    // Clear any pending timer interrupts
    mmio_write(ARM_TIMER_IRQ_CLR, 0);

    // Enable the timer:
    // Bit 7 = Timer enable
    // Bit 5 = Interrupt enable
    // Bit 1 = Use 23-bit counter
    mmio_write(ARM_TIMER_CONTROL,
        (1 << 7) |  // Timer Enable
        (1 << 5) |  // Interrupt Enable
        (1 << 1)    // 23-bit Counter
    );

    // Enable ARM Timer interrupt in the interrupt controller
    mmio_write(IRQ_ENABLE_BASIC, IRQ_ARM_TIMER);      // Basic IRQ line
    mmio_write(IRQ_ENABLE_IRQ1, (1 << 0));            // IRQ1 line
}

// Interrupt handler called on ARM Timer interrupt
void irq_handler(void) {
    // Clear the timer interrupt flag
    mmio_write(ARM_TIMER_IRQ_CLR, 0);

    // Send debug message and toggle GPIO17 (e.g., LED)
    uart_puts("Interrupt\r\n");
    gpio_toggle(17);
}
