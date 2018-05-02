/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "pipe.h"
#include "kernel.h"

extern Queue_t Queue;

void pipe_create(pipe_t * pipe)
{
    pipe->read_pos    = 0;
    pipe->write_pos   = 0;
    pipe->pipe_count  = 0;
}

void pipe_read(pipe_t * pipe, char * msg, u_int pos)
{
    DISABLE_GLOBAL_INTERRUPTS();
    pipe->pos_task_read = pos;
    if ((pipe->read_pos != pipe->write_pos) || (pipe->pipe_count == PIPE_SIZE))
    {
        *msg = pipe->pipe_msg[pipe->read_pos];
        pipe->read_pos = (pipe->read_pos + 1) % PIPE_SIZE;
        pipe->pipe_count--;
        // Verificar se tem alguém esperando msg no pipe.
        if (Queue.task_READY[pipe->pos_task_write].task_state == WAITING) 
        {
            Queue.task_READY[Queue.task_running].blocked--;
            if(Queue.task_READY[pipe->pos_task_write].blocked <= 0 && Queue.task_READY[pipe->pos_task_write].time_to_delay <= 0)
            {
                Queue.tasks_ready++;
                Queue.task_READY[pipe->pos_task_write].task_state = READY;
                dispatcher(READY);
            }
        }
    }
    else 
    {
        Queue.tasks_ready--;
        Queue.task_READY[Queue.task_running].blocked++;
        dispatcher(WAITING);
    }
    ENABLE_GLOBAL_INTERRUPTS();
}

void pipe_write(pipe_t * pipe, char msg, u_int pos)
{
    DISABLE_GLOBAL_INTERRUPTS();
    pipe->pos_task_write = pos;
    if (pipe->pipe_count < PIPE_SIZE) 
    {
        pipe->pipe_msg[pipe->write_pos] = msg;
        pipe->write_pos = (pipe->write_pos + 1) % PIPE_SIZE;
        pipe->pipe_count++;
        // Verificar se tem alguém esperando msg no pipe.
        if (Queue.task_READY[pipe->pos_task_read].task_state == WAITING) 
        {
            Queue.task_READY[Queue.task_running].blocked--;
            if(Queue.task_READY[pipe->pos_task_read].blocked <= 0 && Queue.task_READY[pipe->pos_task_read].time_to_delay <= 0)
            {
                Queue.tasks_ready++;
                Queue.task_READY[pipe->pos_task_read].task_state = READY;
                dispatcher(READY);
            }
        }
    }
    else 
    {
        Queue.tasks_ready--;
        Queue.task_READY[Queue.task_running].blocked++;
        dispatcher(WAITING);
    }
    ENABLE_GLOBAL_INTERRUPTS();
}
       
        