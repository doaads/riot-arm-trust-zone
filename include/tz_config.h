#ifndef TZ_CONFIG_H
#define TZ_CONFIG_H

/* Define default non-secure memory space */
#ifndef NON_SECURE_FLASH_ADDR
    #define NON_SECURE_FLASH_ADDR 0x08020000
#endif
#ifndef NON_SECURE_FLASH_END
    #define NON_SECURE_FLASH_END 0x080FFFFF
#endif
#ifndef NON_SECURE_SRAM_ADDR
    #define NON_SECURE_SRAM_ADDR 0x20000000
#endif
#ifndef NON_SECURE_SRAM_END
    #define NON_SECURE_SRAM_END 0x2FFFFFFF
#endif
#ifndef NSC_ADDR
    #define NSC_ADDR 0x20000000
#endif
#ifndef NSC_END
    #define NSC_END 0x2FFFFFFF
#endif


void TZ_init(void);

void jump_to_nonsecure(void);

void secure_fault_handler(void);
void hard_fault_handler(void);

#endif
