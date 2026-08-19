/**
 * @{
 *
 * @file       tz_config.c
 * @brief      TrustZone SAU memory partitioning and non-secure jump implementation
 *
 * @}
 */

#include "periph/uart.h"
#include "arm_cmse.h"
#include "core_cm33.h"
#include "include/tz_config.h"

/* configure secure peripherials such as GTZC and FPU */
void secure_periph_init(void) {

    /* Give NS access to FPU */
    SCB->NSACR |= SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk;

    for (uint8_t i = 0; i < 8; ++i) {
        GTZC_MPCBB2->VCTR[i] = 0;
    }

    __DSB();
    __ISB();
}

/* map SAU regions */
void TZ_init(void) {
    SAU->CTRL = 0;  /* Disable SAU before configuring */

    /* Allow Non-Secure Flash execution (0x08040000 - 0x0807FFFF) */
    SAU->RNR  = 0;
    SAU->RBAR = (NON_SECURE_FLASH_ADDR & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NON_SECURE_FLASH_END & SAU_RLAR_LADDR_Msk)
        | SAU_RLAR_ENABLE_Msk;

    /* Allow Non-Secure SRAM access to SRAM2 (0x20030000 - 0x2003FFFF) */
    SAU->RNR = 1;
    SAU->RBAR = (NON_SECURE_SRAM_ADDR & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NON_SECURE_SRAM_END & SAU_RLAR_LADDR_Msk) 
        | SAU_RLAR_ENABLE_Msk;

    /* Allow Non-Secure Callable (NSC) region (0x08030000 - 0x08030400) */
    SAU->RNR = 2;
    SAU->RBAR = (NSC_ADDR & SAU_RBAR_BADDR_Msk); 
    SAU->RLAR = (NSC_END & SAU_RLAR_LADDR_Msk)
        | SAU_RLAR_ENABLE_Msk | SAU_RLAR_NSC_Msk;

    /* Enable SAU */
    SAU->CTRL |= SAU_CTRL_ENABLE_Msk;

    __DSB();
    __ISB();
}

typedef void (*funcptr_ns)(void) __attribute__((cmse_nonsecure_call));

/* prepare the NS context for first entry */
void jump_to_nonsecure(void) {
    /* get the address of NS vector table */
    uint32_t *ns_vector_table = (uint32_t*)NON_SECURE_FLASH_ADDR;

    /* set up GTZC and FPU */
    secure_periph_init();

    /* map memory regions */
    TZ_init();

    uint32_t msp_ns_value = ns_vector_table[0];
    uint32_t reset_ns = ns_vector_table[1];

    __TZ_set_MSP_NS(msp_ns_value);

    SCB_NS->VTOR = (uint32_t)ns_vector_table;

    __DSB();
    __ISB();

    /* create an entry point */
    funcptr_ns ns_entry = (funcptr_ns) cmse_nsfptr_create((void*)reset_ns);

    /* call */
    ns_entry();

    /* never return */
    while(1){}
}
