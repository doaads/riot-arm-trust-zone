#include "panic.h"
#include "include/tz_config.h"
#include <stdio.h>

//NORETURN void core_panic(core_panic_t crash_code, const char *message)
//{
//    (void)crash_code;
//    (void)message;
//    while (1) {}
//}

int main(void) {
    puts("RIOT OS secure-bootloader\n");
    jump_to_nonsecure();
    while (1) {}
}

