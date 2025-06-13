/* secure-memory.ld: override Nonsecure-world memory for RIOT’s linker */

/* Non-Secure partitions: */
_rom_start_addr = 0x08040000;   /* start of Bank 2 = NS Flash */  
_rom_length     = 0x0003FFFF;   /* 256 KB NS Flash            */  
_ram_start_addr = 0x20018000;   /* start of NS SRam1 upper    */  
_ram_length     = 0x00028000;   /* 160 KB NS SRAM             */  

INCLUDE cortexm.ld

