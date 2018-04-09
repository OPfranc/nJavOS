#include "scheduler.h"

u_int rr_scheduler()
{
    return (Ready_Queue.task_running + 1) % Ready_Queue.tasks_installed; 
}

u_int priority_scheduler()
{
    u_int the_choose_one;
    int i;   
    the_choose_one = 0;
    for(i = 1; i <= Ready_Queue.tasks_installed; i++)
    {
        if(Ready_Queue.task_READY[the_choose_one].task_prior < Ready_Queue.task_READY[i].task_prior)
            the_choose_one = i;
    }
    return the_choose_one;
}