# Laboratorio 2: "Semáforos en XV6" - Grupo 1 - FaMAF (2024)

Los **integrantes** que desarrollaron este proyecto son: 
 - Juan Cruz Hermosilla Artico
 - Gaspar Saenz Valiente
 - Fernando Cabrera Luque

## **Índice**
 1. [Introducción](#Introducción)
 2. [Dependencias](#Dependencias)
 3. [Compilación y ejecución](#Compilación-y-ejecución)
 4. [Módulos relevantes del proyecto](#Módulos-relevantes-del-proyecto)
 5. [Interfaz extra: FUN mode](#Interfaz-extra-FUN-mode)
 6. [Documentación empleada](#Documentación-empleada)

## **Introducción**
Un semáforo es una estructura de datos que actúa como una señal o un contador para controlar el acceso a un recurso compartido, permitiendo la sincronización y coordinación entre varios procesos o subprocesos que comparten un recurso común o necesitan comunicarse entre sí.
En este proyecto de laboratorio se busca escarbar más profundo dentro del sistema operativo implementando nuestras propias llamadas al sistema (syscalls), en particular unas que gestionan un sistema de semáforos en la versión RISC-V de XV6 en espacio de kernel.

## **Dependencias**
Para poder compilar y ejecutar el proyecto es necesario tener instalado el emulador QEMU, el cual nos permite trabajar amenamente con XV6.
 - QEMU (Ubuntu): `sudo apt-get install qemu-system-riscv64 gcc-riscv64-linux-gnu`
 - Fuente monoespaciada con soporte de caracteres unicode: [`JetBrains Mono`](https://www.jetbrains.com/es-es/lp/mono/)

## **Compilación y ejecución**
Para compilar el proyecto, simplemente ejecutar:
``` sh
git clone https://<user>@bitbucket.org/sistop-famaf/so24lab2g01.git
cd so24lab2g01
make qemu
```
Finalmente, para eliminar todos los archivos ejecutables debe utilizarse el comando:
``` sh
make clean
```

## **Módulos relevantes del proyecto**
- `kernel/semaphore.c`: módulo principal del proyecto, el cual contiene todas las syscalls relacionadas al funcionamiento de los semáforos dentro del espacio de kernel. A su vez expone la estructura del sistema de semáforos y ciertas funciones auxiliares que sirven de soporte al mismo. 

- `kernel/semaphore.h`: si bien como todo encabezado (header) contiene los prototipos de las funciones, también se encuentran aquí otras decisiones de implementación como el valor de un semáforo cerrado, cómo se define el error, etc. Radican también las políticas que se dictaminaron en ciertos casos de error así como también las pre y postcondiciones que deben cumplir todas las funciones.

- `user/pingpong.c`: programa de espacio de usuario que sincroniza la escritura por pantalla de la cadena "ping" y "pong" utilizando el sistema de semáforos construido. Tiene dos interfaces, las cuales se detallan más abajo.

## **Interfaz extra: FUN mode**
En `user/pingpong.c` hay un `#define` para obtener dos comportamientos. Uno pasa los tests y el otro no.
Ambos usan semáforos, solo cambian unos `printf()`.
### #define FUN false
![image](https://github.com/user-attachments/assets/2e3b6c8f-a248-4c46-81d5-cd8fb08c979c)
### #define FUN true
[![asciicast](https://asciinema.org/a/678305.svg)](https://asciinema.org/a/678305)

## **Documentación empleada para el desarrollo del proyecto**
- Remzi H. Arpaci-Dusseau y Andrea C. Arpaci-Dusseau (2023). [*Operating Systems: Three Easy Pieces*](https://pages.cs.wisc.edu/~remzi/OSTEP/). Los capítulos sobre concurrencia fueron de gran ayuda para el desarrollo del laboratorio, en particular el apartado sobre semáforos (capítulo 31).
- Russ Cox, Frans Kaashoek y Robert Morris (2022). [*xv6: a simple, Unix-like teaching operating system*](https://pdos.csail.mit.edu/6.828/2022/xv6/book-riscv-rev3.pdf).
- Allan B. Downey (2016). [*The Little Book of Semaphores*](https://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf).
- [*xv6-explained*](https://github.com/YehudaShapira/xv6-explained). Repositorio GitHub en donde se hacen explicaciones del funcionamiento de XV6.