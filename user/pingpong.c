
// Abre y/o inicializa el semáforo “sem” con un valor arbitrario “value”.
int sem_open(int sem, int value);

// Libera el semáforo “sem”.
int sem_close(int sem);

// Incrementa el semáforo ”sem” desbloqueando los procesos
// cuando su valor es 0.
int sem_up(int sem);

// Decrementa el semáforo ”sem”
// bloqueando los procesos cuando su valor es 0.
// El valor del semaforo nunca puede ser menor a 0
int sem_down(int sem);
