#ifndef ECC_HELPERS_H
#define ECC_HELPERS_H

#include <stddef.h>
int ecc_sign_input(const char *msg, size_t msg_len, char *out, size_t *out_len);

int sign_cmd(int argc, char **argv);

int verify_cmd(int argc, char **argv);

int sign_verify_cmd(int argc, char **argv);

int invalid_access_demo(int argc, char **argv);

#endif
