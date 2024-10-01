#include "kernel/types.h"
#include "user/user.h"
#include "kernel/semaphore.h"
#include "stdbool.h"

#define ERROR_CODE 0
#define SUCCESS_CODE 1

// #define FUN false  // Si quiero pasar los tests
#define FUN true   // Si quiero pasarla bien

/*
    Para FUN. Printea ronda actual, hace un pequeño delay
    y borra la terminal para pasar al siguiente frame.
 */
void
print_round_wait_and_erase(int i) 
{
    printf("Ronda: %d\n",i+1); // i+1 porque cuento desde la ronda 1
    
    int ticks_to_wait = 2;
    int start_time = uptime();
    // Loopeo hasta que hayan pasado ticks_to_wait ticks desde start_time
    while ((uptime() - start_time) < ticks_to_wait) { 
        // skip
    }
    printf("\033[H\033[J"); // https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797#erase-functions
}

/*
    Si sem_ping es positivo imprime "ping", luego libera
    al semáforo sem_pong para que continúe con el siguiente
    proceso.
*/
void
ping(int rounds, int sem_ping, int sem_pong)
{
    for (int i = 0; i < rounds; i++) {
        if (!FUN) { // Pasa todos los tests
            sem_down(sem_ping);
            printf("Ping\n");
            sem_up(sem_pong);
        }
        if (FUN) { // No pasa los tests por tener caracteres raros :(
            sem_down(sem_ping);
            printf("\033[H\033[J"); // https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797#erase-functions
            printf("Ping\t🏓\n");
            print_round_wait_and_erase(i);
            printf("    \t🏓\t⚫\t\t\t\t\t🏓\t\n");
            print_round_wait_and_erase(i);
            printf("    \t🏓\t\t⚫\t\t\t\t🏓\t\n");
            print_round_wait_and_erase(i);
            printf("    \t🏓\t\t\t⚫\t\t\t🏓\n");
            print_round_wait_and_erase(i);
            printf("    \t🏓\t\t\t\t⚫\t\t🏓\n");
            print_round_wait_and_erase(i);
            printf("    \t🏓\t\t\t\t\t⚫\t🏓\n");
            print_round_wait_and_erase(i);
            sem_up(sem_pong);
        }
    }
}

/*
    Si sem_pong es positivo imprime "pong", luego libera
    al semáforo sem_ping para que continúe con el siguiente
    proceso.
*/
void
pong(int rounds, int sem_ping, int sem_pong)
{
    for (int i = 0; i < rounds; i++) {
        if (!FUN) { // Pasa todos los tests
            sem_down(sem_pong);
            printf("\t\t\t\t\tPong\n");
            sem_up(sem_ping);
        }
        if (FUN) { // No pasa los tests por tener caracteres raros :(
            sem_down(sem_pong);
            if (i < rounds - 1) { // Ronda intermedia
                printf("    \t🏓\t\t\t\t\t\t🏓\tPong\n");
                print_round_wait_and_erase(i);
                printf("    \t🏓\t\t\t\t\t⚫\t🏓\n");
                print_round_wait_and_erase(i);
                printf("    \t🏓\t\t\t\t⚫\t\t🏓\n");
                print_round_wait_and_erase(i);
                printf("    \t🏓\t\t\t⚫\t\t\t🏓\n");
                print_round_wait_and_erase(i);
                printf("    \t🏓\t\t⚫\t\t\t\t🏓\n");
                print_round_wait_and_erase(i);
                printf("    \t🏓\t⚫\t\t\t\t\t🏓\n");
                print_round_wait_and_erase(i);
            }
            if (i == rounds - 1) { // Ultima ronda
                printf("    \t🏓\t\t\t\t\t\t🏓\tPong\n");
                printf("🎉\n");
            }
            sem_up(sem_ping);
        }
    }
}

/*
    La idea general de pingpong consiste en la utilización
    de dos semáforos: uno para controlar el proceso que imprime
    "ping" y otro para controlar el proceso que imprime "pong".
    Al inicializarlos uno toma el valor de 1 y el otro de 0,
    haciendo que luego mediante las syscalls sem_down y sem_up
    se vaya alternando la ejecución de ambos procesos de manera
    coordinada.
*/
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
    
    int sem_ping = sem_find_free_channel_and_set(1);
    int sem_pong = sem_find_free_channel_and_set(0);
    
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
    
    sem_close(sem_ping);
    sem_close(sem_pong);
    
    return SUCCESS_CODE;
}
