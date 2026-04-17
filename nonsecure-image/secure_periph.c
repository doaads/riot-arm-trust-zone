#include "secure_api/secure_periph.h"
#include <stdio.h>

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

void hwrng_read(void *buf, unsigned int num) {
    return hwrng_read_secure(buf, num);
}
