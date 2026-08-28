#include "kernel.h"
#include "dd_types.h"
#include <stdint.h>
#include <util/atomic.h>

process * process_list[MAX_PROCESSES];
int process_count = 0;
volatile unsigned long system_ticks = 0;

static char timeReached(unsigned long current_time, unsigned long target_time) {
    return ((int32_t)(current_time - target_time) >= 0);
}

void kernelInit(void) {
    process_count = 0;
    system_ticks = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_list[i] = NULL;
    }
}

unsigned long kernelGetTicks(void) {
    unsigned long ticks;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ticks = system_ticks;
    }

    return ticks;
}

void yield(void) {
    asm volatile("nop");
}

char kernelAddProc(process * new_proc) {
    if (new_proc == NULL) return FAIL;

    for (int i = 0; i < process_count; i++) {
        if (process_list[i] == new_proc) {
            return FAIL;
        }
    }

    if (process_count < MAX_PROCESSES) {
        process_list[process_count] = new_proc;
        process_count++;
        return SUCCESS;
    }

    return FAIL;
}

char kernelRemoveProc(process * proc) {
    if (proc == NULL) return FAIL;

    for (int i = 0; i < process_count; i++) {
        if (process_list[i] == proc) {
            for (int j = i; j < process_count - 1; j++) {
                process_list[j] = process_list[j + 1];
            }
            process_count--;
            process_list[process_count] = NULL;
            return SUCCESS;
        }
    }

    return FAIL;
}

void kernelLoop(void) {
    while (1) {
        int next_proc = -1;
        unsigned long current_time = kernelGetTicks();

        for (int i = 0; i < process_count; i++) {
            if (timeReached(current_time, process_list[i]->deadline) &&
                (next_proc == -1 ||
                 (int32_t)(process_list[i]->deadline -
                           process_list[next_proc]->deadline) < 0)) {

                next_proc = i;
            }
        }

        if (next_proc != -1) {
            char status = process_list[next_proc]->func();

            if (status == SUCCESS) {
                process_list[next_proc]->deadline =
                    kernelGetTicks() + process_list[next_proc]->period;
            } else {
                for (int j = next_proc; j < process_count - 1; j++) {
                    process_list[j] = process_list[j + 1];
                }
                process_count--;
                process_list[process_count] = NULL;
            }
        }

        yield();
    }
}
