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

/* Non-Secure-Callable region: 8 KB at 0x0C03E000–0x0C03FFFF */
#ifndef NSC_ADDR
//#  define NSC_ADDR               0x08000000   /* start of NSC window */ 
#    define NSC_ADDR               0x0803E000
#endif
#ifndef NSC_END
//#  define NSC_END                0x0C03FFFF   /* end of NSC window */ 
#    define NSC_END                0x0803FFFF
#endif



void TZ_init(void);

void jump_to_nonsecure(void);

void secure_fault_handler(void);
void hard_fault_handler(void);

#endif
