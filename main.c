/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <xc.h>
#include "tasks.h"
#include "kernel.h"
#include "config_timer.h"
#include "semaphore.h"

// Variáveis globais
sem_t cheio, vazio;
asm("GLOBAL _task_one, _task_two, _idle");

void main(void) {
  
  // Faz configuração de usuário
  config_user();
  
  nJavOS_start();
  nJavOS_init_timer();
 
  sem_init(&vazio, 1);
  sem_init(&cheio, 0);
  
  // Cria as tarefas
  nJavOS_task_create(1, 10, task_one);
  nJavOS_task_create(2, 10, task_two);
  //nJavOS_task_create(3, 10, task_three);
   
  while (1){
  }

  return;
}
