/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <xc.h>
#include "tasks.h"
#include "kernel.h"
#include "config_timer.h"

void main(void) {
    
    nJavOS_start();  
    nJavOS_init_timer();
    
    // Cria as tarefas
    nJavOS_task_create(1, 10, task_one);
    nJavOS_task_create(1, 10, task_two);
  
    while (1);
    return;
}
