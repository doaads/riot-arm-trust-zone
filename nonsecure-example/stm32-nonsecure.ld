MEMORY
{
    /* Secure flash region */
    secure_flash  : ORIGIN = 0x08000000, LENGTH = 0x20000

    /* Non-Secure flash region */
    nonsecure_flash : ORIGIN = 0x08020000, LENGTH = 0x1E0000

    /* Non-Secure SRAM region */
    nonsecure_sram : ORIGIN = 0x20020000, LENGTH = 0x20000
}

SECTIONS
{
    /* Non-secure vector table at the beginning of the non-secure flash region */
    .vector_table : {
        KEEP(*(.isr_vector))        /* Place interrupt vector table here */
        KEEP(*(.reset_vector))      /* Place reset vector here */
    } > nonsecure_flash

    /* The rest of the non-secure program follows after the vector table */
    .text : {
        *(.text)
        *(.text*)
    } > nonsecure_flash

    .data : {
        *(.data)
    } > nonsecure_sram

    /* Define the heap for non-secure memory */
    .heap ALIGN(4) (NOLOAD) :
    {
        _sheap = ORIGIN(nonsecure_sram);
        _eheap = ORIGIN(nonsecure_sram) + LENGTH(nonsecure_sram);
    } > nonsecure_sram
}

