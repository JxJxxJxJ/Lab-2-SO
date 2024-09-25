#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// PASO 4: La systemcall sys_sem_[open,close,up,down] lo unico que hace
// es ejecutar sem_[open,close,up,down] con los parametros correctos
// uso argint para pasarle los argumentos a la syscall, todas las de arriba lo hacen
// no se por que
// por que no puedo 
// int sem = 0
// int value = 1 
// ?
uint64 sys_sem_open(void) {
  int sem, value;
  argint(0, &sem);
  argint(1, &value);
  // printf("Ejecutando systemcall (sys_sem_open)\n");
  return sem_open(sem, value);
}

uint64 sys_sem_close(void) {
  int sem;
  argint(0, &sem);
  // printf("Ejecutando systemcall (sys_sem_close)\n");
  return sem_close(sem);
}

uint64 sys_sem_up(void) {
  int sem;
  argint(0, &sem);
  // printf("Ejecutando systemcall (sys_sem_up)\n");
  return sem_up(sem);
}

uint64 sys_sem_down(void) {
  int sem;
  argint(0, &sem);
  // printf("Ejecutando systemcall (sys_sem_down)\n");
  return sem_down(sem);
}

uint64 sys_sem_find_free_channel(void) {
  return sem_find_free_channel();
}

uint64 sys_sem_init_array(void) {
  return sem_init_array();
}

