#include "semaphore.h"
#define CHANNEL_SIZE 5
int arreglo_de_semaforos[CHANNEL_SIZE];

// Un sem tiene valores >= 0

struct semaphore_s {
  int value;
};

// Abre y/o inicializa el semáforo “sem” con un valor arbitrario “value”.
int sem_open(semaphore sem, int value);

// Libera el semáforo “sem”.
int sem_close(semaphore sem);

// Incrementa el semáforo ”sem” desbloqueando los procesos
// cuando su valor es 0.
int sem_up(semaphore sem);

// Decrementa el semáforo ”sem”
// bloqueando los procesos cuando su valor es 0.
// El valor del semaforo nunca puede ser menor a 0
int sem_down(semaphore sem);
