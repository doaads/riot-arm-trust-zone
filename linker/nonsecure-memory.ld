/* nonsecure-memory.ld: override Nonsecure-world memory for RIOT’s linker */

/* define memory regions for NS memory that will override RIOT's defaults

/* Non-Secure partitions: */
_rom_start_addr = 0x08040000;   /* start of Bank 2 = NS Flash */
_rom_length     = 0x0003FFFF;   /* NS Flash                   */
_ram_start_addr = 0x20018000;   /* start of NS SRAM           */
_ram_length     = 0x00027FFF;   /* NS SRAM                    */

_estack = _ram_start_addr + 0x400;

INCLUDE cortexm.ld

/*PROVIDE(uart_write_secure = 0x08030001);
PROVIDE(stdio_read_secure = 0x08030009);*/

/*EXTERN(uart_write_secure);
PROVIDE(uart_write_secure = uart_write_secure);

EXTERN(stdio_read_secure);
PROVIDE(stdio_read_secure = stdio_read_secure);*/


