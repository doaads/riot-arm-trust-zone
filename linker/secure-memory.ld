/* secure-memory.ld: override Secure-world memory for RIOT’s linker */

/* Secure partitions */
_rom_start_addr = 0x08000000;      /* start of Secure Flash */
_rom_length     = 0x00030000;      /* Secure Flash          */
_ram_start_addr = 0x20000000;      /* start of Secure SRAM  */
_ram_length     = 0x00018000;      /* Secure SRAM           */

_nsc_start_addr = 0x08030000;      /* NSC                   */
_nsc_length     = 0x400;

INCLUDE cortexm_rom_offset.ld

MEMORY
{
    rom      (rx)   : ORIGIN = _rom_start_addr + _rom_offset,                  LENGTH = _fw_rom_length
    nsc      (rx)   : ORIGIN = _nsc_start_addr,                                LENGTH = _nsc_length
    ram      (w!rx) : ORIGIN = _ram_start_addr,                                LENGTH = _ram_length
}

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

/* This is only used by gdb to understand where to start */
ENTRY(reset_handler_default)

_backup_ram_start_addr = DEFINED( _backup_ram_start_addr ) ? _backup_ram_start_addr : 0x0 ;
_backup_ram_len = DEFINED( _backup_ram_len ) ? _backup_ram_len : 0x0 ;

/* not all Cortex-M platforms use cortexm.ld yet */
MEMORY
{
    bkup_ram (w!rx) : ORIGIN = _backup_ram_start_addr, LENGTH = _backup_ram_len
    rom_aux  (rx)   : ORIGIN = _rom_start_addr + _slot_aux_offset, LENGTH = _slot_aux_len
}

/* cortexm_base.ld - with added .gnu.sgstubs */

/* Section Definitions */
SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        _sfixed = .;
        _isr_vectors = DEFINED(_isr_vectors) ? _isr_vectors : . ;
        KEEP(*(SORT(.vectors*)))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        KEEP (*(SORT(.roxfa.*)))
        *(.ARM.extab* .gnu.linkonce.armextab.*)

        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(0x4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        _efixed = .;            /* End of text section */
    } > rom

    /*
     * TLS relocations are offsets relative to the address
     * of the first TLS symbol. That means we just need to
     * allocate them all together so that the TLS region
     * is compact when allocated for each thread.
     */

    /*
     * TLS initialization data is loaded into ROM so that
     * each thread can get its values initialized from there
     * at startup
     */
    .tdata :
    {
         __tdata_start = .;
        *(.tdata .tdata.* .gnu.linkonce.td.*)
        __tdata_end = .;
    } > rom
    __tdata_source = LOADADDR(.tdata);
    __tdata_size = SIZEOF(.tdata);

    /*
     * TLS zeroed data is relocated as if it immediately followed
     * the tdata values. However, the linker 'magically' erases the
     * memory allocation so that no ROM is consumed by this
     * section
     */
    .tbss :
    {
        *(.tbss .tbss.* .gnu.linkonce.tb.*)
        *(.tcommon)
        __tbss_end = .;
    } > rom
    __tls_size = __tbss_end - __tdata_start;
    __tbss_size = __tls_size - __tdata_size;
    PROVIDE( __tbss_offset = ADDR(.tbss) - ADDR(.tdata) );
    PROVIDE( __tls_align = MAX(ALIGNOF(.tdata), ALIGNOF(.tbss)) );
    PROVIDE( __arm32_tls_tcb_offset = MAX(8, __tls_align) );
    PROVIDE( __arm64_tls_tcb_offset = MAX(16, __tls_align) );

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    PROVIDE_HIDDEN (__exidx_end = .);

    /* exception handling */
    . = ALIGN(4);
    .eh_frame :
    {
        KEEP (*(.eh_frame))
    } > rom

    . = ALIGN(4);
    _etext = .;

    /* stack section */
    .stack (NOLOAD):
    {
        KEEP (*(.puf))
        . = ALIGN(8);
        _sstack = .;
        KEEP (*(.isr_stack))
        . = ALIGN(8);
        _estack = .;
    } > ram

    .relocate :
    {
        . = ALIGN(4);
        _srelocate = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        KEEP (*(SORT(.xfa.*)))
        KEEP (*(.openocd .openocd.*))
        . = ALIGN(4);
        _erelocate = .;
    } > ram AT> rom

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = . ;
        _szero = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = . ;
        _ezero = .;
    } > ram

    /*
     * collect all uninitialized sections that go into RAM
     */
    .noinit (NOLOAD) :
    {
        __noinit_start = .;
        *(.noinit)
        . = ALIGN(4);
        __noinit_end = .;
    }  > ram

    /* heap section */
    . = ALIGN(4);
    _sheap = . ;
    _eheap = ORIGIN(ram) + LENGTH(ram);

    /* Populate information about ram size */
    _sram = ORIGIN(ram);
    _eram = ORIGIN(ram) + LENGTH(ram);

    _sbackup_data_load = LOADADDR(.backup.data);
    .backup.data :  ALIGN(4) {
        _sbackup_data = .;
        *(.backup.data)
        _ebackup_data = .;
        /* Round size so that we can use 4 byte copy in init */
        . = ALIGN(4);
    } > bkup_ram AT> rom

    .backup.bss (NOLOAD) : ALIGN(4) {
        _sbackup_bss = .;
        *(.backup.bss)
        _ebackup_bss = .;
        /* Round size so that we can use 4 byte copy in init */
        . = ALIGN(4);
    } > bkup_ram

    .heap3 (NOLOAD) : ALIGN(4) {
        _sheap1 = . ;
        _eheap1 = ORIGIN(bkup_ram) + LENGTH(bkup_ram);
    } > bkup_ram

    .flash_writable (NOLOAD) : {
        KEEP(*(SORT(.flash_writable.*)))
    } > rom

    .gnu.sgstubs :
    {
        . = ALIGN(32);
        KEEP(*(.gnu.sgstubs*))
    } > nsc

}



