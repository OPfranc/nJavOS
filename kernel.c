/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "kernel.h"
#include "scheduler.h"
#include "config.h"
#include "types.h"
#include "delay.h"

/* Declara��o da variavel global de fila */

Queue_t Queue;

/* Esta � uma tarefa que n�o faz nada, apena ocupa o processador enquanto
 * n�o houver tarefas para escalonar. 
 */

TASK idle() {   
    while (1) 
    {    
        Nop();
    }
    return 0;
}

/* Com a fila de tarefas instaladas j� criada a fun��o cria a tarefa IDLE
 * que fica na posi��o m�xima da fila. Define a prioridade do IDLE para a menor
 * poss�vel e o coloca como a tarefa executando. 
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
     * de pronta, a fun��o idle e sem delay
     */
    
    Queue.task_READY[IDLE].task_ID    = 0;
    Queue.task_READY[IDLE].task_prior = 99;
    Queue.task_READY[IDLE].task_state = READY;
    Queue.task_READY[IDLE].task_ptr   = idle;
    Queue.task_READY[IDLE].time_to_delay = 0;
}

/* Esta fun��o instala tarefas na fila de tarefas instaladas, ela 
 * recebe o ID da tarefa, a prioridade, e a fun��o que a executa. 
 * A fun��o instala no vetor de tarefas instaladas de forma sequencial,
 * como a fila come�a com tasks_installed igual a 0, precisamos incrementar
 * essa variavel ap�s o seu uso. Em caso de n�o estar habilitado o RR, 
 * ou seja, a prioridade est� habilitada, ao criar a tarefa chamamos
 * o dispachante para verificar quem � a tarefa de maior prioridade. */

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
    /* Define se a tarefa j� foi executada alguma vez */
    Queue.task_READY[Queue.tasks_installed].task_exec = 0;
    
    /* Incrementa o numero de tarefas instaladas e o n�mero de tarefas prontas */
    Queue.tasks_installed++;
    Queue.tasks_ready++;
    if(!RR_SCHEDULER) dispatcher(READY);
}

/* A fun��o decrementa o n�mero de tarefas prontas e chama o dispachante mudando
 * o estado da tarefa para finalizada.
 */

void task_remove()
{
    Queue.tasks_ready--;
    dispatcher(FINISHED);
}

/* Recebe a posi��o da tarefa no vetor de tarefas instaladas e puxa as tarefaas
 * posteriores uma posi��o antes removendo a tarefa apontada, ao final decrementa 
 * o n�mero de tarefas instaladas. 
 */

void organize_queue(u_int r)
{
    for(r; r < Queue.tasks_installed - 1; r++)
    {
        Queue.task_READY[r] = Queue.task_READY[r+1];
    }
    Queue.tasks_installed--;    
}

/* A fun��o recebe o tempo t em milisegundo e atribui o tempo na TCB da tarefa
 * em execu��o, decrementa o n�mero de tarefas prontas e ent�o chama o 
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

/* A fun��o recebe o pr�ximo estado da tarefa em execu��o, troca pela para o 
 * estado passado, verifica se o estado passado � finalizado, se for, reorganiza
 * a fila a fim de remover a tarefa atual da fila de instalados,
 */

void dispatcher(state_t state)
{
    u_int t;
    
    /* Desabilita as interrup��es */
    DISABLE_GLOBAL_INTERRUPTS();
    
    /* Altera o estado da tarefa que est� em execu��o */
    Queue.task_READY[Queue.task_running].task_state = state;

    /* Verifica se o estado da tarefa � finalizada */
    if(state == FINISHED) organize_queue(Queue.task_running);
    
    /* Salva o contexto da tarefa em execu��o caso o estado n�o seja o finalizado */
    if(state != FINISHED) {
        SAVE_CONTEXT();
    }
    /* Caso o RR estaja habilitado ele escalona pelo RR caso contrario executa
     * o escalonador por prioridade e guarda na variavel t. 
     */
    
    if(RR_SCHEDULER){ t = rr_scheduler(); } else { t = priority_scheduler(); }
    
    /* Atribui t a tarefa que est� executando */
    Queue.task_running = t;
    
    /* Coloca o estado da fun��o escolhida para executando */  
    Queue.task_READY[Queue.task_running].task_state = RUNNING;
    
    /* Restaura o contexto da fun��o escolhida */
    RESTORE_CONTEXT();
    
    /* Habilita as interrup��es */
    ENABLE_GLOBAL_INTERRUPTS();
}