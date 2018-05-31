#include "types.h"
#include "delay.h"
#include "kernel.h"

extern Queue_t * Queue;

void delay_remove()
{
    int i;
    tcb_t * t;
    t = Queue->head;

    for (i = 0; i < Queue->tasks_installed; i++)
    {
        if (t->task_state == WAITING)
        {
            if(t->time_to_delay > 0) t->time_to_delay--;
            if ((t->blocked <= 0) && (t->time_to_delay <= 0)) 
            {
                Queue->tasks_ready++;
                t->task_state = READY;
                t->time_to_delay = 0;
                t->blocked = 0;
                if(!RR_SCHEDULER) dispatcher(READY);
            }
        }
        t = t->next;
    }
}