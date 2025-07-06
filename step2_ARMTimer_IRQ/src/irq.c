#include <stdint.h>
#include "uart_driver.h"
#include "gpio_driver.h"

#define ARM_TIMER_BASE     0x3F00B000
#define ARM_TIMER_LOAD     (ARM_TIMER_BASE + 0x400)
#define ARM_TIMER_VALUE    (ARM_TIMER_BASE + 0x404)
#define ARM_TIMER_CONTROL  (ARM_TIMER_BASE + 0x408)
#define ARM_TIMER_IRQ_CLR  (ARM_TIMER_BASE + 0x40C)
#define ARM_TIMER_RELOAD   (ARM_TIMER_BASE + 0x418)
#define ARM_TIMER_DIVIDER  (ARM_TIMER_BASE + 0x41C)

#define IRQ_PENDING_1      (ARM_TIMER_BASE + 0x204)
#define IRQ_DISABLE_IRQ1   (ARM_TIMER_BASE + 0x21C)
#define IRQ_ENABLE_IRQ1    (ARM_TIMER_BASE + 0x210)
#define IRQ_ENABLE_BASIC   (ARM_TIMER_BASE + 0x218)
#define IRQ_DISABLE_BASIC  (ARM_TIMER_BASE + 0x224)
#define IRQ_ARM_TIMER      (1 << 0)

#define COUNT_PER_SEC 1000000

void disable_other_irqs(void) {
    mmio_write(IRQ_DISABLE_BASIC, 0xffffffff);  
    mmio_write(IRQ_DISABLE_IRQ1,  0xffffffff); 
}

void timer_init(void) {
    disable_other_irqs();

    mmio_write(ARM_TIMER_LOAD, COUNT_PER_SEC);
    mmio_write(ARM_TIMER_RELOAD, COUNT_PER_SEC);
    mmio_write(ARM_TIMER_DIVIDER, 249);     
    mmio_write(ARM_TIMER_IRQ_CLR, 1);       

    mmio_write(ARM_TIMER_CONTROL,
        (1 << 7) |  // Timer Enable
        (1 << 5) |  // Interrupt Enable
        (1 << 1)    // 23-bit Counter
    );

    mmio_write(IRQ_ENABLE_BASIC, IRQ_ARM_TIMER);
    mmio_write(IRQ_ENABLE_IRQ1, (1 << 0));

}

void irq_handler(void) {

    uint32_t irq_pending = mmio_read(IRQ_PENDING_1);
    if ((irq_pending & (1 << 0)) == 0) return;

    mmio_write(ARM_TIMER_IRQ_CLR, 1);
    mmio_write(ARM_TIMER_LOAD, COUNT_PER_SEC); 

    uart_puts("Interrupt\r\n");
    gpio_toggle(17);
}
