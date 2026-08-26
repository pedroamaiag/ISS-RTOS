#include "driver_controller.h"
#include "led_driver.h"
#include "uart_driver.h"
#include <stddef.h>

char (*init_functions[MAX_DRIVERS])(void *);
char (*driver_functions[MAX_DRIVERS][MAX_COMMANDS])(void *);

void initCtrDrv(void) {
    for (int i = 0; i < MAX_DRIVERS; i++) {
        init_functions[i] = NULL;
        for (int j = 0; j < MAX_COMMANDS; j++) {
            driver_functions[i][j] = NULL;
        }
    }
}

char initDriver(char driverID) {
    int id = (int)driverID;

    if (id < 0 || id >= MAX_DRIVERS) {
        return FAIL;
    }

    switch (id) {
        case DRV_LED:
            init_functions[DRV_LED] = ledInit;
            driver_functions[DRV_LED][LED_SET]  = ledSet;
            driver_functions[DRV_LED][LED_FLIP] = ledFlip;
            break;

        case DRV_UART:
            init_functions[DRV_UART] = uartInit;
            driver_functions[DRV_UART][UART_READ_CHAR] = uartReadChar;
            driver_functions[DRV_UART][UART_SEND_CHAR] = uartSendChar;
            break;

        default:
            return FAIL;
    }

    if (init_functions[id] != NULL) {
        return init_functions[id](NULL);
    }

    return FAIL;
}

char callDriver(char driverID, char command, void * args) {
    int id  = (int)driverID;
    int cmd = (int)command;

    if (id < 0 || id >= MAX_DRIVERS ||
        cmd < 0 || cmd >= MAX_COMMANDS) {
        return FAIL;
    }

    if (driver_functions[id][cmd] != NULL) {
        return driver_functions[id][cmd](args);
    }

    return FAIL;
}
