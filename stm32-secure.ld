/* ARM TrustZone Bootloader Loader Script for Secure and Non-Secure Regions */

ccmram_length = DEFINED( _ccmram_len ) ? _ccmram_len : 0x0 ;
sram4_addr = DEFINED( _sram4_length ) ? 0x28000000 : 0x0 ;
sram4_length = DEFINED( _sram4_length ) ? _sram4_length : 0x0 ;
fmc_ram_addr = DEFINED( _fmc_ram_addr ) ? _fmc_ram_addr : 0x0 ;
fmc_ram_len = DEFINED( _fmc_ram_len ) ? _fmc_ram_len : 0x0 ;

secure_flash_addr = 0x08000000;  /* Start of Secure Flash */
secure_flash_len = 0x20000;     /* Define the secure flash region length */
nonsecure_flash_addr = 0x08020000; /* Start of Non-Secure Flash */
nonsecure_flash_len = 0x1E0000;  /* Define the non-secure flash region length */

secure_sram_addr = 0x20000000;   /* Start of Secure SRAM */
secure_sram_len = 0x40000;       /* Define the secure SRAM region length */
nonsecure_sram_addr = 0x20040000; /* Start of Non-Secure SRAM */
nonsecure_sram_len = 0x20000;     /* Define the non-secure SRAM region length */

MEMORY
{
    secure_flash   : ORIGIN = secure_flash_addr, LENGTH = secure_flash_len
    secure_sram    : ORIGIN = secure_sram_addr, LENGTH = secure_sram_len

    nonsecure_flash : ORIGIN = nonsecure_flash_addr, LENGTH = nonsecure_flash_len
    nonsecure_sram : ORIGIN = nonsecure_sram_addr, LENGTH = nonsecure_sram_len

    ccmram         : ORIGIN = 0x10000000, LENGTH = ccmram_length
    sram4          : ORIGIN = sram4_addr, LENGTH = sram4_length
    fmcram         : ORIGIN = fmc_ram_addr, LENGTH = fmc_ram_len
}

SECTIONS
{
    .secure_code ALIGN(4) :
    {
        _secure_code_start = .;
        *(.secure_code)
        _secure_code_end = .;
    } > secure_flash AT> secure_flash

    .secure_data ALIGN(4) :
    {
        _secure_data_start = .;
        *(.secure_data)
        _secure_data_end = .;
    } > secure_sram

    .nonsecure_code ALIGN(4) :
    {
        _nonsecure_code_start = .;
        *(.nonsecure_code)
        _nonsecure_code_end = .;
    } > nonsecure_flash AT> nonsecure_flash

    .nonsecure_data ALIGN(4) :
    {
        _nonsecure_data_start = .;
        *(.nonsecure_data)
        _nonsecure_data_end = .;
    } > nonsecure_sram

    .heap2 ALIGN(4) (NOLOAD) :
    {
        _sheap2 = ORIGIN(secure_sram);
        _eheap2 = ORIGIN(secure_sram) + LENGTH(secure_sram);
    } > secure_sram

    .heap4 ALIGN(4) (NOLOAD) :
    {
        _sheap3 = ORIGIN(nonsecure_sram);
        _eheap3 = ORIGIN(nonsecure_sram) + LENGTH(nonsecure_sram);
    } > nonsecure_sram

    .stack2 ALIGN(4) :
    {
        _sstack2 = ORIGIN(secure_sram) + LENGTH(secure_sram) - 0x1000; /* Adjust stack size */
        _estack2 = _sstack2 - 0x1000;  /* Adjust stack size */
    } > secure_sram

    .stack4 ALIGN(4) :
    {
        _sstack4 = ORIGIN(nonsecure_sram) + LENGTH(nonsecure_sram) - 0x1000; /* Adjust stack size */
        _estack4 = _sstack4 - 0x1000;  /* Adjust stack size */
    } > nonsecure_sram
}

INCLUDE cortexm.ld
