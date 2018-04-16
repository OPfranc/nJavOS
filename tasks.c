/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "tasks.h"
#include "semaphore.h"

extern sem_t controle;

TASK task_one()
{
  while (1) {
    sem_wait(&controle);
    
    // Faz alguma coisa
    
    sem_post(&controle);
  }
}

TASK task_two()
{
  while (1) {
    sem_wait(&controle);
    
    // Faz alguma coisa
    
    sem_post(&controle);    
  }
}

