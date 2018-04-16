/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "semaphore.h"
#include "kernel.h"

extern ready_Queue_t Ready_Queue;

void sem_init(sem_t *s, u_int s_valor)
{
  s->sem_v = s_valor;
  s->sem_bloqued_queue.tasks_bloqued = 0;
  s->sem_bloqued_queue.next_task_to_free = 0;
}

void sem_wait(sem_t *s)
{
  DISABLE_GLOBAL_INTERRUPTS();
  
  u_int pos_task_running = Ready_Queue.task_running;
  
  s->sem_v--;
  if (s->sem_v < 0) {
    s->sem_bloqued_queue.tasks[s->sem_bloqued_queue.tasks_bloqued] = pos_task_running;
    s->sem_bloqued_queue.tasks_bloqued = (s->sem_bloqued_queue.tasks_bloqued+1) % MAX_TASKS;
    nJavOS_dispatcher(WAITING);    
  }
  
  ENABLE_GLOBAL_INTERRUPTS();
}

void sem_post(sem_t *s)
{
  DISABLE_GLOBAL_INTERRUPTS();
  
  u_int task_free_pos;
  
  s->sem_v++;
  if (s->sem_v <= 0) {
    task_free_pos = s->sem_bloqued_queue.next_task_to_free;
    // Voltar a tarefa do estado de WAITING para READY
    Ready_Queue.task_READY[s->sem_bloqued_queue.tasks[task_free_pos]].task_state = READY;
    s->sem_bloqued_queue.next_task_to_free = (s->sem_bloqued_queue.next_task_to_free + 1) % MAX_TASKS;
  }
  
  nJavOS_dispatcher(READY);
  
  ENABLE_GLOBAL_INTERRUPTS();  
}

