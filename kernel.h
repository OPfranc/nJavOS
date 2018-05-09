/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"
#include <xc.h>
#include <pic18f4520.h>

/* Habilita as interruções externas ou de timer */
#define ENABLE_GLOBAL_INTERRUPTS() INTCONbits.GIE   = 1

/* Desabilita as interruções externas ou de timer */
#define DISABLE_GLOBAL_INTERRUPTS() INTCONbits.GIE  = 0

extern Queue_t Queue;

/* Macro para salva contexto */
#define SAVE_CONTEXT() u_int index_stack = 0; Queue.task_READY[Queue.task_running].rBSR = BSR; Queue.task_READY[Queue.task_running].rWORK = WREG; while (STKPTR) { Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSH = TOSH; Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSL = TOSL; Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSU = TOSU; asm("POP"); index_stack++; } Queue.task_READY[Queue.task_running].task_stack.stack_size = index_stack;

/* Macro para restaurar contexto */
#define RESTORE_CONTEXT() u_int index_stack; STKPTR = 0; if(Queue.task_READY[Queue.task_running].task_exec == 0){ asm("PUSH"); Queue.task_READY[Queue.task_running].task_exec = 1; TOS = (int) Queue.task_READY[Queue.task_running].task_ptr; } else { index_stack = Queue.task_READY[Queue.task_running].task_stack.stack_size - 1; BSR = Queue.task_READY[Queue.task_running].rBSR; WREG = Queue.task_READY[Queue.task_running].rWORK; while (index_stack) { asm("PUSH"); TOSH = Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSH; TOSL = Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSL; TOSU = Queue.task_READY[Queue.task_running].task_stack.stack_data[index_stack].r_TOSU; index_stack--;}}

/* Função da tarefa IDLE */
TASK idle();

/* Inicializa a fila de tarefas instaladas e cria o IDLE */
void OS_start();

/* Instala a tarefa na fila de tarefas instaladas */
void task_create(u_int id, u_int prior, task_ptr_t ptr_f);

/* Remove a tarefa da fila de tarefas instaladas */
void task_remove();

/* Atribui o tempo de delay na tarefa */
void task_delay(u_int t);

/* Altera o estado da tarefa, salva o contexto, organiza a fila, chama o 
 * escalonador e restaura o contexto.
 */
void dispatcher(state_t state);

/* Move a fila de instalados a partir de r para uma posição abaixo */
void organize_queue(u_int r);

#endif	/* KERNEL_H */

