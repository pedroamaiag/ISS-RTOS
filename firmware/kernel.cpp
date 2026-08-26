#include "kernel.h"
#include "dd_types.h"

process * process_list[MAX_PROCESSES];
int process_count = 0;
volatile unsigned long system_ticks = 0;

void kernelInit(void) {
    process_count = 0;
    system_ticks = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_list[i] = NULL;
    }
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

void kernelLoop(void) {
    while (1) {
        int next_proc = -1;
        unsigned long min_deadline = 0xFFFFFFFF;

        for (int i = 0; i < process_count; i++) {
            if (process_list[i]->deadline <= system_ticks &&
                process_list[i]->deadline < min_deadline) {

                min_deadline = process_list[i]->deadline;
                next_proc = i;
            }
        }

        if (next_proc != -1) {
            char status = process_list[next_proc]->func();

            if (status == SUCCESS) {
                process_list[next_proc]->deadline =
                    system_ticks + process_list[next_proc]->period;
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