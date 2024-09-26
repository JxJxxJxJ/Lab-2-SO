#include "semaphore.h"
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "stdbool.h"

#define MAX_SEM            500
#define ERROR_CODE         0
#define SUCCESS_CODE       1
#define CLOSED_SEM_VALUE   -1

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
  bool is_active;
} semaphore;

semaphore sem_array[MAX_SEM];


uint64
sem_init_array(void)
{
  for (int i = 0; i < MAX_SEM; i++){
    sem_array[i].value = -1;
    sem_array[i].is_active = false;
  }
  return SUCCESS_CODE;
}

uint64
sem_open(int sem, int value)
{
  // Adquiero el lock del semaforo sem
  acquire(&(sem_array[sem].lock));

  // ----------- ZONA CRITICA -----------
  
  if (sem < 0 || sem >= MAX_SEM){
    printf("ERROR: Indice fuera del arreglo de semaforos. (%d) debe estar entre 0 y (%d)\n", sem, MAX_SEM);
    return ERROR_CODE;
  }
  if (sem >= 0 && sem < MAX_SEM){
    if(value < 0){
     printf("ERROR: No se puede inicializar un semaforo con un valor < 0. (%d)\n", value);
     return ERROR_CODE;
    };

    // printf("Se ejecuta semopen\n");
    sem_array[sem].value = value;
    sem_array[sem].is_active = true;

    // -------- FIN ZONA CRITICA -----------

    // Devuelvo el lock del semaforo sem
    release(&(sem_array[sem].lock));
  }
  return SUCCESS_CODE;
}

uint64
sem_close(int sem)
{
  // Adquiero el lock del semaforo sem
  acquire(&(sem_array[sem].lock));

  // ----------- ZONA CRITICA -----------

  if (sem < 0 || sem >= MAX_SEM){
    printf("ERROR: Indice fuera del arreglo de semaforos. (%d) debe estar entre 0 y (%d)\n", sem, MAX_SEM);
    return ERROR_CODE;
  }

  // printf("Se ejecuta semclose\n");
  sem_array[sem].value = CLOSED_SEM_VALUE;
  sem_array[sem].is_active = false;

  // -------- FIN ZONA CRITICA -----------
  
  // Devuelvo el lock del semaforo sem
  release(&(sem_array[sem].lock));
  return SUCCESS_CODE;
}

uint64
sem_up(int sem)
{
  // Adquiero el lock del semaforo sem
  acquire(&(sem_array[sem].lock));

  // ----------- ZONA CRITICA -----------

  if (sem < 0 || sem >= MAX_SEM){
    printf("ERROR: Indice fuera del arreglo de semaforos. (%d) debe estar entre 0 y (%d)\n", sem, MAX_SEM);
    return ERROR_CODE;
  }

  // printf("Se ejecuta semup\n");
  sem_array[sem].value++;
  wakeup(&(sem_array[sem]));

  // -------- FIN ZONA CRITICA -----------
  
  // Devuelvo el lock del semaforo sem
  release(&(sem_array[sem].lock));
  return SUCCESS_CODE;

}

uint64
sem_down(int sem)
{
  // Adquiero el lock del semaforo sem
  acquire(&(sem_array[sem].lock));

  // ----------- ZONA CRITICA -----------

  if (sem < 0 || sem >= MAX_SEM){
    printf("ERROR: Indice fuera del arreglo de semaforos. (%d) debe estar entre 0 y (%d)\n", sem, MAX_SEM);
    return ERROR_CODE;
  }

  // printf("Se ejecuta semdown\n");
  while (sem_array[sem].value <= 0) {
        sleep(&(sem_array[sem]), &(sem_array[sem].lock)); 
  }
  sem_array[sem].value--;

  // -------- FIN ZONA CRITICA -----------

  // Devuelvo el lock del semaforo sem
  release(&(sem_array[sem].lock));
  return SUCCESS_CODE;
}

uint64
sem_find_free_channel(void)
{
  bool index_found = false;
  int index = 100000;
  for (int i = 0; i < MAX_SEM && !index_found; i++){
    if (sem_array[i].is_active){
        // skip
    }
    if (!sem_array[i].is_active){

      // Adquiero el lock del semaforo sem
      // acquire(&sem_array[i].lock);

      // ----------- ZONA CRITICA -----------

      sem_open(i,0);
      sem_array[i].is_active = true;
      index = i;
      index_found = true;


      // -------- FIN ZONA CRITICA -----------
      
      // Devuelvo el lock del semaforo sem
      // release(&sem_array[i].lock);
    }
  }

  return index_found ? index : ERROR_CODE;
}

