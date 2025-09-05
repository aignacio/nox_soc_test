/* Minimal RV32I linker script for single 10 KiB SRAM at 0x00000000 */
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
  SRAM (rxw) : ORIGIN = 0x00000000, LENGTH = 10K
}

_stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
PROVIDE(__stack = _stack_top);

SECTIONS
{
  .text : {
    KEEP(*(.text.entry))
    KEEP(*(.init))
    *(.text .text.*)
    *(.rodata .rodata.*)
  } > SRAM

  . = ALIGN(4);
  /* Provide a GP near the middle of small data window to keep la/ld offsets small */
  PROVIDE(__global_pointer$ = . + 0x800);

  .data : {
    *(.data .data.*)
  } > SRAM

  .sdata : {
    *(.sdata .sdata.*)
  } > SRAM

  .bss (NOLOAD) : {
    __bss_start = .;
    *(.sbss .sbss.*)
    *(.bss .bss.*)
    *(COMMON)
    __bss_end = .;
  } > SRAM

  /DISCARD/ : { *(.comment) *(.note*) }
}

