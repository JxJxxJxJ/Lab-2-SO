#ifndef SEMAPHORE_H
#define SEMAPHORE_H

/*
    Me da uint64
*/
#include "types.h"  
/*
    Cantidad máxima de semáforos
    disponibles.
*/
#define MAX_SEM            500

/*
    Optamos por tomar al valor
    -1 para indicar que el semáforo
    está cerrado.
*/
#define CLOSED_SEM_VALUE    -1

/*
    POLITICAS:

    Fue necesario especificar ciertos comportamientos de nuestra librería. 
    Es decir, definir las políticas de nuestro sistema de semáforos:
    -- No se puede decrementar o incrementar el valor de un semáforo inactivo.
    -- No se puede abrir un semáforo que ya esta abierto.
    -- No se puede cerrar un semáforo que ya esta cerrado.
    -- No se puede abrir un semáforo y setearlo con un valor negativo.
    -- Solo son accesibles los semáforos cuyo id esta entre [0,MAX_SEM). 
*/

#define ERROR_CODE           0
#define SUCCESS_CODE         1

/*
    PRECONDICION: 0 <= sem <= MAX_SEM && 0 < value 
    DESCRIPCION: Abre el semáforo “sem” con un valor arbitrario “value”. Si "sem" ya estaba abierto no modifica nada y retorna ERROR_CODE.
    POSTCONDICION: Si rc = sem_open(sem, value) entonces:
                    Si "sem" estaba cerrado -> rc = SUCCESS_CODE y semaforo sem tiene valor = value
                    Si "sem" estaba abierto -> rc = ERROR_CODE y no modifica nada
*/
uint64    sem_open(int sem, int value);

/*
    PRECONDICION: 0 <= sem <= MAX_SEM 
    DESCRIPCION: Despierta los procesos pendientes del semaforo "sem".
                 Cierra el semáforo “sem”. Si "sem" ya estaba cerrado no modifica nada y retorna ERROR_CODE. 
    POSTCONDICION: Si rc = sem_close(sem) entonces:
                    Si "sem" estaba abierto -> rc = SUCCESS_CODE 
                    Si "sem" estaba cerrado -> rc = ERROR_CODE y no modifica nada
*/
uint64    sem_close(int sem);

/*
    PRECONDICION: 0 <= sem <= MAX_SEM && semaforo "sem" tiene value i.
    DESCRIPCION: Incrementa el semáforo ”sem” desbloqueando los procesos cuando su valor i = 0.
    POSTCONDICION: semaforo "sem" tiene value i+1 
                   && procesos pendientes del semaforo "sem" fueron despertados si el i = 0.
*/
uint64    sem_up(int sem);

/* 
    PRECONDICION: 0 <= sem <= MAX_SEM && semaforo "sem" tiene value i
    DESCRIPCION: Decrementa el semáforo ”sem” bloqueando los procesos cuando su valor i = 0.
    POSTCONDICION: Semaforo "sem" tiene value i-1 
                   && si i = 0 entonces el proceso se encuentra bloqueado, esperando al canal "sem".
*/
uint64    sem_down(int sem);

/*
    PRECONDICION: 0 <= value
    DESCRIPCION: Busca el primer semáforo inactivo en el arreglo de semáforos `sem_array`. Al hallarlo, abre ese semáforo
                 e inicializa su valor en value.
    POSTCONDICION: Si i = sem_find_free_channel_and_set(value) entonces:
                    Si habia un canal libre, 
                        i = numero del semaforo libre 
                        value = valor del semaforo
                    Si no habia un canal libre:
                        i = CLOSED_SEM_VALUE
*/
uint64    sem_find_free_channel_and_set(int value);

#endif 
