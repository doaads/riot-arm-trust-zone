/**
 * @{
 *
 * @file       tz_config.c
 * @brief      TrustZone SAU memory partitioning and non-secure jump implementation
 *
 * @}
 */

#include "periph/uart.h"
#include "stm32l5xx.h"
#include "arm_cmse.h"
#include "core_cm33.h"
#include <stdio.h>
#include "include/tz_config.h"

/*void __attribute__((cmse_nonsecure_entry)) uart_write_secure(uart_t uart, const uint8_t *data, size_t len) {
    uart_write(uart, data, len);
}*/

//__attribute__((cmse_nonsecure_entry, section(".gnu.sgstubs"), aligned(32)))
//void uart_write_secure(void) {
//    __ASM volatile ("nop");
//}

__attribute__((cmse_nonsecure_entry))
void uart_write_secure(void) {
    __ASM volatile ("nop");
}

void secure_periph_init(void) {
    /* Enable GTZCEN */
    RCC->AHB1ENR |= RCC_AHB1ENR_GTZCEN;

    /* mark SRAM accessible */
    for (int i = 0; i < 10; i++) {
        GTZC_MPCBB1->VCTR[i] = 0xFFFFFFFF;
    }

    for (int i = 10; i < 24; i++) {
        GTZC_MPCBB1->VCTR[i] = 0x00000000;
    }

    for (int i = 0; i < 12; i++) {
        GTZC_MPCBB2->VCTR[i] = 0xFFFFFFFF;
    }

    __DSB();
    __ISB();
}

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

    /* Allow Non-Secure Callable (NSC) region (0x08030000 - 0x08030400) */
    SAU->RNR = 2;
    SAU->RBAR = (NSC_ADDR & SAU_RBAR_BADDR_Msk); 
    //SAU->RLAR = (NSC_END & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk | SAU_RLAR_NSC_Msk;
    SAU->RLAR = ((NSC_ADDR + 0x400 - 1) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk | SAU_RLAR_NSC_Msk;

    /* Enable SAU */
    SAU->CTRL |= SAU_CTRL_ENABLE_Msk;

    __DSB();
    __ISB();
}

typedef void (*funcptr_ns)(void) __attribute__((cmse_nonsecure_call));

void jump_to_nonsecure(void) {
    /* get the address of NS vector table */
    uint32_t *ns_vector_table = (uint32_t*)NON_SECURE_FLASH_ADDR;

    /* mark certain peripherials as NS accessible */
    secure_periph_init();

    /* map memory regions */
    TZ_init();

    uint32_t msp_ns_value = ns_vector_table[0];
    uint32_t reset_ns = ns_vector_table[1];

    /* set the stack pointer */
    __TZ_set_MSP_NS(msp_ns_value);
    __set_MSP(0x20017FE0);

    /* set VTOR */
    SCB_NS->VTOR = (uint32_t)ns_vector_table;

    /* create an entry point */
    funcptr_ns ns_entry = (funcptr_ns) cmse_nsfptr_create((void*)reset_ns);

    /* call */
    ns_entry();

}
