#include "pipe.h"
#include "kernel.h"

extern Queue_t * Queue;

void pipe_create(pipe_t * pipe)
{
    pipe->task_read = NULL;    
    pipe->task_write = NULL;    
    pipe->read_pos = 0;
    pipe->write_pos = 0;
    pipe->v = 0;
}

void pipe_write(pipe_t * pipe, char msg)
{
    DISABLE_GLOBAL_INTERRUPTS();
    
    if(pipe->task_write == NULL)
        pipe->task_write = Queue->running;
        
    if (pipe->v < PIPE_SIZE) 
    {
        pipe->pipe_msg[pipe->write_pos] = msg;
        pipe->write_pos = (pipe->write_pos + 1) % PIPE_SIZE;
        pipe->v++;
        if(pipe->task_read != NULL && pipe->task_read->task_state == WAITING) 
        {
            pipe->task_read->blocked--;
            if(pipe->task_read->blocked <= 0 && pipe->task_read->time_to_delay <= 0)
            {
                Queue->tasks_ready++;
                pipe->task_read->task_state = READY;
            }
        }
    }
    else 
    {
        Queue->tasks_ready--;
        Queue->running->blocked++;
        dispatcher(WAITING);
    }
    ENABLE_GLOBAL_INTERRUPTS();
}

void pipe_read(pipe_t * pipe, char * msg)
{
    DISABLE_GLOBAL_INTERRUPTS();    
    
    if(pipe->task_read == NULL)
        pipe->task_read = Queue->running;
    
    if (pipe->v > 0)
    {
        pipe->v--;
        *msg = pipe->pipe_msg[pipe->read_pos];
        pipe->read_pos = (pipe->read_pos + 1) % PIPE_SIZE;
        if(pipe->task_write != NULL && pipe->task_write->task_state == WAITING) 
        {
            pipe->task_write->blocked--;
            if(pipe->task_write->blocked <= 0 && pipe->task_write->time_to_delay <= 0)
            {
                Queue->tasks_ready++;
                pipe->task_write->task_state = READY;
            }
        }
    }
    else 
    {
        Queue->tasks_ready--;
        Queue->running->blocked++;
        dispatcher(WAITING);
    }
    ENABLE_GLOBAL_INTERRUPTS();
}