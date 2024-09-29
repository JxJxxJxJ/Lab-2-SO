#include "semaphore.h"
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "stdbool.h"

/*
    Todas estas syscalls realmente están modificando
    una entrada en la SEM_ARRAY. Es importante que, como
    RISC-V es multicore, esta modificación se haga de forma
    atómica. XV6's locks enter the game.
    
    Voy a usar locks para proteger una sección crítica, que
    va a ser cuando accedo a las entries del arreglo de semá-
    foros. Se dice que el lock va a proteger a la entry del
    arreglo.
*/

/*
    Estructura del semáforo.
*/
typedef struct sem_s {
  struct spinlock lock;
  int value;
  bool is_active;
} semaphore;

/*
    Se implementan a los semáforos como un arreglo dentro
    del espacio de kernel.
*/
/*
    INVARIANTE (*): El usuario solo puede acceder a los semaforos 
    entre los indices [0] y [MAX_SEM]. El ultimo semaforo se usa
    para sincronizar a la funcion sem_find_free_channel_and_set()
    cuando muchos procesos la llaman al mismo tiempo.
*/
semaphore sem_array[MAX_SEM+1];

/*
    Inicializa el arreglo global de semáforos `sem_array`,
    seteando todos los valores en CLOSED_SEM_VALUE (-1).
*/
uint64
sem_init_array(void)
{
  for (int i = 0; i < MAX_SEM; i++){
    sem_array[i].value = CLOSED_SEM_VALUE;
    sem_array[i].is_active = false;
  }
  return SUCCESS_CODE;
}

/*
    Busca el primer semáforo inactivo en el arreglo de
    semáforos `sem_array`. Al hallarlo, abre ese semáforo
    e inicializa su valor en value.
*/
uint64
sem_find_free_channel_and_set(int value)
{
  // Adquiero el lock especializado
  acquire(&sem_array[MAX_SEM].lock);

  // ----------- ZONA CRITICA -----------
  bool index_found = false;
  int index = CLOSED_SEM_VALUE;
  for (int i = 0; i < MAX_SEM && !index_found; i++){
    if (sem_array[i].is_active){
        // skip
    }
    if (!sem_array[i].is_active){
      sem_open(i,value);
      index = i;
      index_found = true;
    }
  }
  // -------- FIN ZONA CRITICA -----------
  
  // Devuelvo el lock especializado
  release(&sem_array[MAX_SEM].lock);
  return index;
}

/*
    Abre un semáforo dado su índice `sem` y lo inicializa
    con el valor `value` proporcionado.
*/
uint64
sem_open(int sem, int value)
{
  // Adquiero el lock del semáforo sem.
  acquire(&(sem_array[sem].lock));
  bool error = false;

  // ------------------ ZONA CRITICA ------------------
  if (sem < 0 || sem >= MAX_SEM){ // Si estoy fuera de los indices
    error = true;
  }
  if (0 <= sem && sem < MAX_SEM){ // Si estoy dentro de los indices
    if(value < 0  || sem_array[sem].is_active){  // Pero le doy un valor negativo o el semaforo esta activado
      error = true;
    }
    if (!error){                                 // Si todo esta bien
      sem_array[sem].value = value;
      sem_array[sem].is_active = true;
    }
  }
  // ---------------- FIN ZONA CRITICA ----------------

  // Devuelvo el lock del semáforo sem.
  release(&(sem_array[sem].lock));
  return error ? ERROR_CODE : SUCCESS_CODE;
}

/*
    Libera el semáforo en la posición `sem` del arreglo
    `sem_array`.
*/
uint64
sem_close(int sem)
{
  // Adquiero el lock del semáforo sem.
  acquire(&(sem_array[sem].lock));
  bool error = false;

  // ------------------ ZONA CRITICA ------------------
  if (sem < 0 || sem >= MAX_SEM){ // Si estoy fuera de los indices
    error = true;
  }
  if (0 <= sem && sem < MAX_SEM){ // Si estoy dentro de los indices
    if(!sem_array[sem].is_active){               // Pero el semaforo no esta activado
      error = true;
    }
  }
  if (!error){                                   // Y todo esta bien
    wakeup(&sem_array[sem]); // <-- Para que los procesos esperando a
                             // sem tengan la oportunidad de hacer algo
                             // y no se queden colgados.
    sem_array[sem].value = CLOSED_SEM_VALUE;
    sem_array[sem].is_active = false;
  }
  // ---------------- FIN ZONA CRITICA ----------------

  // Devuelvo el lock del semáforo sem.
  release(&(sem_array[sem].lock));
  return error ? ERROR_CODE : SUCCESS_CODE;
}

/*
    Incrementa el valor del semáforo en la posición `sem`
    del arreglo `sem_array`. Si el valor del semáforo es 0,
    desbloquea los procesos.
*/
uint64
sem_up(int sem)
{
  // Adquiero el lock del semáforo sem.
  acquire(&(sem_array[sem].lock));
  bool error = false;

  // ------------------ ZONA CRITICA ------------------
  // Si sem no es un indice valido o si el semaforo no esta activo.
  if (sem < 0 || sem >= MAX_SEM || !sem_array[sem].is_active){
    error = true;
  }
  if (!error){
    bool sem_was_zero = 0 == sem_array[sem].value;
    sem_array[sem].value++;
    if(sem_was_zero){
      wakeup(&(sem_array[sem]));
    }
  }
  // ---------------- FIN ZONA CRITICA ----------------

  // Devuelvo el lock del semáforo sem.
  release(&(sem_array[sem].lock));
  return error ? ERROR_CODE : SUCCESS_CODE;

}

/*
     Decrementa el semáforo ”sem” bloqueando los procesos
     cuando su valor es 0. El valor del semáforo nunca puede
     ser menor a 0.
*/
uint64
sem_down(int sem)
{
  // Adquiero el lock del semáforo sem.
  acquire(&(sem_array[sem].lock));
  bool error = false;

  // ------------------ ZONA CRITICA ------------------
  // Si sem no es un indice valido o si el semaforo no esta activo.
  if (sem < 0 || sem >= MAX_SEM || !sem_array[sem].is_active){
    error = true;
  }
  if (!error){
    bool sem_was_zero = sem_array[sem].value == 0;
    if (sem_was_zero) {
      sleep(&(sem_array[sem]), &(sem_array[sem].lock));
    }
    sem_array[sem].value--;
  }
  // ---------------- FIN ZONA CRITICA ----------------

  // Devuelvo el lock del semáforo sem.
  release(&(sem_array[sem].lock));
  return error ? ERROR_CODE : SUCCESS_CODE;
}
