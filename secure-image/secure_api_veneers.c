#include "periph/hwrng.h"
#include "periph/uart.h"
#include "stdio_base.h"
#include "periph/pm.h"
#include "core_cm33.h"
#include <arm_cmse.h>
#include <stdio.h>
#include "include/secure_api.h"
#include "include/crypto.h"

/* power management */
__attribute__((cmse_nonsecure_entry))
void pm_off_secure(void) {
    pm_off();
}

/* stdio */
__attribute__((cmse_nonsecure_entry))
void uart_write_secure(uart_t uart, const uint8_t *data, size_t len) {
    uart_write_secure_entry(uart, data, len);
}

/* validate pointer to ensure secure world stdio confidentiality */
void uart_write_secure_entry(
        uart_t uart, const uint8_t *data, size_t len) {
    const uint8_t *data_s = 
        cmse_check_address_range((void *)data, len, CMSE_NONSECURE);
    if (!data_s) return; 

    uart_write(uart, data_s, len);
}

__attribute__((cmse_nonsecure_entry))
ssize_t stdio_read_secure(void *buffer, size_t len) {
    return stdio_read_secure_entry(buffer, len);
}

/* validate pointer to ensure secure world stdio integrity */
ssize_t stdio_read_secure_entry(void *buffer, size_t len) {
    void *buffer_s =
        cmse_check_address_range(buffer, len, CMSE_NONSECURE);
    if (!buffer_s) return 0; 

    return stdio_read(buffer_s, len);
}

/* crypto */
__attribute__((cmse_nonsecure_entry))
int ecc_sign_secure(const unsigned char *hash, int hash_len, char *out, unsigned int *out_len) {
    return ecc_sign(hash, hash_len, out, out_len);
}

__attribute__((cmse_nonsecure_entry))
int ecc_import_pubkey_secure(uint8_t *out, size_t *out_len) {
    return ecc_export_pubkey(out, out_len);
}

#ifdef __TZ_INSECURE_DEV
__attribute__((cmse_nonsecure_entry))
uint8_t *ecc_import_privkey_insecure(void) {
    return ecc_export_privkey();
}
#endif

/* Hardware Random Number Generator (for micro-ecc) */
__attribute__((cmse_nonsecure_entry))
void hwrng_read_secure(void *buf, unsigned int num) {
    return hwrng_read(buf, num);
}
