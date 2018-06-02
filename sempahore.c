#include "semaphore.h"
#include "kernel.h"

extern Queue_t * Queue;

sem_t * sem_init(u_int s_valor)
{
    sem_t * s;
    s = SRAMalloc(sizeof(sem_t));
    s->sem_v = s_valor;
    s->sem_bloqued_queue_head = NULL;
    s->sem_bloqued_queue_tail = NULL;
    return s;
}

void sem_wait(sem_t *s)
{
    DISABLE_GLOBAL_INTERRUPTS();
    s->sem_v--;
    if (s->sem_v < 0) 
    {
        sem_queue_t * bloqued;
        bloqued = SRAMalloc(sizeof(sem_queue_t));
        
        bloqued->task = Queue->running;
        bloqued->next = NULL;
        
        if(s->sem_bloqued_queue_head == NULL)
        {
            s->sem_bloqued_queue_head = bloqued;
            s->sem_bloqued_queue_tail = bloqued;
        } 
        else
        {
            s->sem_bloqued_queue_tail->next = bloqued;
            s->sem_bloqued_queue_tail = bloqued;
        }
        
        Queue->running->blocked++;
        Queue->tasks_ready--;
        
        dispatcher(WAITING);
    }
    ENABLE_GLOBAL_INTERRUPTS();
}

void sem_post(sem_t *s)
{
    DISABLE_GLOBAL_INTERRUPTS();
    int v = s->sem_v;
    v++;
    if(v <= 0){
        sem_queue_t * bloqued;
        bloqued = s->sem_bloqued_queue_head;
        if(bloqued->task->task_state == WAITING)
        {
            bloqued->task->blocked--;
            if(bloqued->task->blocked <= 0 && bloqued->task->time_to_delay <= 0){
                Queue->tasks_ready++;
                bloqued->task->blocked = 0;
                bloqued->task->time_to_delay = 0;
                bloqued->task->task_state = READY;
            }
        }
        s->sem_bloqued_queue_head = bloqued->next;
        SRAMfree(bloqued);
    }
    s->sem_v = v;
    ENABLE_GLOBAL_INTERRUPTS();  
}

