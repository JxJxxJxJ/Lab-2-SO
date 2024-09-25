#include "kernel/types.h"
#include "user/user.h"

int 
main (void)
{
  printf("Se ejecuta pingpong.c \n");
  sem_open(0, 1);  
  sem_close(0);  
  sem_up(0);  
  sem_down(0);  

  return 0;
}
