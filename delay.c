/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "types.h"
#include "delay.h"
#include "kernel.h"

// Variáveis globais
extern Queue_t Queue;

void delay_remove()
{
    int i;
    for (i = 0; i < (int)Queue.tasks_installed; i++)
    {
        if (Queue.task_READY[i].task_state == WAITING)
        {
            if(Queue.task_READY[i].time_to_delay > 0) Queue.task_READY[i].time_to_delay--;
            if ((Queue.task_READY[i].blocked <= 0) && (Queue.task_READY[i].time_to_delay <= 0)) 
            {
                Queue.tasks_ready++;
                Queue.task_READY[i].task_state = READY;   
                Queue.task_READY[i].time_to_delay = 0;
                Queue.task_READY[i].blocked = 0;
                if(!RR_SCHEDULER) dispatcher(READY);
            }
        }
    }
}