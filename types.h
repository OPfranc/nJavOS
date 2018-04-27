/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef TYPES_H
#define	TYPES_H

#include "config.h"

#define MAX_STACK_SIZE 31


// Tipos de dados
typedef void* TASK;
typedef TASK (task_ptr_t)(void);
typedef unsigned int u_int;
typedef enum {READY = 0, RUNNING, WAITING, WAITING_PIPE, WAITING_SEMAPHORE, FINISHED} state_t;

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
    u_int time_to_delay;
    task_ptr_t *task_ptr;
    state_t task_state;
    stack_t task_stack;
    u_int task_exec;
    u_int rBSR;
    u_int rWORK;
} tcb_t;

typedef struct ready_Queue {
    tcb_t task_READY[MAX_TASKS + 1];
    u_int tasks_installed;
    u_int tasks_ready;
    u_int task_running;
} ready_Queue_t;


#endif	/* TYPES_H */

