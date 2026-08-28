#ifndef KERNEL_H
#define KERNEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define MAX_PROCESSES 10

typedef struct {
    char (*func)(void);
    unsigned long period;
    unsigned long next_release;
    unsigned long relative_deadline;
    unsigned long absolute_deadline;
} process;

void kernelInit(void);
char kernelAddProc(process *p);
char kernelRemoveProc(process *p);
unsigned long kernelGetTicks(void);
void kernelLoop(void);
void yield(void);

#ifdef __cplusplus
}
#endif

#endif
