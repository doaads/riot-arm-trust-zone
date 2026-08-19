#ifndef CRYPTO_SECURE_API_H
#define CRYPTO_SECURE_API_H

#include <stddef.h>
#include <stdint.h>

/* RNG helper function for micro-ecc */
static int rng_func(uint8_t *dest, unsigned size);

/* key generation */
int ecc_init(void);
int ecc_generate_key(void);

/* signature helpers */
int ecc_sign_raw(const uint8_t *hash, size_t hash_len, uint8_t *sig, size_t *sig_len);
int ecc_sign_base64(const uint8_t *sig, size_t sig_len, char *out, size_t *out_len);

/* main sign wrapper */
int ecc_sign(const uint8_t *hash, size_t hash_len, char *out, size_t *out_len);

/* other */
int ecc_export_pubkey(uint8_t *out, size_t *out_len);

/* debug */
# ifdef __TZ_INSECURE_DEV
uint8_t *ecc_export_privkey(void);
#endif

#endif
