#ifndef NS_UART_H
#define NS_UART_H

#include "periph/uart.h"

extern void uart_write_secure(uart_t uart, const uint8_t *data, size_t len);
extern ssize_t stdio_read_secure(void* buffer, size_t len);
extern void pm_off_secure(void);

#endif

