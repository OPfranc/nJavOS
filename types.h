/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef TYPES_H
#define	TYPES_H

#include "config.h"

typedef void* TASK;
typedef TASK (task_ptr_t)(void);
typedef unsigned int u_int;
typedef enum {READY = 0, RUNNING, WAITING, FINISHED} state_t;

typedef struct stack_PIC18 {
    u_int r_TOSU;
    u_int r_TOSL;
    u_int r_TOSH;
    struct stack_PIC18 * next;
    struct stack_PIC18 * previous;
} stack_PIC18_t;

typedef struct stack {
    stack_PIC18_t * head;
    stack_PIC18_t * tail;
    stack_PIC18_t * pointer;
    u_int stack_size;
} stack_t;

typedef struct tcb {
    u_int task_ID;
    u_int task_prior;
    int time_to_delay;
    int blocked;
    task_ptr_t *task_ptr;
    state_t task_state;
    stack_t task_stack;
    u_int task_exec;
    u_int rBSR;
    u_int rWORK;
    struct tcb * next;
} tcb_t;

typedef struct _Queue {
    tcb_t * head;
    tcb_t * tail;
    tcb_t * running;
    tcb_t * idle;
    u_int tasks_installed;
    u_int tasks_ready;
} Queue_t;

#endif	/* TYPES_H */

