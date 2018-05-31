/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include "types.h"
#include "config.h"


typedef struct sem_queue {
    tcb_t * task;
    struct sem_queue * next;
} sem_queue_t;

typedef struct semaphore {
    int sem_v;
    sem_queue_t * sem_bloqued_queue_head;
    sem_queue_t * sem_bloqued_queue_tail;
} sem_t;

void sem_init(sem_t *s, u_int s_valor);
void sem_wait(sem_t *s);
void sem_post(sem_t *s);

#endif	/* SEMAPHORE_H */

