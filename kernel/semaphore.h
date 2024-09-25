#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "types.h"  // me da al amigo uint64
uint64    sem_open(int sem, int value);
uint64    sem_close(int sem);
uint64    sem_up(int sem);
uint64    sem_down(int sem);

#endif 