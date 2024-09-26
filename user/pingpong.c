#include "kernel/types.h"
#include "user/user.h"
#include "kernel/semaphore.h"

#define ERROR_CODE 0
#define SUCCESS_CODE 1

// void ping(int rondas, int sem_number) {
//     for (int i = 0; i < rondas; i++) {
//         sem_down(sem_number); 
//         printf("Ping\n");
//         sleep(1);
//         sem_up(sem_number); 
//     }
// }

// void pong(int rondas, int sem_number) {
//     for (int i = 0; i < rondas; i++) {
//         sem_down(sem_number); 
//         printf("          Pong\n");
//         sleep(1);
//         sem_up(sem_number); 
//     }
// }

// int main(int argc, char *argv[]) {
//     if (argc == 1){
//       printf("ERROR: Se necesita un argumento para el numero de rondas.\n");
//       return ERROR_CODE;
//     }

//     int rondas = atoi(argv[1]);

//     if (rondas <= 0){
//       printf("ERROR: Se necesita un argumento >= 0 para el numero de rondas\n");
//       return ERROR_CODE;
//     }

//     int sem_number = sem_find_free_channel();
//     sem_open(sem_number, 1); 

//     int rc = fork();
//     if (rc == 0) {
//         pong(rondas,sem_number); 
//         //sem_up(sem_number);
//     }
//     if (rc > 0) {
//         ping(rondas,sem_number); 
//         //sem_down(sem_number);
//     }

//     sleep(1);
//     sem_close(sem_number);
//     return 1;
// }

void ping(int rondas, int sem_ping, int sem_pong) {
    for (int i = 0; i < rondas; i++) {
        sem_down(sem_ping); 
        printf("Ping\n");
        sleep(1);
        sem_up(sem_pong); 
    }
}

void pong(int rondas, int sem_ping, int sem_pong) {
    for (int i = 0; i < rondas; i++) {
        sem_down(sem_pong); 
        printf("          Pong\n");
        sleep(1);
        sem_up(sem_ping); 
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1){
      printf("ERROR: Se necesita un argumento para el numero de rondas.\n");
      return ERROR_CODE;
    }

    int rondas = atoi(argv[1]);

    if (rondas <= 0){
      printf("ERROR: Se necesita un argumento >= 0 para el numero de rondas.\n");
      return ERROR_CODE;
    }

    int sem_ping = sem_find_free_channel();
    int sem_pong = sem_find_free_channel();

    sem_open(sem_ping, 1); 
    sem_open(sem_pong, 0);

    int rc = fork();

    if (rc == -1) {
        printf("ERROR: fork() ha fallado.\n");
        return ERROR_CODE;
    }
    if (rc == 0) {  // Proceso hijo (pong)
        pong(rondas, sem_ping, sem_pong); 
    }
    else {          // Proceso padre (ping)
        ping(rondas, sem_ping, sem_pong); 
        wait(0); 
    }

    sem_close(sem_ping);
    sem_close(sem_pong);
    return SUCCESS_CODE;
}