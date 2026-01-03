#include "periph/uart.h"
#include "stdio_base.h"

#ifndef TZ_CONFIG_H
#define TZ_CONFIG_H

/* Non-Secure Flash: Flash Bank 2 (256 KB) */
#ifndef NON_SECURE_FLASH_ADDR
#  define NON_SECURE_FLASH_ADDR  0x08040000   /* start of Bank 2 */ 
#endif
#ifndef NON_SECURE_FLASH_END
#  define NON_SECURE_FLASH_END   0x0807FFFF   /* end of Bank 2 */ 
#endif

/* Non-Secure SRAM: upper half of SRAM1+SRAM2 */
#ifndef NON_SECURE_SRAM_ADDR
#  define NON_SECURE_SRAM_ADDR   0x20018000   /* start of NS-SRAM */ 
#endif
#ifndef NON_SECURE_SRAM_END
#  define NON_SECURE_SRAM_END    0x2003FFFF   /* end of NS-SRAM */ 
#endif

/* Non-Secure-Callable region: 8 KB at 0x08030000–0x080301FF */
#ifndef NSC_ADDR
#    define NSC_ADDR               0x08030000
#endif
#ifndef NSC_END
#define NSC_END  (NSC_ADDR + 0x20 - 1)
#endif



void TZ_init(void);

void jump_to_nonsecure(void);

void secure_periph_init(void);

void uart_write_secure(uart_t uart, const uint8_t *data, size_t len);
ssize_t stdio_read_secure(void* buffer, size_t len);

#endif
