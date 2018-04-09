/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef TYPES_H
#define	TYPES_H

#include "config.h"

#define IDLE 1
#define MAX_STACK_SIZE 31


// Tipos de dados
typedef void* TASK;
typedef TASK (task_ptr_t)(void);
typedef unsigned int u_int;
typedef enum {READY = 0, RUNNING, WAITING, FINISHED} state_t;

typedef struct stack_PIC18 {
    u_int r_TOSU;
    u_int r_TOSL;
    u_int r_TOSH;
} stack_PIC18_t;

typedef struct stack {
    stack_PIC18_t stack_data[MAX_STACK_SIZE];
    u_int stack_size;
} stack_t;

typedef struct tcb {
    u_int task_ID;
    u_int task_prior;
    task_ptr_t *task_ptr;
    state_t task_state;
    stack_t task_stack;
    u_int rBSR;
    u_int rWORK;
} tcb_t;

typedef struct ready_Queue {
    tcb_t task_READY[MAX_TASKS+IDLE];
    u_int tasks_installed;
    u_int task_running;
} ready_Queue_t;

typedef struct delay_task_data {
    u_int index_ready_queue;
    u_int time_to_delay;
} delay_task_data_t;

typedef struct delay_Queue {
    // Guarda a posição da tarefa na fila de aptos que está em delay
    delay_task_data_t task_DELAY_W[MAX_TASKS];
    // Quantidade de tarefas aguardando o fim da espera de tempo
    u_int tasks_waiting;    
} delay_Queue_t;


#endif	/* TYPES_H */

