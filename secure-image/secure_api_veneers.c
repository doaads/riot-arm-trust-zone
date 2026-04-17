#include "periph/hwrng.h"
#include "periph/uart.h"
#include "stdio_base.h"
#include "periph/pm.h"
#include "core_cm33.h"
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
    uart_write(uart, data, len);
}

__attribute__((cmse_nonsecure_entry))
ssize_t stdio_read_secure(void* buffer, size_t len) {
    return stdio_read(buffer, len);
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

/* Hardware Random Number Generator (for micro-ecc) */
__attribute__((cmse_nonsecure_entry))
void hwrng_read_secure(void *buf, unsigned int num) {
    return hwrng_read(buf, num);
}
