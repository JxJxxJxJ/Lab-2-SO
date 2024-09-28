#ifndef SEMAPHORE_H
#define SEMAPHORE_H

/*
    Me da uint64
*/
#include "types.h"  

/*
    Aca se va a definir la politica
    
    Algunas cosas a decir:
    1- Las funciones fallan silenciosamente, esto es: 
    NO usan printf, solo imprimen su codigo de error (0),
    esto es una mala decision de diseño, algo mejor 
    seria usar multiples codigos de error, pero no nos dejan.
    
    2- Que sucede cuando sem_open abre un sem ya abierto.
    
    3- Que sucede cuando sem_close cierra un sem ya cerrado.
*/

#define ERROR_CODE           0
#define SUCCESS_CODE         1

/*
    PRECONDICION:
    DESCRIPCION:
    POSTCONDICION:
*/
uint64    sem_open(int sem, int value);

/*
    PRECONDICION:
    DESCRIPCION:
    POSTCONDICION:
*/
uint64    sem_close(int sem);

/*
    PRECONDICION:
    DESCRIPCION:
    POSTCONDICION:
*/
uint64    sem_up(int sem);

/*
    PRECONDICION:
    DESCRIPCION:
    POSTCONDICION:
*/
uint64    sem_down(int sem);

/*
    PRECONDICION:
    DESCRIPCION:
    POSTCONDICION:
*/
uint64    sem_find_free_channel_and_set(int value);

#endif 
