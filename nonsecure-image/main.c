/*
 * Copyright (C) 2008, 2009, 2010 Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2013 INRIA
 * Copyright (C) 2013 Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Default application that shows a lot of functionality of RIOT
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <arm_cmse.h>
#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "ns_uart.h"
#include <arm_cmse.h>
#include "core_cm33.h"

void uart_write(uart_t uart, const uint8_t *data, size_t len)
{
    uart_write_secure(uart, data, len);
}

ssize_t stdio_read(void* buffer, size_t len)
{
    return stdio_read_secure(buffer, len);
}

void pm_off(void) {
    return pm_off_secure();
}

static const shell_command_t shell_commands[] = {
	{ NULL, NULL, NULL }
};

int main(void)
{
    (void) puts("Welcome to RIOT! Running in Non-secure world.");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
