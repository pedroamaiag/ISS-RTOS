#include "kernel.h"
#include "driver_controller.h"
#include "dd_types.h"
#include <avr/interrupt.h>

extern volatile unsigned long system_ticks;

volatile char piscar_ativo = 0;
unsigned long proximo_alerta_disponivel = 0;

char tarefa_monitorar_iss(void);
char tarefa_piscar_led(void);
char tarefa_apagar_led(void);

process p_uart   = { tarefa_monitorar_iss, 1,   0 };   
process p_blink  = { tarefa_piscar_led,   200, 0 };   
process p_stop   = { tarefa_apagar_led,   0,   0 };   

void enviarMensagem(const char* msg) {
    for (int i = 0; msg[i] != '\0'; i++) {
        while (callDriver(DRV_UART, UART_SEND_CHAR, (void*)&msg[i]) == FAIL) {
        }
    }

    char nl = '\n';
    while (callDriver(DRV_UART, UART_SEND_CHAR, &nl) == FAIL) {
    }
}

char tarefa_monitorar_iss(void) {
    char sinal;

    while (callDriver(DRV_UART, UART_READ_CHAR, &sinal) == SUCCESS) {

        if (sinal == 'A' || sinal == 'a') {

            if (system_ticks >= proximo_alerta_disponivel) {

                enviarMensagem(">>> S.O.: EVENTO DETECTADO");

                piscar_ativo = 1;

                p_blink.deadline = system_ticks;
                kernelAddProc(&p_blink);

                p_stop.deadline = system_ticks + 2000;
                kernelAddProc(&p_stop);

                proximo_alerta_disponivel = system_ticks + 5000;
            }
        }
    }
    return SUCCESS;
}

char tarefa_piscar_led(void) {
    if (!piscar_ativo) {
        return FAIL;
    }

    callDriver(DRV_LED, LED_FLIP, NULL);
    return SUCCESS;
}

char tarefa_apagar_led(void) {
    piscar_ativo = 0;

    char off = 0;
    callDriver(DRV_LED, LED_SET, &off);

    enviarMensagem("S.O.: Evento encerrado");
    return FAIL;
}

int main(void) {
    kernelInit();
    initCtrDrv();
    initDriver(DRV_UART);
    initDriver(DRV_LED);

    char off = 0;
    callDriver(DRV_LED, LED_SET, &off);

    kernelAddProc(&p_uart);

    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A  = 249;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei();

    enviarMensagem("S.O.: SISTEMA PRONTO");
    kernelLoop();
}

ISR(TIMER1_COMPA_vect) {
    system_ticks++;
}
