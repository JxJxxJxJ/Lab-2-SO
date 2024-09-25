#include "semaphore.h"
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

uint64
sem_open(int sem, int value)
{
  printf("Se ejecuta semopen\n");
  return sem;
}

uint64
sem_close(int sem)
{
  printf("Se ejecuta semclose\n");
  return sem;
}

uint64
sem_up(int sem)
{
  printf("Se ejecuta semup\n");
  return sem;
}

uint64
sem_down(int sem)
{
  printf("Se ejecuta semdown\n");
  return sem;
}
