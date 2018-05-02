#include "scheduler.h"
#include "kernel.h"

extern Queue_t Queue;

u_int rr_scheduler()
{   
    u_int n_task = Queue.task_running;
    if(Queue.tasks_ready > 0)
    {
        do
        {
            if(n_task == IDLE) n_task = 0;
            else n_task = ((n_task + 1) % Queue.tasks_installed);
        } while(Queue.task_READY[n_task].task_state != READY);
    }
    else 
    {
       n_task = IDLE; 
    }   
    return n_task;
}

u_int priority_scheduler()
{
    u_int i, n_task = IDLE;
    if(Queue.tasks_ready > 0)
    {        
        for(i = 0; i < Queue.tasks_installed; i++)
        {
            if(Queue.task_READY[i].task_state == READY && Queue.task_READY[i].task_prior < Queue.task_READY[n_task].task_prior) n_task = i;
        }
    }  
    return n_task;
}
