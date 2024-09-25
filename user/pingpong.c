#include "kernel/types.h"
#include "user/user.h"
#include "kernel/semaphore.h"

#define ERROR_CODE 0

void ping(int rondas, int sem_number) {
    for (int i = 0; i < rondas; i++) {
        sem_down(sem_number); 
        printf("Ping\n");
        sem_up(sem_number); 
    }
}

void pong(int rondas, int sem_number) {
    for (int i = 0; i < rondas; i++) {
        sem_down(sem_number); 
        printf("          Pong\n");
        sem_up(sem_number); 
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1){
      printf("ERROR: Se necesita un argumento para el numero de rondas.\n");
      return ERROR_CODE;
    }

    int rondas = atoi(argv[1]);
    if (rondas < 0){
      printf("ERROR: Se necesita un argumento >= 0 para el numero de rondas\n");
      return ERROR_CODE;
    }

    sem_init_array();
    int sem_number = sem_find_free_channel();
    sem_open(sem_number, 1); 


    int rc = fork();
    if (rc == 0) {
        pong(rondas,sem_number); 
        sem_up(sem_number);
    }
    if (rc > 0) {
        ping(rondas,sem_number); 
        sem_down(sem_number);
    }
  return 1;
}
