#include "pipe.h"
#include "kernel.h"

extern Queue_t * Queue;

pipe_t * pipe_create()
{
    pipe_t *pipe;
    
    pipe = SRAMalloc(sizeof(pipe_t));
    
    pipe->count = PIPE_SIZE;       
    pipe->head = NULL;
    pipe->tail = NULL;
    pipe->blocked_task = NULL;    
    
    return pipe;
}

void pipe_write(pipe_t * pipe, char msg[])
{
    DISABLE_GLOBAL_INTERRUPTS();
    if(pipe->count > 0){
        message_t * m, * t;
        m = SRAMalloc(sizeof(message_t));
        m->msg = msg;
        m->next = NULL;
        if(pipe->head == NULL){
            pipe->head = m;
        } else
        {
            t = pipe->tail;
            t->next = m;
        }
        pipe->tail = m;
        pipe->count--;
        free_pipe(pipe);
    } else
    {
        block_pipe(pipe);
    }
    ENABLE_GLOBAL_INTERRUPTS();
}

void block_pipe(pipe_t * pipe)
{
    pipe->blocked_task = Queue->running;
    Queue->running->blocked++;
    Queue->tasks_ready--;
    dispatcher(WAITING);
}

void free_pipe(pipe_t * pipe)
{
    if(pipe->blocked_task != NULL)
    {
        pipe->blocked_task->blocked--;
        if(pipe->blocked_task->blocked <= 0 && pipe->blocked_task->time_to_delay <= 0)
        {
            Queue->tasks_ready++;
            pipe->blocked_task->blocked = 0;
            pipe->blocked_task->time_to_delay = 0;
            pipe->blocked_task->task_state = READY;
        }
        pipe->blocked_task = NULL;
    }
}

char * pipe_read(pipe_t * pipe)
{
    DISABLE_GLOBAL_INTERRUPTS();
    char * msg;
    if(pipe->count == PIPE_SIZE)
    {
        block_pipe(pipe);
    }
    else
    {
        message_t * m;        
        m = pipe->head;
        pipe->head = m->next;
        msg = m->msg;
        SRAMfree(m);
        pipe->count++;
        free_pipe(pipe);
    }
    ENABLE_GLOBAL_INTERRUPTS();
    return msg;
}