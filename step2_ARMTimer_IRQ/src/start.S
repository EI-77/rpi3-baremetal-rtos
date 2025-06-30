.section .text.boot
.global _start
.extern irq_handler

_start:
    // Initialize stack pointer for Supervisor mode
    ldr sp, =__stack_top

    // Zero-initialize the .bss section
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
1:
    cmp r0, r1
    it lt
    strlt r2, [r0], #4
    blt 1b

    // Copy .data section from ROM to RAM
    ldr r0, =__rom_data
    ldr r1, =__data_start
    ldr r2, =__data_end
2:
    cmp r1, r2
    it lt
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt 2b

    // Switch to IRQ mode and set IRQ stack
    mov r0, #0xD2              // CPSR: IRQ mode, FIQ/IRQ disabled
    msr cpsr_c, r0
    ldr sp, =__stack_top_irq

    // Switch back to Supervisor (SVC) mode and reset SP
    mov r0, #0xD3              // CPSR: SVC mode, FIQ/IRQ disabled
    msr cpsr_c, r0
    ldr sp, =__stack_top

    // Set vector base address to our custom vector table
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0

    // Enable IRQ (clear I bit in CPSR)
    mrs r0, cpsr
    bic r0, r0, #0x80          // Clear bit 7 (I bit)
    msr cpsr_c, r0

    // Jump to C kernel entry point
    bl kernel_main

3:  b 3b                      // Halt if returned unexpectedly

// ----------------------------------------------------------------------
// Exception Vector Table (must be 32-byte aligned)
// ----------------------------------------------------------------------
.align 7
.global vector_table
vector_table:
    ldr pc, =_start              // Reset
    ldr pc, =hang                // Undefined Instruction
    ldr pc, =hang                // Software Interrupt (SWI/SVC)
    ldr pc, =hang                // Prefetch Abort
    ldr pc, =hang                // Data Abort
    ldr pc, =hang                // Reserved
    ldr pc, =_irq_handler_entry  // IRQ
    ldr pc, =hang                // FIQ

// Fallback infinite loop (used for unhandled exceptions)
hang:
    b hang

// ----------------------------------------------------------------------
// IRQ handler wrapper (saves registers and jumps to C irq_handler)
// ----------------------------------------------------------------------
_irq_handler_entry:
    sub lr, lr, #4              // Adjust LR to point to the interrupted instruction
    stmfd sp!, {r0-r12, lr}     // Push all general-purpose registers + LR onto IRQ stack
    bl irq_handler              // Call C interrupt handler
    ldmfd sp!, {r0-r12, lr}     // Restore registers
    subs pc, lr, #0             // Return from IRQ (restores CPSR from SPSR)
