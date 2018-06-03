/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"
#include <xc.h>
#include <pic18f4520.h>
#include <stdio.h>
#include "sralloc.h"
#include "uart.h"

#define ENABLE_GLOBAL_INTERRUPTS() INTCONbits.GIE   = 1
#define DISABLE_GLOBAL_INTERRUPTS() INTCONbits.GIE  = 0

extern Queue_t * Queue;

#define SAVE_CONTEXT()\
    u_int index_stack = 0;\
    stack_PIC18_t * s;\
    Queue->running->rBSR = BSR;\
    Queue->running->rWORK = WREG;\
    while (STKPTR) {\
    Queue->running->task_stack.pointer->r_TOSH = TOSH;\
    Queue->running->task_stack.pointer->r_TOSL = TOSL;\
    Queue->running->task_stack.pointer->r_TOSU = TOSU;\
    asm("POP");\
    Queue->running->task_stack.pointer = Queue->running->task_stack.pointer->next;\
    index_stack++;\
    } Queue->running->task_stack.stack_size = index_stack;

#define RESTORE_CONTEXT()\
    u_int index_stack = Queue->running->task_stack.stack_size;\
    STKPTR = 0;\
    if(Queue->running->task_exec == 0){\
    asm("PUSH");\
    Queue->running->task_exec = 1;\
    TOS = (int) Queue->running->task_ptr; }\
    else {\
    BSR = Queue->running->rBSR;\
    WREG = Queue->running->rWORK;\
    Queue->running->task_stack.pointer = Queue->running->task_stack.tail;\
    while (index_stack) {\
    asm("PUSH");\
    TOSH = Queue->running->task_stack.pointer->r_TOSH;\
    TOSL = Queue->running->task_stack.pointer->r_TOSL;\
    TOSU = Queue->running->task_stack.pointer->r_TOSU;\
    Queue->running->task_stack.pointer = Queue->running->task_stack.pointer->previous;\
    index_stack--;}}

TASK idle();

void BCD(int n);

void start_queue();

void OS_start();

void task_create(u_int id, u_int prior, task_ptr_t ptr_f);

void task_remove();

void create_stack();

void destroy_stack();

void task_delay(u_int t);

void dispatcher(state_t state);

void organize_queue(u_int r);

#endif	/* KERNEL_H */

