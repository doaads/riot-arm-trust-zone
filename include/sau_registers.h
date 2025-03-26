// sau_registers.h
#include <stdint.h>

#ifndef SAU_REGISTERS_H
#define SAU_REGISTERS_H

typedef struct {
    volatile uint32_t CTRL;  /* SAU Control Register              */
    volatile uint32_t TYPE;  /* SAU Type Register                 */
    volatile uint32_t RNR;   /* SAU Region Number Register        */
    volatile uint32_t RBAR;  /* SAU Region Base Register          */
    volatile uint32_t RLAR;  /* SAU Region Limit Address Register */
    volatile uint32_t SFSR;  /* Secure Fault Status Register      */
    volatile uint32_t SFAR;  /* Secure Fault Address Register     */
} SAU_Type;

/* From Arm v8-M Architercture Reference Manual */
#define SAU_BASE (0xE000EDD0)
#define SAU ((SAU_Type*)SAU_BASE)

#endif
