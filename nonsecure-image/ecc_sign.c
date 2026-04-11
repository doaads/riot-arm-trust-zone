#include "secure_api/ecc_sign.h"
#include "include/ecc_helpers.h"
#include <stdio.h>
#include "hashes/sha256.h"

int ecc_sign_input(const char *msg, size_t msg_len, char *out, size_t *out_len) {
    uint8_t hash[SHA256_DIGEST_LENGTH];

    /* hash the input */
    sha256((const uint8_t *)msg, msg_len, hash);

    /* sign the hash */
    return ecc_sign_secure(hash, sizeof(hash), out, out_len);
}

int ecc_verify_signature(const char *msg, size_t msg_len, char *sig, size_t sig_len) {
    (void)msg;
    (void)msg_len;
    (void)sig;
    (void)sig_len;

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
        printf("Signing failed: %d\n", ret);
        return 1;
    }

    printf("Signature (base64): %.*s\n", (int)sig_len, sig_b64);
    return 0;
}

int verify_cmd(int argc, char **argv) {

    if (argc < 3) {
        printf("Usage: verify <message> <signature>");
    }

    char* msg = argv[1];
    size_t msg_len = strlen(msg);

    char* sig = argv[2];
    size_t sig_len = strlen(sig);

    return ecc_verify_signature(msg, msg_len, sig, sig_len);

    return 0;
}
