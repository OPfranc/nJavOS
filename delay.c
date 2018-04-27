/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "types.h"
#include "delay.h"

// Variáveis globais
extern ready_Queue_t Ready_Queue;

void delay_remove()
{
    int i;
    for (i = 0; i < Ready_Queue.tasks_installed; i++)
    {
        if (Ready_Queue.task_READY[i].task_state == WAITING)
        {            
            Ready_Queue.task_READY[i].time_to_delay = Ready_Queue.task_READY[i].time_to_delay - 1;
            if (Ready_Queue.task_READY[i].time_to_delay <= 0) 
            {
                Ready_Queue.tasks_ready++;
                Ready_Queue.task_READY[i].task_state = READY;
                Ready_Queue.task_READY[i].time_to_delay = 0;
            }
        }
    }
}