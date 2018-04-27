/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"
#include <xc.h>
#include <pic18f4520.h>

#define ENABLE_GLOBAL_INTERRUPTS() INTCONbits.GIE   = 1
#define DISABLE_GLOBAL_INTERRUPTS() INTCONbits.GIE  = 0

extern ready_Queue_t Ready_Queue;

// Save context
#define SAVE_CONTEXT()\
    u_int index_stack = 0;\
    Ready_Queue.task_READY[Ready_Queue.task_running].rBSR = BSR;\
    Ready_Queue.task_READY[Ready_Queue.task_running].rWORK = WREG;\
    while (STKPTR) {\
        Ready_Queue.task_READY[Ready_Queue.task_running].task_stack.stack_data[index_stack].r_TOSH = TOSH;\
        Ready_Queue.task_READY[Ready_Queue.task_running].task_stack.stack_data[index_stack].r_TOSL = TOSL;\
        Ready_Queue.task_READY[Ready_Queue.task_running].task_stack.stack_data[index_stack].r_TOSU = TOSU;\
        asm("POP");\
        index_stack++;\        
    }\
    Ready_Queue.task_READY[Ready_Queue.task_running].task_stack.stack_size = index_stack;\

#define RESTORE_CONTEXT()\
    STKPTR = 0;\
    if(Ready_Queue.task_READY[Ready_Queue.task_running].task_exec == 0)\
    {\
        asm("PUSH");\
        Ready_Queue.task_READY[Ready_Queue.task_running].task_exec = 1;\
        TOS = Ready_Queue.task_READY[Ready_Queue.task_running].task_ptr;\
    }\
    else\
    {\
        u_int index_stack = Ready_Queue.task_READY[Ready_Queue.task_running].task_stack.stack_size - 1;\
        BSR = Ready_Queue.task_READY[Ready_Queue.task_running].rBSR;\
        WREG = Ready_Queue.task_READY[Ready_Queue.task_running].rWORK;\
        while (index_stack) {\
            asm("PUSH");\
            TOSH = Ready_Queue.task_READY[Ready_Queue.task_running].task_stack.stack_data[index_stack].r_TOSH;\
            TOSL = Ready_Queue.task_READY[Ready_Queue.task_running].task_stack.stack_data[index_stack].r_TOSL;\
            TOSU = Ready_Queue.task_READY[Ready_Queue.task_running].task_stack.stack_data[index_stack].r_TOSU;\
            index_stack--;\
        }\
    }\

// Tarefa idle
TASK idle();

// Chamadas de sistema
void nJavOS_start();
void nJavOS_task_create(u_int id, u_int prior, task_ptr_t ptr_f);
void nJavOS_task_delay(u_int xMs);
void nJavOS_dispatcher(state_t state);

#endif	/* KERNEL_H */

