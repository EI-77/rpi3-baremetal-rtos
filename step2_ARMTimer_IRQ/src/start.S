.section .text.boot
.global __start
.extern irq_handler

.section .text
.global __start

__start:
    mrs r0, cpsr
    and r0, r0, #0x1F
    cmp r0, #0x1A
    bne __real_start

    ldr r1, =__real_start 
    msr ELR_hyp, r1

    mrs r2, cpsr
    bic r2, r2, #0x1F
    orr r2, r2, #0x13 
    msr SPSR_hyp, r2

    eret    

__real_start:
    cpsid if

    ldr sp, =__stack_top

    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
1:
    cmp r0, r1
    it lt
    strlt r2, [r0], #4
    blt 1b

    ldr r0, =__rom_data
    ldr r1, =__data_start
    ldr r2, =__data_end
2:
    cmp r1, r2
    it lt
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt 2b

    mov r0, #0xD2             
    msr cpsr_c, r0
    ldr sp, =__stack_top_irq

    mov r0, #0xD3 
    msr cpsr_c, r0
    ldr sp, =__stack_top

    ldr r0, =__vector_table
    mcr p15, 0, r0, c12, c0, 0

    mrs r0, cpsr
    bic r0, r0, #0x80
    msr cpsr_c, r0

    bl kernel_main

3:  b 3b

.align 5
.global __vector_table
__vector_table:
    b __start // Reset
    b hang // Undefined
    b hang // SVC
    b hang // Prefetch Abort
    b hang // Data Abort
    b hang // Reserved
    b __irq_handler_entry // IRQ
    b hang // FIQ

hang:
    b hang

__irq_handler_entry:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    bl irq_handler
    ldmfd sp!, {r0-r12, lr}
    subs pc, lr, #0

