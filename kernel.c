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
Queue_t Queue;

// Tarefa IDLE
TASK idle() {   
    while (1) 
    {    
        Nop();
        Nop();
        Nop();  
    }
    return 0;
}

// Chamadas de sistema
void OS_start()
{
    Queue.tasks_installed = 0;
    Queue.task_running = IDLE;
    Queue.tasks_ready = 0;
    // Cria tarefa idle
    Queue.task_READY[IDLE].task_ID    = 0;
    Queue.task_READY[IDLE].task_prior = 99;
    Queue.task_READY[IDLE].task_state = READY;
    Queue.task_READY[IDLE].task_ptr   = idle;
    Queue.task_READY[IDLE].time_to_delay = 0;
}

void task_create(u_int id, u_int prior, task_ptr_t ptr_f)
{
    Queue.task_READY[Queue.tasks_installed].task_ID    = id;
    Queue.task_READY[Queue.tasks_installed].task_prior = prior;
    Queue.task_READY[Queue.tasks_installed].time_to_delay = 0;  
    Queue.task_READY[Queue.tasks_installed].blocked = 0;
    Queue.task_READY[Queue.tasks_installed].task_state = READY;
    Queue.task_READY[Queue.tasks_installed].task_ptr   = ptr_f;
    Queue.task_READY[Queue.tasks_installed].task_stack.stack_size = 0;
    Queue.task_READY[Queue.tasks_installed].task_exec = 0;
    Queue.tasks_installed++;
    Queue.tasks_ready++;
    if(!RR_SCHEDULER) dispatcher(READY);
}

void task_remove()
{
    DISABLE_GLOBAL_INTERRUPTS();
    u_int t = Queue.task_running;
    Queue.tasks_ready--;
    dispatcher(FINISHED);
    for(t; t < Queue.tasks_installed - 1; t++)
    {
        Queue.task_READY[t] = Queue.task_READY[t+1];
    }
    Queue.tasks_installed--;
    ENABLE_GLOBAL_INTERRUPTS();
}

void task_delay(u_int xMs)
{
    DISABLE_GLOBAL_INTERRUPTS();
    Queue.task_READY[Queue.task_running].time_to_delay = xMs;
    Queue.tasks_ready--;
    dispatcher(WAITING);
    ENABLE_GLOBAL_INTERRUPTS();
}

void dispatcher(state_t state)
{
    u_int t;
    DISABLE_GLOBAL_INTERRUPTS();
    // Altera o estado da tarefa que está em execução
    Queue.task_READY[Queue.task_running].task_state = state;
    // Salva o contexto da tarefa em execução
    SAVE_CONTEXT();
    // Escolha a próxima tarefa que será executada
    RR_SCHEDULER ? t = rr_scheduler() : t = priority_scheduler();
    Queue.task_running = t;
    // Restaura o contexto da tarefa que será executada  
    Queue.task_READY[Queue.task_running].task_state = RUNNING;
    RESTORE_CONTEXT();
    //Queue.task_READY[Queue.task_running].task_ptr();
    ENABLE_GLOBAL_INTERRUPTS();
}