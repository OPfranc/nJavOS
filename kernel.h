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

extern Queue_t Queue;

// Save context
#define SAVE_CONTEXT() u_int index_stack = 0; Queue.task_READY[Queue.task_running].rBSR = BSR; Queue.task_READY[Queue.task_running].rWORK = WREG; while (STKPTR) { Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSH = TOSH; Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSL = TOSL; Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSU = TOSU; asm("POP"); index_stack++; } Queue.task_READY[Queue.task_running].task_stack.stack_size = index_stack;
#define RESTORE_CONTEXT() u_int index_stack; STKPTR = 0; if(Queue.task_READY[Queue.task_running].task_exec == 0){ asm("PUSH"); Queue.task_READY[Queue.task_running].task_exec = 1; TOS = (int) Queue.task_READY[Queue.task_running].task_ptr; } else { index_stack = Queue.task_READY[Queue.task_running].task_stack.stack_size - 1; BSR = Queue.task_READY[Queue.task_running].rBSR; WREG = Queue.task_READY[Queue.task_running].rWORK; while (index_stack) { asm("PUSH"); TOSH = Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSH; TOSL = Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSL; TOSU = Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSU; index_stack--;}}

// Tarefa idle
TASK idle();

// Chamadas de sistema
void OS_start();
void task_create(u_int id, u_int prior, task_ptr_t ptr_f);
void task_remove();
void task_delay(u_int xMs);
void dispatcher(state_t state);


#endif	/* KERNEL_H */

