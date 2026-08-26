#ifndef DD_TYPES_H
#define DD_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define SUCCESS 1
#define FAIL    0

#define DRV_LED   0
#define DRV_UART  1
#define MAX_DRIVERS 2

#define LED_SET   0
#define LED_FLIP  1

#define UART_READ_CHAR  0
#define UART_SEND_CHAR  1

#define MAX_COMMANDS 2

#ifdef __cplusplus
}
#endif

#endif