/*
 * Copyright (C) 2017 Kaspar Schleiser <kaspar@schleiser.de>
 *                    Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     bootloaders
 * @{
 *
 * @file
 * @brief      Minimal riot-based bootloader, modified to support ARM TrustZone
 *
 * @author     Kaspar Schleiser <kaspar@schleiser.de>
 * @author     Francisco Acosta <francisco.acosta@inria.fr>
 *
 * @}
 */

#include "panic.h"
#include "include/tz_config.h"
#include <stdio.h>

//void kernel_init(void)
//{
//    jump_to_nonsecure();
//    while (1) {}
//}

NORETURN void core_panic(core_panic_t crash_code, const char *message)
{
    (void)crash_code;
    (void)message;
    while (1) {}
}

int main(void) {
    puts("RIOT OS secure-bootloader\n");
    jump_to_nonsecure();
    return 0;
}

