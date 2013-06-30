#ifndef KERNEL_SPINLOCK_H
#define KERNEL_SPINLOCK_H

#include "int.h"
#include "common.h"
#include "atomic.h"

typedef uint16_t ticket_t;

//Must be strictly in this order, or else head will overflow into tail
//This ordering will arrange a DWORD in memory like (0xTTTTHHHH)
typedef struct ticket_pair {
    ticket_t head;
    ticket_t tail;
} PACKED ticket_pair_t;

typedef struct spinlock {
    ticket_pair_t tickets;
} spinlock_t;

#define SPINLOCK_LOCKED   {.tickets = {.head = 0, .tail = 1}}
#define SPINLOCK_UNLOCKED {.tickets = {.head = 0, .tail = 0}}

#define SPINLOCK_INIT(name) spinlock_t name = SPINLOCK_UNLOCKED

void spin_lock(spinlock_t *lock);
void spin_unlock(spinlock_t *lock);

void spin_lock_irq(spinlock_t *lock);
void spin_unlock_irq(spinlock_t *lock);

void spin_lock_irqsave(spinlock_t *lock, uint32_t *flags);
void spin_unlock_irqstore(spinlock_t *lock, uint32_t flags);

#endif
