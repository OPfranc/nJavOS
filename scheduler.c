#include "scheduler.h"
#include "kernel.h"

/* Declaração da variavel global de fila */
extern Queue_t Queue;

/* A função pega o indice da tarefa atual, em execução, e quadar em n_task,
 * compara se tem tarefas prontas, caso não tenha retorna IDLE como a tarefa 
 * escolhida, caso tenha tarefas prontas, ele faz ao menos uma vez o segunte:
 * caso se a tarefa em execução for IDLE ela define n_task como 0, a primeira 
 * tarefa, e vai passando a fila até encontrar uma tarefa que esteja em estado 
 * de pronto para executa, quando encontrar ele para, retorna n_task. 
 */

u_int rr_scheduler()
{   
    u_int n_task = Queue.task_running;
    if(Queue.tasks_ready > 0)
    {
        do
        {
            if(n_task == IDLE) n_task = 0;
            else n_task = ((n_task + 1) % Queue.tasks_installed);
        } while(Queue.task_READY[n_task].task_state != READY);
    }
    else 
    {
       n_task = IDLE; 
    }   
    return n_task;
}

/* A função define n_task como IDLE, faz a comparação se existe tarefas prontas, 
 * caso exista então a função passa por todas as tarefas instaladas através do 
 * for comparando se as tarefas estão prontas e se o a prioridade é maior, caso 
 * a tarefa esteja pronta e a prioridade é maior ele troca n_task por i, que é
 * o indice da tarefa no vetor de tarefas instaladas.
 */

u_int priority_scheduler()
{
    u_int i, n_task = IDLE;
    if(Queue.tasks_ready > 0)
    {        
        for(i = 0; i < Queue.tasks_installed; i++)
        {
            if(Queue.task_READY[i].task_state == READY && Queue.task_READY[i].task_prior < Queue.task_READY[n_task].task_prior) n_task = i;
        }
    }  
    return n_task;
}
