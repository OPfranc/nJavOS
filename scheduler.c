#include "scheduler.h"
#include "kernel.h"

extern ready_Queue_t Ready_Queue;

u_int rr_scheduler()
{
  u_int task_to_running, tasks_ready = Ready_Queue.tasks_installed;
  
  do {
    task_to_running = (Ready_Queue.task_running + 1) % (Ready_Queue.tasks_installed + IDLE);            
    tasks_ready--;
  } while (Ready_Queue.task_READY[task_to_running].task_state != READY && tasks_ready != 0);
  
  if (tasks_ready)  
    return task_to_running;
  
  return IDLE;
}

u_int priority_scheduler()
{

}
