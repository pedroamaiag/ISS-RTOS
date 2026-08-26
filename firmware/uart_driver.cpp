#include "uart_driver.h"
#include "dd_types.h"
#include <stddef.h>   
#include <avr/io.h>

char uartInit(void * parameters) {
    (void)parameters;

    unsigned int baud = 103; 
    UBRR0H = (unsigned char)(baud >> 8);
    UBRR0L = (unsigned char)baud;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    return SUCCESS;
}

char uartReadChar(void * parameters) {
    if (parameters == NULL) {
        return FAIL;
    }

    if (UCSR0A & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))) {
        volatile char dummy = UDR0;
        (void)dummy;
        return FAIL;
    }

    if (!(UCSR0A & (1 << RXC0))) {
        return FAIL;
    }

    *(unsigned char *)parameters = UDR0;
    return SUCCESS;
}

char uartSendChar(void * parameters) {
    if (parameters == NULL) {
        return FAIL;
    }

    if (!(UCSR0A & (1 << UDRE0))) {
        return FAIL;
    }

    UDR0 = *(unsigned char *)parameters;
    return SUCCESS;
}
