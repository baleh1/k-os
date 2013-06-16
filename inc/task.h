#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <stdint.h>
#include "registers.h"

typedef struct task {
    uint32_t pid;
    registers_t registers;
    task_state_t state;
} task_t;

void task_switch();
void * task_alloc_page(task_t *task, uint32_t vaddr);
task_t * task_create();

void task_usermode();

#endif