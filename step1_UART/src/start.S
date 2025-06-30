.section .text.boot
.global _start

_start:
    ldr sp, =__stack_top

    // .BSS
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
1:
    cmp r0, r1
    it lt
    strlt r2, [r0], #4
    blt 1b

    // .data
    ldr r0, =__rom_data    
    ldr r1, =__data_start 
    ldr r2, =__data_end
2:
    cmp r1, r2
    it lt
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt 2b

    bl kernel_main

3:  b 3b  
