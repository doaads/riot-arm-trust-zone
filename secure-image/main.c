#include "panic.h"
#include "include/tz_config.h"
#include "include/crypto.h"
#include <stdio.h>

//NORETURN void core_panic(core_panic_t crash_code, const char *message)
//{
//    (void)crash_code;
//    (void)message;
//    while (1) {}
//}

int main(void) {
    puts("RIOT OS secure-bootloader\n");
    
    puts("> Initializing crypto...\n");
    ecc_init();

    puts("> Generating ECC private key...\n");
    ecc_generate_key();

    puts("> Done! Booting application...\n");
    jump_to_nonsecure();

    while (1) {}
}

