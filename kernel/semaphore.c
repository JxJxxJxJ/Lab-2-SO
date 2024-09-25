#include "semaphore.h"
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#define MAX_SEM 3

// Todas estas syscalls, realmente, estan modificando 
// una entrada en la SEM_ARRAY, es importante que, como
// RISC-V es multicore, esta modificacion se haga de forma
// atomica. xv6's locks enter the game

// Voy a usar locks para proteger una seccion critica, que
// va a ser cuando accedo a las entries del arreglo de sema-
// foros. Se dice que el lock va a proteger a la entry del
// arreglo.

typedef struct sem_s {
  struct spinlock lock;
  int value;
} semaphore;

semaphore sem_array[MAX_SEM];

uint64
sem_open(int sem, int value)
{
  // Adquiero el lock del semaforo sem
  acquire(&sem_array[sem].lock);

  printf("Se ejecuta semopen\n");

  // Devuelvo el lock del semaforo sem
  release(&sem_array[sem].lock);
  return sem;
}

uint64
sem_close(int sem)
{
  // Adquiero el lock del semaforo sem
  acquire(&sem_array[sem].lock);

  printf("Se ejecuta semclose\n");

  // Devuelvo el lock del semaforo sem
  release(&sem_array[sem].lock);
  return sem;
}

uint64
sem_up(int sem)
{
  // Adquiero el lock del semaforo sem
  acquire(&sem_array[sem].lock);

  printf("Se ejecuta semup\n");

  // Devuelvo el lock del semaforo sem
  release(&sem_array[sem].lock);
  return sem;

}

uint64
sem_down(int sem)
{
  // Adquiero el lock del semaforo sem
  acquire(&sem_array[sem].lock);

  printf("Se ejecuta semdown\n");

  // Devuelvo el lock del semaforo sem
  release(&sem_array[sem].lock);
  return sem;
}
