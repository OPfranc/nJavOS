/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include "types.h"
#include "config.h"

typedef struct sem_queue {
    u_int tasks[MAX_TASKS];
    u_int tasks_bloqued;
    u_int next_task_to_free;
} sem_queue_t;

typedef struct semaphore {
    int sem_v;
    int sem_f;
    u_int sem_b;
    sem_queue_t sem_bloqued_queue;
} sem_t;

void sem_init(sem_t *s, u_int s_valor, u_int f_valor);
void sem_wait(sem_t *s);
void sem_post(sem_t *s);

#endif	/* SEMAPHORE_H */

