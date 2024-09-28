#include "kernel/types.h"
#include "user/user.h"
#include "kernel/semaphore.h"

#define ERROR_CODE 0
#define SUCCESS_CODE 1

// Si sem_ping es positivo imprime "ping", luego libera
// al semáforo sem_pong para que continúe con el siguiente 
// proceso.
void 
ping(int rounds, int sem_ping, int sem_pong) 
{
    for (int i = 0; i < rounds; i++) {
        sem_down(sem_ping); 
        printf("Ping\n");
        sem_up(sem_pong); 
    }
}


// Si sem_pong es positivo imprime "pong", luego libera
// al semáforo sem_ping para que continúe con el siguiente 
// proceso.
void 
pong(int rounds, int sem_ping, int sem_pong) 
{
    for (int i = 0; i < rounds; i++) {
        sem_down(sem_pong); 
        printf("          Pong\n");
        sem_up(sem_ping); 
    }
}


// La idea general de pingpong consiste en la utilización 
// de dos semáforos: uno para controlar el proceso que imprime 
// "ping" y otro para controlar el proceso que imprime "pong". 
// Al inicializarlos uno toma el valor de 1 y el otro de 0, 
// haciendo que luego mediante las syscalls sem_down y sem_up 
// se vaya alternando la ejecución de ambos procesos de manera 
// coordinada.
int 
main(int argc, char *argv[]) 
{
    if (argc == 1){
      printf("ERROR: Se necesita un argumento para el número de rondas.\n");
      return ERROR_CODE;
    }

    int rounds = atoi(argv[1]);

    if (rounds <= 0){
      printf("ERROR: Se necesita un argumento mayor a 0 para el número de rondas.\n");
      return ERROR_CODE;
    }

    // Inicialización de los semáforos.
    int sem_ping = sem_find_free_channel();
    int sem_pong = sem_find_free_channel();

    sem_open(sem_ping, 1); // Como sem_ping comienza en 1,
                           // empieza primero.
    sem_open(sem_pong, 0);

    // Nacimiento de los dos procesos.
    int rc = fork();

    if (rc == -1) {
        printf("ERROR: fork() ha fallado.\n");
        return ERROR_CODE;
    }
    if (rc == 0) {    // Proceso hijo, que imprime "ping".
        ping(rounds, sem_ping, sem_pong);
        sem_down(sem_ping);  // Ping() es el que primero termina,
                             // debe esperar a la ultima señal 
                             // que genera pong() para retornar.
    }
    if (rc > 0) {     // Proceso padre, que imprime "pong".
        pong(rounds, sem_ping, sem_pong);
    }

    // Se cierran los semáforos para liberar recursos.
    sem_close(sem_ping);
    sem_close(sem_pong);

    return SUCCESS_CODE;
}