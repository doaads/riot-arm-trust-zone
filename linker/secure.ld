/* Top of both scripts: Common defines */
_ROM_START       = 0x08000000;           /* start of on-chip Flash */
_SRAM_START      = 0x20000000;           /* start of on-chip SRAM */

_SEC_FLASH_SZ    = 0x00030000;           /* 192 KB Secure Flash */
_SEC_RAM_SZ      = 0x00018000;           /* 96 KB Secure SRAM */

_NSC_OFFSET      = _SEC_FLASH_SZ - 0x00004000; /* 16 KB before end for NSC */
_NSC_SIZE        = 0x00004000;           /* 16 KB NS-Callable */

_NS_FLASH_ORG    = _ROM_START + _SEC_FLASH_SZ;
_NS_RAM_ORG      = _SRAM_START + _SEC_RAM_SZ;

/* Secure world linker script */
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(reset_handler)

MEMORY
{
  FLASH_SEC  (rx) : ORIGIN = _ROM_START,                      LENGTH = _SEC_FLASH_SZ
  FLASH_NSC  (rx) : ORIGIN = _ROM_START + _NSC_OFFSET,        LENGTH = _NSC_SIZE
  SRAM_SEC   (rwx): ORIGIN = _SRAM_START,                     LENGTH = _SEC_RAM_SZ
}

/* Sections placed in Secure Flash/SRAM */
SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    KEEP(*(SORT(.vectors*)))          /* Secure vector table */
    *(.text .text.*)
    *(.rodata .rodata*)
    . = ALIGN(4);
    KEEP(*(.init .init.*))
    KEEP(*(.fini .fini.*))
  } > FLASH_SEC

  /* Non-Secure Callable wrappers (cmse_nonsecure_entry) */
  .nsc : ALIGN(4)
  {
    KEEP(*(SORT(.cmse_nonsecure_entry*)))  /* SG instructions */
  } > FLASH_NSC

  .data :
  {
    . = ALIGN(4);
    *(.data .data.*)
  } > SRAM_SEC AT> FLASH_SEC

  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    *(.bss .bss.*)
    *(COMMON)
  } > SRAM_SEC

  /* Secure heap & stack */
  .sheap (NOLOAD) :
  {
    _s_sheap = .;
    . = ORIGIN(SRAM_SEC) + LENGTH(SRAM_SEC);
    _e_sheap = .;
  } > SRAM_SEC

  .sstack (NOLOAD) :
  {
    . = ALIGN(8);
    _s_sstack = .;
    . = ALIGN(8);
    _e_sstack = .;
  } > SRAM_SEC
}
