/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include "types.h"
#include "config.h"

/*
 * A estrutura de fila do semaforo conta com:
 * - tasks[] que é um vetor com o indice das tarefas bloqueadas pelo semaforo
 * - tasks_bloqued guarda o indice do final da fila
 * - next_task_to_free guarda o indice do inicio da fila
 */

typedef struct sem_queue {
    u_int tasks[MAX_TASKS];
    u_int tasks_bloqued;
    u_int next_task_to_free;
} sem_queue_t;

/*
 * A estrutura do semaforo é formada por:
 * - sem_v que é o valor de "lugares" disponíveis para o recurso
 * - sem_f que é o valor de "lugares" máximo para o recurso
 * - sem_b é a variavel que identifica se o semaforo já foi bloqueado
 * - sem_bloqued_queue é a fila de indices de tarefas bloqueadas
*/

typedef struct semaphore {
    int sem_v;
    int sem_f;
    sem_queue_t sem_bloqued_queue;
} sem_t;

void sem_init(sem_t *s, u_int s_valor, u_int f_valor);
void sem_wait(sem_t *s);
void sem_post(sem_t *s);

#endif	/* SEMAPHORE_H */

