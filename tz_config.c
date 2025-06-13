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
    uint32_t msp_ns_value = ns_vector_table[0];
    uint32_t reset_ns = ns_vector_table[1];
    TZ_init();

    __TZ_set_MSP_NS(msp_ns_value);

    //SCB->VTOR = (uint32_t)ns_vector_table;
    //__TZ_set_CONTROL_NS(0);
    SCB_NS->VTOR = (uint32_t)ns_vector_table;

    funcptr_ns ns_entry = (funcptr_ns) cmse_nsfptr_create((void*)reset_ns);
    ns_entry();

}
