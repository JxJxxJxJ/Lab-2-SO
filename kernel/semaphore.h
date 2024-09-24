#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef struct semaphore_s *semaphore;

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

#endif
