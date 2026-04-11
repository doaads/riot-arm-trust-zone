#include <string.h>
#include "uECC.h"
#include "base64.h"
#include "periph/hwrng.h"

static const struct uECC_Curve_t *curve = NULL;
static uint8_t s_private_key[32];
static uint8_t s_public_key[64];

static int rng_func(uint8_t *dest, unsigned size) {
    hwrng_read(dest, size);
    return 1;
}

int ecc_init(void) {
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

/* DER-sign a message */
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

/* base64-encode a signature */
int ecc_sign_base64(const uint8_t *sig, size_t sig_len, char *out, size_t *out_len)
{
    return base64_encode(sig, sig_len, (uint8_t *)out, out_len);
}

/* base64 signature wrapper */
int ecc_sign(const uint8_t *hash, size_t hash_len, char *out, size_t *out_len)
{
    uint8_t sig[64];
    size_t  sig_len = sizeof(sig);

    int ret = ecc_sign_raw(hash, hash_len, sig, &sig_len);
    if (ret) return ret;

    return ecc_sign_base64(sig, sig_len, out, out_len);
}

/* export the public key for signature validation */
int ecc_export_pubkey(uint8_t *out, size_t *out_len) {
    if (*out_len < 65) return -1;

    out[0] = 0x04;
    memcpy(out + 1, s_public_key, 64);
    *out_len = 65;
    return 0;
}
