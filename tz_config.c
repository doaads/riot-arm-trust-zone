/**
 * @{
 *
 * @file
 * @brief      TrustZone SAU memory partitioning and non-secure jump implementation
 *
 * @}
 */

#include "stm32l5xx.h"
#include "arm_cmse.h"
#include "core_cm33.h"
#include "include/tz_config.h"

void TZ_init(void) {

    RCC->AHB1ENR |= RCC_AHB1ENR_GTZCEN;

    for (int i = 0; i < 256; i++) {
        GTZC_MPCBB1->VCTR[i] = 0x00000000;
    }

    SAU->CTRL = 0;  /* Disable SAU before configuring */

    /* Allow Non-Secure Flash execution (0x08040000 - 0x0807FFFF) */
    SAU->RNR  = 0;
    SAU->RBAR = (NON_SECURE_FLASH_ADDR & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NON_SECURE_FLASH_END & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

    /* Allow Non-Secure SRAM access (0x20018000 - 0x2003FFFF) */
    SAU->RNR = 1;
    SAU->RBAR = (NON_SECURE_SRAM_ADDR & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NON_SECURE_SRAM_END & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

    /* Allow Non-Secure Callable (NSC) region (0x0C000000 - 0x0FFFFFFF) */
    SAU->RNR = 2;
    SAU->RBAR = (NSC_ADDR & SAU_RBAR_BADDR_Msk); 
    SAU->RLAR = (NSC_END & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk | SAU_RLAR_NSC_Msk;

    /* Enable SAU */
    SAU->CTRL |= SAU_CTRL_ENABLE_Msk;
    /*SAU->CTRL |= SAU_CTRL_ALLNS_Msk;*/

    __DSB();
    __ISB();
}

typedef void (*funcptr_ns)(void) __attribute__((cmse_nonsecure_call));

void jump_to_nonsecure(void) {
    uint32_t *ns_vector_table = (uint32_t*)NON_SECURE_FLASH_ADDR;

    /* This needs to be fixed - we should be getting these from ns_vector_table */
    //uint32_t msp_ns_value = ns_vector_table[0];
    uint32_t msp_ns_value = 0x20018200;
    //uint32_t reset_ns = ns_vector_table[1];
    uint32_t reset_ns = 0x804061d;

    /* map memory regions */
    TZ_init();

    /* set the stack pointer */
    __TZ_set_MSP_NS(msp_ns_value);

    /* set VTOR */
    SCB_NS->VTOR = (uint32_t)ns_vector_table;

    /* create an entry point */
    funcptr_ns ns_entry = (funcptr_ns) cmse_nsfptr_create((void*)reset_ns);

    /* call */
    ns_entry();

}
