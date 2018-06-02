/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef PIPE_H
#define	PIPE_H

#include "types.h"
#include "semaphore.h"


#define PIPE_SIZE   4

typedef char byte;

typedef struct message {
    char msg;
    struct message * next;
} message_t;

typedef struct pipe {
    message_t * head;
    message_t * tail;
    sem_t * read;
    sem_t * write;
} pipe_t;

pipe_t * pipe_create();
void pipe_read(pipe_t * pipe, char * msg);
void pipe_write(pipe_t * pipe, char msg);

#endif	/* PIPE_H */

