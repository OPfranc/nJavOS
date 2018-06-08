/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef PIPE_H
#define	PIPE_H

#include "types.h"

#define PIPE_SIZE 4

typedef char byte;

typedef struct pipe {
    byte pipe_msg[PIPE_SIZE];
    u_int read_pos;
    u_int write_pos;
    tcb_t * task_read;
    tcb_t * task_write;
    int v;
} pipe_t;

void pipe_create(pipe_t * pipe);
void pipe_read(pipe_t * pipe, char * msg);
void pipe_write(pipe_t * pipe, char msg);
#endif	/* PIPE_H */

