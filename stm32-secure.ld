MEMORY
{
    /* Secure flash region */
    secure_flash  : ORIGIN = 0x08000000, LENGTH = 0x20000

    /* Non-Secure flash region */
    nonsecure_flash : ORIGIN = 0x08020000, LENGTH = 0x1E0000

    /* Non-Secure SRAM region */
    nonsecure_sram : ORIGIN = 0x20000000, LENGTH = 0x20000

    /*ccmram  : ORIGIN = 0x10000000, LENGTH = ccmram_length*/
    /*sram4   : ORIGIN = sram4_addr, LENGTH = sram4_length*/
    /*fmcram  : ORIGIN = fmc_ram_addr, LENGTH = fmc_ram_len*/
}

SECTIONS
{
    /* Ensure non-secure application is placed in non-secure flash region */
    .text : 
    {
        *(.text) 
        *(.text*) 
    } > nonsecure_flash

    .data : 
    {
        *(.data) 
    } > nonsecure_sram

    /* Define the heap for non-secure memory */
    .heap ALIGN(4) (NOLOAD) :
    {
        _sheap = ORIGIN(nonsecure_sram);
        _eheap = ORIGIN(nonsecure_sram) + LENGTH(nonsecure_sram);
    } > nonsecure_sram
}

