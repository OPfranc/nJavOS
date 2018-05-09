/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "types.h"
#include "delay.h"
#include "kernel.h"

/* Declara��o da variavel global de fila */
extern Queue_t Queue;

/* A fun��o passa por toda fila de instalados a procura de uma tarefa em espera
 * caso encontre ela verifica se o tempo de delay � maior que zero e ent�o o
 * decrementa, ent�o ela compara se o tempo de delay for menor ou igual a zero
 * e a tarefa n�o tem nenhum outro bloqueio (como um pipe ou um sem�foro), ent�o
 * a fun��o libera a tarefa. Ela incrementa o n�mero de fun��es prontas, troca o 
 * estado da tarefa para pronto, define o delay e os bloqueios para 0 e caso o 
 * escalonador de prioridade esteja habilitado ela chama o dispachante, pois �
 * necess�rio verificar se a tarefa que acabou de ser liberada n�o tem uma 
 * prioridade maior que a tarefa em execu��o.
 */

void delay_remove()
{
    int i;
    for (i = 0; i < (int)Queue.tasks_installed; i++)
    {
        if (Queue.task_READY[i].task_state == WAITING)
        {
            if(Queue.task_READY[i].time_to_delay > 0) Queue.task_READY[i].time_to_delay--;
            if ((Queue.task_READY[i].blocked <= 0) && (Queue.task_READY[i].time_to_delay <= 0)) 
            {
                Queue.tasks_ready++;
                Queue.task_READY[i].task_state = READY;   
                Queue.task_READY[i].time_to_delay = 0;
                Queue.task_READY[i].blocked = 0;
                if(!RR_SCHEDULER) dispatcher(READY);
            }
        }
    }
}