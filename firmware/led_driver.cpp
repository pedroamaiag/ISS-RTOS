#include "led_driver.h"
#include <avr/io.h>
#include <stddef.h>

char ledInit(void * parameters) {
    (void)parameters;

    PORTA &= ~(1 << PORTA0);

    DDRA |= (1 << DDA0);

    return SUCCESS;
}

char ledSet(void * parameters) {
    if (parameters == NULL) {
        return FAIL;
    }

    char value = *(char *)parameters;

    if (value) {
        PORTA |= (1 << PORTA0);
    } else {
        PORTA &= ~(1 << PORTA0);
    }

    return SUCCESS;
}

char ledFlip(void * parameters) {
    (void)parameters;
    PORTA ^= (1 << PORTA0);
    return SUCCESS;
}
