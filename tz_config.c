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
    SCB->VTOR = (NON_SECURE_FLASH_ADDR);
    SAU->CTRL = (0);  /* Disable SAU before configuring */

    /* Allow Non-Secure Flash execution (0x08020000 - 0x080FFFFF) */
    SAU->RNR  = (0);
    SAU->RBAR = (NON_SECURE_FLASH_ADDR & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NON_SECURE_FLASH_END & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

    /* Allow Non-Secure SRAM access (0x20000000 - 0x2FFFFFFF) */
    SAU->RNR++;
    SAU->RBAR = (NON_SECURE_SRAM_ADDR & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NON_SECURE_SRAM_END & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

    /* Allow Non-Secure Callable (NSC) region (0x0C000000 - 0x0FFFFFFF) */
    SAU->RNR++;
    SAU->RBAR = (NSC_ADDR & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (NSC_END & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk | SAU_RLAR_NSC_Msk;

    /* Enable SAU */
    /* Set ENABLE bit[0] to 1 */
    SAU->CTRL |= SAU_CTRL_ENABLE_Msk;
    /* SAU->CTRL |= SAU_CTRL_ALLNS_Msk; */

    /* Data Synchronization Barrier */
    __DSB();

    /* Information Synchronization Barrier */
    __ISB();
}

void jump_to_nonsecure(void) {
    TZ_init();

    __TZ_set_MSP_NS(*((volatile uint32_t *)(SCB->VTOR)));

    uint32_t ns_reset_vector = *((volatile uint32_t *)(SCB->VTOR + 4U));

    // uint32_t control = __get_CONTROL();
    // control &= ~CONTROL_nPRIV_Msk;
    // __set_CONTROL(control);

    __ISB();

    /* Branch with Exchange */
    __asm__ volatile("BX %0" ::"r"(ns_reset_vector) : "memory");
}

