/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "kernel.h"
#include "scheduler.h"
#include "config.h"
#include "types.h"
#include "delay.h"

// Variáveis globais
ready_Queue_t Ready_Queue;

// Tarefa IDLE
TASK idle() {   
    while (1) 
    {    
        Nop();
        Nop();
        Nop();
    }
}

// Chamadas de sistema
void nJavOS_start()
{
    Ready_Queue.tasks_installed = 0;
    Ready_Queue.task_running = MAX_TASKS;
    Ready_Queue.tasks_ready = 0;
    // Cria tarefa idle
    Ready_Queue.task_READY[MAX_TASKS].task_ID    = 0;
    Ready_Queue.task_READY[MAX_TASKS].task_prior = 0;
    Ready_Queue.task_READY[MAX_TASKS].task_state = READY;
    Ready_Queue.task_READY[MAX_TASKS].task_ptr   = idle;
    Ready_Queue.task_READY[MAX_TASKS].time_to_delay = 0;
}

void nJavOS_task_create(u_int id, u_int prior, task_ptr_t ptr_f)
{
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_ID    = id;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_prior = prior;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].time_to_delay = 0;  
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_state = READY;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_ptr   = ptr_f;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_stack.stack_size = 0;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_exec = 0;
  Ready_Queue.tasks_installed++;
  Ready_Queue.tasks_ready++;
}

void nJavOS_task_delay(u_int xMs)
{
    u_int position = 0;
    DISABLE_GLOBAL_INTERRUPTS();
    Ready_Queue.task_READY[Ready_Queue.task_running].time_to_delay = xMs;
    Ready_Queue.tasks_ready = Ready_Queue.tasks_ready - 1;
    nJavOS_dispatcher(WAITING);
    ENABLE_GLOBAL_INTERRUPTS();
}

void nJavOS_dispatcher(state_t state)
{
    DISABLE_GLOBAL_INTERRUPTS();
    // Altera o estado da tarefa que está em execução
    Ready_Queue.task_READY[Ready_Queue.task_running].task_state = state;
    // Salva o contexto da tarefa em execução
    SAVE_CONTEXT();
    // Escolha a próxima tarefa que será executada
    Ready_Queue.task_running = rr_scheduler();
    // Restaura o contexto da tarefa que será executada  
    Ready_Queue.task_READY[Ready_Queue.task_running].task_state = RUNNING;
    RESTORE_CONTEXT();
    //Ready_Queue.task_READY[Ready_Queue.task_running].task_ptr();
    ENABLE_GLOBAL_INTERRUPTS();
}