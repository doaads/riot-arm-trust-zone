/* secure-mem.ld: override Secure-world memory for RIOT’s linker */

/* 1) Core Secure partitions */
_rom_start_addr = 0x08000000;      /* start of Secure Flash    */  
_rom_length     = 0x00030000;      /* 192 KB Secure Flash      */  
_ram_start_addr = 0x20000000;      /* start of Secure SRAM     */  
_ram_length     = 0x00018000;      /* 96 KB Secure SRAM        */  

/* 2) Include RIOT’s standard Cortex-M sections */
INCLUDE cortexm.ld            /* all .text/.data/.bss/.stack/.relocate */  

/* Place non-secure-callable (NSC) functions at the end of Secure Flash */
//.nsc_region :
//{
//    KEEP(*(.non_secure_entry))
//    KEEP(*(.cmse_nonsecure_call))
//    KEEP(*(.cmse_nonsecure_entry))
//} > FLASH
//
