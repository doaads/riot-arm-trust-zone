/* secure-memory.ld: override Secure-world memory for RIOT’s linker */

/* Secure partitions */
_rom_start_addr = 0x08000000;      /* start of Secure Flash    */  
_rom_length     = 0x00030000;      /* 192 KB Secure Flash      */  
_ram_start_addr = 0x20000000;      /* start of Secure SRAM     */  
_ram_length     = 0x00018000;      /* 96 KB Secure SRAM        */  

INCLUDE cortexm.ld
