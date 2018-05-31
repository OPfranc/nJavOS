#include "pipe.h"
#include "kernel.h"

extern Queue_t * Queue;

void pipe_create(pipe_t * pipe)
{
    pipe->head = NULL;
    pipe->tail = NULL;
    sem_init(&pipe->read, 0);
    sem_init(&pipe->write, PIPE_SIZE);
}

void pipe_write(pipe_t * pipe, char msg)
{
    sem_wait(&pipe->write);
    DISABLE_GLOBAL_INTERRUPTS();
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
    ENABLE_GLOBAL_INTERRUPTS();
    sem_post(&pipe->read);
 
}

void pipe_read(pipe_t * pipe, char * msg)
{
    sem_wait(&pipe->read);
    DISABLE_GLOBAL_INTERRUPTS();
    message_t * m;        
    m = pipe->head;
    pipe->head = m->next;
    *msg = m->msg;
    SRAMfree(m);
    ENABLE_GLOBAL_INTERRUPTS();
    sem_post(&pipe->write);
}