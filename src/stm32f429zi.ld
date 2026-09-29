/* STM32F429ZI: 2 MB flash, 192 KB contiguous SRAM (SRAM1+SRAM2+SRAM3). */
ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 192K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);
_min_stack_size = 0x800;   /* Main stack: used before the scheduler and by ISRs */

SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    .text :
    {
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    ASSERT(_ebss + _min_stack_size <= _estack, "Not enough RAM for the main stack")
}
