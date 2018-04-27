#include "scheduler.h"
#include "kernel.h"

extern ready_Queue_t Ready_Queue;

u_int rr_scheduler()
{   
    u_int i = 0, n_task = Ready_Queue.task_running;
    if(Ready_Queue.tasks_installed > 0 && Ready_Queue.tasks_ready > 0)
    {
        if(n_task == MAX_TASKS) n_task = 0;
        n_task = ((n_task + 1) % Ready_Queue.tasks_installed);
        while(Ready_Queue.task_READY[n_task].task_state != READY)
        {
            i++;
            n_task = ((n_task + 1) % Ready_Queue.tasks_installed);
            if (i > Ready_Queue.tasks_ready)
            {
                n_task = MAX_TASKS;
                break;
            }
        }
    }
    else 
    {
       n_task = MAX_TASKS; 
    }   
    return n_task;
}

u_int priority_scheduler()
{

}
