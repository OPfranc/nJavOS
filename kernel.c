/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "kernel.h"
#include "scheduler.h"
#include "config.h"
#include "types.h"

// Variáveis globais
ready_Queue_t Ready_Queue;
delay_Queue_t Delay_Queue;


// Tarefa IDLE
TASK idle() {
  while (1) {
      Nop();
      Nop();
      Nop();
  }
  
  return;
}

// Chamadas de sistema
void nJavOS_start()
{
  Ready_Queue.tasks_installed = 0;
  Delay_Queue.tasks_waiting = 0;
  
  // Cria tarefa idle
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_ID    = 0;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_prior = 0;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_state = READY;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_ptr   = idle;
  
  Ready_Queue.tasks_installed++;
}

void nJavOS_task_create(u_int id, u_int prior, task_ptr_t ptr_f)
{
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_ID    = id;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_prior = prior;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_state = READY;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_ptr   = ptr_f;
  Ready_Queue.task_READY[Ready_Queue.tasks_installed].task_stack.stack_size = 0;
  
  Ready_Queue.tasks_installed++;
}

void nJavOS_task_delay(u_int xMs)
{
  DISABLE_GLOBAL_INTERRUPTS();
  
  Delay_Queue.task_DELAY_W[Delay_Queue.tasks_waiting].index_ready_queue = Ready_Queue.task_running;
  Delay_Queue.task_DELAY_W[Delay_Queue.tasks_waiting].time_to_delay = xMs;
  Delay_Queue.tasks_waiting++;
  
  // Troca o contexto da tarefa que está executando
  nJavOS_dispatcher(WAITING);
  
  ENABLE_GLOBAL_INTERRUPTS();
}

void nJavOS_dispatcher(state_t state)
{
  u_int next_task_to_run;
  
  DISABLE_GLOBAL_INTERRUPTS();
  
  // Altera o estado da tarefa que está em execução
  Ready_Queue.task_READY[Ready_Queue.task_running].task_state = state;
  // Salva o contexto da tarefa em execução
  SAVE_CONTEXT();
  // Escolha a próxima tarefa que será executada
#if RR_SCHEDULER
  next_task_to_run = rr_scheduler();
#else
  next_task_to_run = priority_scheduler();
#endif
  
  // Informa qual será a próxima tarefa a ser executada
  Ready_Queue.task_running = next_task_to_run;
  
  // Restaura o contexto da tarefa que será executada  
  RESTORE_CONTEXT();
  
  ENABLE_GLOBAL_INTERRUPTS();
}
