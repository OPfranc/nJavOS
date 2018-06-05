#include "pipe.h"
#include "kernel.h"

extern Queue_t * Queue;

pipe_t * pipe_create()
{
    pipe_t *pipe;
    sem_t *w, *r;
    
    pipe = SRAMalloc(sizeof(pipe_t));
    
    w = sem_init(PIPE_SIZE);
    r = sem_init(0);

    pipe->head = NULL;
    pipe->tail = NULL;
    pipe->write = w;
    pipe->read = r;
    
    return pipe;
}

void pipe_write(pipe_t * pipe, char msg[])
{
    sem_wait(pipe->write);
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
    sem_post(pipe->read);
    ENABLE_GLOBAL_INTERRUPTS();
}

char * pipe_read(pipe_t * pipe)
{
    sem_wait(pipe->read);
    DISABLE_GLOBAL_INTERRUPTS();
    char * msg;
    message_t * m;        
    m = pipe->head;
    pipe->head = m->next;
    msg = m->msg;
    SRAMfree(m);
    ENABLE_GLOBAL_INTERRUPTS();
    sem_post(pipe->write);
    return msg;
}