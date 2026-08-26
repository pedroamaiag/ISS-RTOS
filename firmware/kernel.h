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
    unsigned long deadline;
} process;

void kernelInit(void);
char kernelAddProc(process *p);
void kernelLoop(void);
void kernelTick(void);
void yield(void);

#ifdef __cplusplus
}
#endif

#endif