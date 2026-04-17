#include "panic.h"
#include "include/tz_config.h"
#include "include/crypto.h"
#include <stdio.h>

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

