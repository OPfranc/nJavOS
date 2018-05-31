#include "scheduler.h"
#include "kernel.h"

extern Queue_t * Queue;

void rr_scheduler()
{   
    tcb_t * t;
    t = Queue->running;
    if(Queue->tasks_ready > 0)
    {
        do
        {
            t = t->next;
        } while(t->task_state != READY);
    }
    else 
    {
       t = Queue->idle; 
    }   
    Queue->running = t;
}

void priority_scheduler()
{
    int i;
    tcb_t * t, * c;
    
    c = Queue->idle;
    t = Queue->head;
    
    for (i = 0; i < Queue->tasks_installed; i++)
    {
        if (t->task_state == READY && t->task_prior < c->task_prior) c = t;
        t = t->next;
    }
    Queue->running = c;
}
