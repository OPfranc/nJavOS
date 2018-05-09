/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "semaphore.h"
#include "kernel.h"

/* Declaração da variavel global de fila */
extern Queue_t Queue;

void sem_init(sem_t *s, u_int s_valor, u_int f_valor)
{
    s->sem_v = s_valor;
    s->sem_f = f_valor;
    s->sem_b = 0;
    s->sem_bloqued_queue.tasks_bloqued = 0;
    s->sem_bloqued_queue.next_task_to_free = 0;
}

void sem_wait(sem_t *s)
{
    DISABLE_GLOBAL_INTERRUPTS();
    s->sem_v--;
    if (s->sem_v < 0) 
    {
        s->sem_b = 1;
        Queue.tasks_ready--;
        Queue.task_READY[Queue.task_running].blocked++;
        s->sem_bloqued_queue.tasks[s->sem_bloqued_queue.tasks_bloqued] = Queue.task_running;
        s->sem_bloqued_queue.tasks_bloqued = ((s->sem_bloqued_queue.tasks_bloqued + 1) % MAX_TASKS);
        dispatcher(WAITING);
    }
    ENABLE_GLOBAL_INTERRUPTS();
}

void sem_post(sem_t *s)
{
    DISABLE_GLOBAL_INTERRUPTS();
    u_int task_free_pos;
    int v;
    v = s->sem_v;
    v++;
    if(v > s->sem_f) v = s->sem_f;
    if (v <= 0 && s->sem_b) 
    {
        task_free_pos = s->sem_bloqued_queue.next_task_to_free;
        if(Queue.task_READY[s->sem_bloqued_queue.tasks[task_free_pos]].task_state == WAITING)
        {
            Queue.task_READY[s->sem_bloqued_queue.tasks[task_free_pos]].blocked--;
            if(Queue.task_READY[s->sem_bloqued_queue.tasks[task_free_pos]].blocked <= 0 && Queue.task_READY[s->sem_bloqued_queue.tasks[task_free_pos]].time_to_delay <= 0){
                Queue.tasks_ready++;
                Queue.task_READY[s->sem_bloqued_queue.tasks[task_free_pos]].blocked = 0;
                Queue.task_READY[s->sem_bloqued_queue.tasks[task_free_pos]].time_to_delay = 0;
                Queue.task_READY[s->sem_bloqued_queue.tasks[task_free_pos]].task_state = READY;
            }
            s->sem_bloqued_queue.next_task_to_free = ((s->sem_bloqued_queue.next_task_to_free + 1) % MAX_TASKS);
            dispatcher(READY);
        }
    }
    s->sem_v = v;
    ENABLE_GLOBAL_INTERRUPTS();  
}

