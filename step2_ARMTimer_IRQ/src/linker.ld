ENTRY(_start) 

SECTIONS
{
  . = 0x8000; 

  .text : {
    KEEP(*(.text.boot))  
    *(.text*) 
  }
  
  .rodata : {
    *(.rodata*)
  }

  __rom_data = .;

  .data : {
    __data_start = .;
    *(.data*)
    __data_end   = .;
  }

  .bss : {
    __bss_start = .;
    *(.bss*)
    *(COMMON)
    __bss_end   = .;
  }
  
  . = ALIGN(8);
  .stack_irq : {
    . = . + 4096;
  }
  __stack_top_irq = .;

  . = ALIGN(8);
  .stack : {
    . = . + 4096;
  }
  __stack_top = .;
}