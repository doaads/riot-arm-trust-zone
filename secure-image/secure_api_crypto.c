#include <stdio.h>
#include <string.h>
#include "arm_cmse.h"
#include "uECC.h"
#include "base64.h"
#include "periph/hwrng.h"
#include "cmsis_gcc.h"

static const struct uECC_Curve_t *curve = NULL;
static uint8_t s_private_key[32];
static uint8_t s_public_key[64];

/* RNG function for micro ecc */
static int rng_func(uint8_t *dest, unsigned size) {
    hwrng_read(dest, size);
    return 1;
}

/* set curve and rng function */
int ecc_init(void) {
    hwrng_init();
    curve = uECC_secp256r1();
    uECC_set_rng(rng_func);
    return 0;
}

/* generate a pair of ECDSA keys */
int ecc_generate_key(void) {

    if (!uECC_make_key(s_public_key, s_private_key, curve)) {
        return -1;
    }

    return 0;
}

/* raw-sign a message */
int ecc_sign_raw(const uint8_t *hash, size_t hash_len, uint8_t *sig, size_t *sig_len)
{
    uint8_t raw_sig[64];

    if (*sig_len < 64) return -1;

    if (!uECC_sign(s_private_key, hash, hash_len, raw_sig, curve)) {
        return -1;
    }

    memcpy(sig, raw_sig, 64);
    *sig_len = 64;
    return 0;
}

/* base64 signature wrapper */
int ecc_sign(const uint8_t *hash, size_t hash_len, char *out, size_t *out_len) {
    uint8_t *hash_s;
    size_t *out_len_s;
    char *out_s;
    int ret;
    uint8_t sig[64];
    size_t  sig_len = sizeof(sig);

    /* validate all passed pointers */
    out_len_s = cmse_check_address_range(
            out_len, sizeof(size_t), CMSE_NONSECURE);
    if (!out_len_s) return -2;

    out_s = cmse_check_address_range(
            out, *out_len_s, CMSE_NONSECURE);
    if (!out_s) return -2;

    hash_s = cmse_check_address_range(
            (void *)hash, hash_len, CMSE_NONSECURE);
    if (!hash_s) return -2;

    __DMB();

    ret = ecc_sign_raw(hash_s, hash_len, sig, &sig_len);
    if (ret) return ret;

    return base64_encode(sig, sig_len, out_s, out_len_s);
}

/* export the public key for signature validation */
int ecc_export_pubkey(uint8_t *out, size_t *out_len) {
    /* validate all pointers */
    size_t *out_len_s = cmse_check_address_range(
            out_len, sizeof(size_t), CMSE_NONSECURE);
    if (!out_len_s) return -2;

    uint8_t *out_s = cmse_check_address_range(
            out, *out_len_s, CMSE_NONSECURE);
    if (!out_s) return -2;

    __DMB();

    /* ensure enough space is reseved */
    if (*out_len_s < 64) return -2;

    memcpy(out_s, s_public_key, 64);

    *out_len_s = 64;
    return 0;
}

#ifdef __TZ_INSECURE_DEV
uint8_t *ecc_export_privkey(void) {
    return s_private_key;
}
#endif
