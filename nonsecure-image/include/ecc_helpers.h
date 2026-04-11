#ifndef ECC_HELPERS_H
#define ECC_HELPERS_H

#include <stddef.h>
int ecc_sign_input(const char *msg, size_t msg_len, char *out, size_t *out_len);

int sign_cmd(int argc, char **argv);

#endif
