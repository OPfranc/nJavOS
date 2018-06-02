#include "pipe.h"
#include "kernel.h"

extern Queue_t * Queue;
sem_t * temp_w, * temp_r;

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
    pipe->read  = r;
    
    return pipe;
}

void pipe_write(pipe_t * pipe, char msg)
{
    temp_w = pipe->write;
    temp_r = pipe->read;
    
    sem_wait(temp_w);
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
    sem_post(temp_r);
 
}

void pipe_read(pipe_t * pipe, char * msg)
{
    temp_w = pipe->write;
    temp_r = pipe->read;

    sem_wait(temp_r);
    DISABLE_GLOBAL_INTERRUPTS();
    message_t * m;        
    m = pipe->head;
    pipe->head = m->next;
    *msg = m->msg;
    SRAMfree(m);
    ENABLE_GLOBAL_INTERRUPTS();
    sem_post(temp_w);
}