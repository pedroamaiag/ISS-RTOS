#ifndef UART_DRIVER_H
#define UART_DRIVER_H
#include "dd_types.h"

char uartInit(void * parameters);
char uartReadChar(void * parameters);
char uartSendChar(void * parameters);

#endif
