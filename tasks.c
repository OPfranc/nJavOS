/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <pic18f4520.h>

#include "tasks.h"
#include "semaphore.h"
#include "kernel.h"

extern sem_t controle;

void config_user()
{
  // Configurações de usuário que dizem respeito as tarefas
  TRISDbits.RD0 = 0;
  PORTDbits.RD0 = 0;
  TRISDbits.RD1 = 0;
  PORTDbits.RD1 = 0;
  
  // Configura tarefa IDLE
  TRISDbits.RD2 = 0;
  PORTDbits.RD2 = 0;


  TRISDbits.RD3 = 0;
  PORTDbits.RD3 = 0;
  
  TRISDbits.RD4 = 0;
  PORTDbits.RD4 = 0;

  TRISDbits.RD5 = 0;
  PORTDbits.RD5 = 0;
}

TASK task_one()
{
    while(1)
    {
        PORTDbits.RD0 = ~PORTDbits.RD0;
        PORTDbits.RD3 = 1;
        PORTDbits.RD4 = 0;
        PORTDbits.RD5 = 0;
        nJavOS_task_delay(10);
    }
}

TASK task_two()
{
    while(1)
    {
        PORTDbits.RD1 = ~PORTDbits.RD1;
        PORTDbits.RD3 = 0;
        PORTDbits.RD4 = 1;
        PORTDbits.RD5 = 0;
        nJavOS_task_delay(100);
    }
}

TASK task_three()
{
    PORTDbits.RD3 = 1;
    PORTDbits.RD4 = 1;
    PORTDbits.RD5 = 0;
}

