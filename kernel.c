/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "kernel.h"
#include "scheduler.h"
#include "config.h"
#include "types.h"
#include "delay.h"

/* Declaração da variavel global de fila */

Queue_t Queue;

/* Esta é uma tarefa que não faz nada, apena ocupa o processador enquanto
 * não houver tarefas para escalonar. 
 */

TASK idle() {   
    while (1) 
    {    
        Nop();
    }
    return 0;
}

/* Com a fila de tarefas instaladas já criada a função cria a tarefa IDLE
 * que fica na posição máxima da fila. Define a prioridade do IDLE para a menor
 * possível e o coloca como a tarefa executando. 
 */

void OS_start()
{
    /*Define tarefas instaladas igual a 0*/
    Queue.tasks_installed = 0;
    
    /*Define a tarefa executando a tarefa IDLE*/
    Queue.task_running = IDLE;
    
    /*Define as tarefas prontas como 0 */
    Queue.tasks_ready = 0;
    
    /* Cria tarefa idle com o ID 0, a menor prioridade possivel, com o estado
     * de pronta, a função idle e sem delay
     */
    
    Queue.task_READY[IDLE].task_ID    = 0;
    Queue.task_READY[IDLE].task_prior = 99;
    Queue.task_READY[IDLE].task_state = READY;
    Queue.task_READY[IDLE].task_ptr   = idle;
    Queue.task_READY[IDLE].time_to_delay = 0;
}

/* Esta função instala tarefas na fila de tarefas instaladas, ela 
 * recebe o ID da tarefa, a prioridade, e a função que a executa. 
 * A função instala no vetor de tarefas instaladas de forma sequencial,
 * como a fila começa com tasks_installed igual a 0, precisamos incrementar
 * essa variavel após o seu uso. Em caso de não estar habilitado o RR, 
 * ou seja, a prioridade está habilitada, ao criar a tarefa chamamos
 * o dispachante para verificar quem é a tarefa de maior prioridade. */

void task_create(u_int id, u_int prior, task_ptr_t ptr_f)
{
    Queue.task_READY[Queue.tasks_installed].task_ID    = id;
    Queue.task_READY[Queue.tasks_installed].task_prior = prior;
    
    /* Tempo de delay na tarefa */
    Queue.task_READY[Queue.tasks_installed].time_to_delay = 0;  

    /* Define a quantidade de bloqueios da tarefa */
    Queue.task_READY[Queue.tasks_installed].blocked = 0;
    Queue.task_READY[Queue.tasks_installed].task_state = READY;
    Queue.task_READY[Queue.tasks_installed].task_ptr   = ptr_f;
    Queue.task_READY[Queue.tasks_installed].task_stack.stack_size = 0;
    /* Define se a tarefa já foi executada alguma vez */
    Queue.task_READY[Queue.tasks_installed].task_exec = 0;
    
    /* Incrementa o numero de tarefas instaladas e o número de tarefas prontas */
    Queue.tasks_installed++;
    Queue.tasks_ready++;
    if(!RR_SCHEDULER) dispatcher(READY);
}

/* A função decrementa o número de tarefas prontas e chama o dispachante mudando
 * o estado da tarefa para finalizada.
 */

void task_remove()
{
    Queue.tasks_ready--;
    dispatcher(FINISHED);
}

/* Recebe a posição da tarefa no vetor de tarefas instaladas e puxa as tarefaas
 * posteriores uma posição antes removendo a tarefa apontada, ao final decrementa 
 * o número de tarefas instaladas. 
 */

void organize_queue(u_int r)
{
    for(r; r < Queue.tasks_installed - 1; r++)
    {
        Queue.task_READY[r] = Queue.task_READY[r+1];
    }
    Queue.tasks_installed--;    
}

/* A função recebe o tempo t em milisegundo e atribui o tempo na TCB da tarefa
 * em execução, decrementa o número de tarefas prontas e então chama o 
 * dispachante alterando o estado da tarefa atual para espera.
 */

void task_delay(u_int t)
{
    DISABLE_GLOBAL_INTERRUPTS();
    Queue.task_READY[Queue.task_running].time_to_delay = t;
    Queue.tasks_ready--;
    dispatcher(WAITING);
    ENABLE_GLOBAL_INTERRUPTS();
}

/* A função recebe o próximo estado da tarefa em execução, troca pela para o 
 * estado passado, verifica se o estado passado é finalizado, se for, reorganiza
 * a fila a fim de remover a tarefa atual da fila de instalados,
 */

void dispatcher(state_t state)
{
    u_int t;
    
    /* Desabilita as interrupções */
    DISABLE_GLOBAL_INTERRUPTS();
    
    /* Altera o estado da tarefa que está em execução */
    Queue.task_READY[Queue.task_running].task_state = state;

    /* Verifica se o estado da tarefa é finalizada */
    if(state == FINISHED) organize_queue(Queue.task_running);
    
    /* Salva o contexto da tarefa em execução caso o estado não seja o finalizado */
    if(state != FINISHED) {
        SAVE_CONTEXT();
    }
    /* Caso o RR estaja habilitado ele escalona pelo RR caso contrario executa
     * o escalonador por prioridade e guarda na variavel t. 
     */
    
    if(RR_SCHEDULER){ t = rr_scheduler(); } else { t = priority_scheduler(); }
    
    /* Atribui t a tarefa que está executando */
    Queue.task_running = t;
    
    /* Coloca o estado da função escolhida para executando */  
    Queue.task_READY[Queue.task_running].task_state = RUNNING;
    
    /* Restaura o contexto da função escolhida */
    RESTORE_CONTEXT();
    
    /* Habilita as interrupções */
    ENABLE_GLOBAL_INTERRUPTS();
}