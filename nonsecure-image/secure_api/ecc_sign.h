#ifndef ECC_SIGN_H
#define ECC_SIGN_H

#include <stddef.h>
#include <stdint.h>

/* ECDSA signature of a message hash */
extern int ecc_sign_secure(const unsigned char *hash, int hash_len, char *out, unsigned int *out_len);

/* public key import */
extern int ecc_import_pubkey_secure(uint8_t *out, size_t *out_len);

#ifdef __TZ_INSECURE_DEV
extern uint8_t *ecc_import_privkey_insecure(void);
#endif

#endif
