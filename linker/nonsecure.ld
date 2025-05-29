/* Top of both scripts: Common defines */
_ROM_START       = 0x08000000;           /* start of on-chip Flash */
_SRAM_START      = 0x20000000;           /* start of on-chip SRAM */

_SEC_FLASH_SZ    = 0x00030000;           /* 192 KB Secure Flash */
_SEC_RAM_SZ      = 0x00018000;           /* 96 KB Secure SRAM */

_NSC_OFFSET      = _SEC_FLASH_SZ - 0x00004000; /* 16 KB before end for NSC */
_NSC_SIZE        = 0x00004000;           /* 16 KB NS-Callable */

_NS_FLASH_ORG    = _ROM_START + _SEC_FLASH_SZ;
_NS_RAM_ORG      = _SRAM_START + _SEC_RAM_SZ;

/* Non-Secure world linker script */
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(NonSecure_ResetHandler)

MEMORY
{
  FLASH_NS   (rx) : ORIGIN = _NS_FLASH_ORG,                   LENGTH = ( _rom_length - _SEC_FLASH_SZ )
  SRAM_NS    (rwx): ORIGIN = _NS_RAM_ORG,                     LENGTH = ( _ram_length - _SEC_RAM_SZ )
}

/* Sections placed in Non-Secure Flash/SRAM */
SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    KEEP(*(SORT(.nvectors*)))          /* NS vector table */
    *(.text .text.*)
    *(.rodata .rodata*)
  } > FLASH_NS

  .data : AT> FLASH_NS
  {
    . = ALIGN(4);
    *(.data .data.*)
  } > SRAM_NS

  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    *(.bss .bss.*)
    *(COMMON)
  } > SRAM_NS

  /* Non-Secure heap & stack */
  .nsheap (NOLOAD) :
  {
    _s_nsheap = .;
    . = ORIGIN(SRAM_NS) + LENGTH(SRAM_NS);
    _e_nsheap = .;
  } > SRAM_NS

  .nsstack (NOLOAD) :
  {
    . = ALIGN(8);
    _s_nsstack = .;
    . = ALIGN(8);
    _e_nsstack = .;
  } > SRAM_NS
}

