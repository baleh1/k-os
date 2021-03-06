#include "lib/int.h"
#include "common/list.h"
#include "sync/spinlock.h"
#include "sync/semaphore.h"
#include "task/task.h"

void semaphore_down(semaphore_t *lock) {
    uint32_t flags;
    spin_lock_irqsave(&lock->lock, &flags);

    if(lock->count) {
        lock->count--;

        spin_unlock_irqstore(&lock->lock, flags);
    } else {
        list_add(&current->wait_list, &lock->waiters);

        spin_unlock_irqstore(&lock->lock, flags);

        task_sleep_current();
    }
}

void semaphore_up(semaphore_t *lock) {
    uint32_t flags;
    spin_lock_irqsave(&lock->lock, &flags);

    if(list_empty(&lock->waiters)) {
        lock->count++;
    } else {
        task_t *waiting = list_first(&lock->waiters, task_t, wait_list);

        task_wake(waiting);

        list_rm(&waiting->wait_list);
    }

    spin_unlock_irqstore(&lock->lock, flags);
}
