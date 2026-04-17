#include <arm_cmse.h>
#include <stdio.h>
#include "shell.h"
#include <arm_cmse.h>
#include "core_cm33.h"
#include "include/ecc_helpers.h"

static const shell_command_t shell_commands[] = {
	{ "sign", "Sign message with ECDSA (delegate to S world)", sign_cmd },
	{ "verify", "Verify ECDSA signature (in NS world)", verify_cmd },
	{ "sign-verify", "ECDSA sign & verify", sign_verify_cmd },
    { NULL, NULL, NULL }
};

int main(void)
{
    (void) puts("Welcome to RIOT! Running in Non-secure world.");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
