#ifndef SECURE_API_H
#define SECURE_API_H

#include "periph/uart.h"
#include <stdio.h>

/* stdio (for RIOT NS shell) */
void uart_write_secure(uart_t uart, const uint8_t *data, size_t len);
ssize_t stdio_read_secure(void* buffer, size_t len);

/* power management */
void pm_off_secure(void);

/* crypto */
int ecc_sign_secure(const unsigned char *hash, int hash_len, char *out, unsigned int *out_len);

/* API client facing function renamed to import for clarity */
int ecc_import_pubkey_secure(uint8_t *out, size_t *out_len);

/* hwrng */
void hwrng_read_secure(void *buf, unsigned int num);

#endif
