#include "secure_api/ecc_sign.h"
#include "secure_api/secure_periph.h"
#include "include/ecc_helpers.h"
#include "uECC.h"
#include <stdio.h>
#include "hashes/sha256.h"
#include "base64.h"

int ecc_sign_input(const char *msg, size_t msg_len, char *out, size_t *out_len) {
    uint8_t hash[SHA256_DIGEST_LENGTH];

    /* hash the input */
    sha256((const uint8_t *)msg, msg_len, hash);

    /* sign the hash */
    return ecc_sign_secure(hash, SHA256_DIGEST_LENGTH, out, out_len);
}

int ecc_verify_signature(const char *msg, size_t msg_len, char *sig, size_t sig_len) {
    const struct uECC_Curve_t *curve = uECC_secp256r1();

    uint8_t pubkey[64];
    size_t pubkey_len = sizeof(pubkey);

    uint8_t hash[SHA256_DIGEST_LENGTH];
    sha256((const uint8_t *)msg, msg_len, hash);

    uint8_t sig_bin[80];
    size_t sig_bin_len = sizeof(sig_bin);

    int ret = base64_decode(sig, sig_len, sig_bin, &sig_bin_len);
    if (ret < 0) {
        printf("! Error on base64-decoding signature (%d). Ensure a correct signature was passed\n", ret);
        return -1;
    }

    if (ecc_import_pubkey_secure(pubkey, &pubkey_len) != 0) {
        puts("! Error on importing public key");
        return -2;
    }

    ret = uECC_verify(pubkey, hash, sizeof(hash), sig_bin, curve);
    if (ret != 1) {
        puts("! Signature invalid");
        return -3;
    }

    puts("> Signature verified. Result: valid!");
    return 0;
}

int sign_cmd(int argc, char **argv)
{
    if (argc < 2) {
        puts("Usage: sign <message>");
        return 1;
    }

    char sig_b64[128];
    unsigned int sig_len = sizeof(sig_b64);

    char* msg = argv[1];
    size_t msg_len = strlen(msg);

    int ret = ecc_sign_input(msg, msg_len, sig_b64, &sig_len);
    if (ret != 0) {
        printf("! Signing failed: %d\n", ret);
        return 1;
    }

    printf("> Signature (base64): %.*s\n", (int)sig_len, sig_b64);
    return 0;
}

int verify_cmd(int argc, char **argv) {

    if (argc < 3) {
        puts("Usage: verify <message> <signature>");
        return 1;
    }

    char* msg = argv[1];
    size_t msg_len = strlen(msg);

    char* sig = argv[2];
    size_t sig_len = strlen(sig);

    return ecc_verify_signature(msg, msg_len, sig, sig_len);
}

int sign_verify_cmd(int argc, char **argv) {

    if (argc < 2) {
        puts("Usage: sign-verify <message>");
        return 1;
    }

    char sig_b64[128];
    unsigned int sig_len = sizeof(sig_b64);

    char* msg = argv[1];
    size_t msg_len = strlen(msg);

    int ret = ecc_sign_input(msg, msg_len, sig_b64, &sig_len);
    if (ret) return ret;
    printf("> Signature (base64): %.*s\n", (int)sig_len, sig_b64);

    puts("> Verifying signature...");
    return ecc_verify_signature(msg, msg_len, sig_b64, sig_len);
}

#ifdef __TZ_INSECURE_DEV
int invalid_access_demo(int argc, char **argv) {
    (void)argc;
    (void)argv;

    uint8_t *privkey = ecc_import_privkey_insecure();

    puts("Demo script for TrustZone security check\n");

    /* attempt print using uart_write */
    puts("1. Attempt print private key using uart_write");
    printf("? PRIVKEY:\n");
    uart_write_secure(0, privkey, sizeof(uint8_t) * 32);
    printf("\n(PASS if empty)\n");

    /* attempt write using stdio_read */
    puts("2. Attempt rewrite private key using stdio_read");
    size_t ret = stdio_read_secure(privkey, sizeof(uint8_t) * 32);
    if (ret) {
        puts("! FAIL: buffer modified");
    } else {
        puts("> PASS");
    }

    puts("3. Attempt direct access to private key");
    printf("Privkey byte: %d\n", *privkey);

    puts("! FAIL: Unreachable! Expected secure fault");

    return 0;
}
#else
int invalid_access_demo(int argc, char **argv) {
    (void)argc;
    (void)argv;

    int *ptr = (int*)0x08000000;
    printf("Secure image start address: %d\n", *ptr);

    return 0;
}
#endif
