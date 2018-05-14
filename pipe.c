/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "pipe.h"
#include "kernel.h"

/* Declara��o da variavel global de fila */
extern Queue_t Queue;

/* A fun��o recebe o endere�o do pipe e seta os valores da posi��o de leitura 
 * e de escrita para a primeira posi��o do vetor. pipe_count � a variavel que 
 * nos diz quantas mensagens n�o lidas temos no nosso vetor.
 */

void pipe_create(pipe_t * pipe)
{
    pipe->read_pos    = 0;
    pipe->write_pos   = 0;
    pipe->pipe_count  = 0;
}

/* A fun��o recebe o endere�o do pipe e o endere�o da variavel onde vamos guardar
 * a mensagem do pipe.
 */

void pipe_read(pipe_t * pipe, char * msg)
{
    DISABLE_GLOBAL_INTERRUPTS();
    
    /* Recebe o indice da fun��o que tentou ler algo no pipe */
    pipe->pos_task_read = Queue.task_running;
    
    /* Se tiver algo para ler no pipe ele entra no bloco e coloca a mensagem
     * na variavel que foi passada por paramentro, avan�a a posi��o do read_pos,
     * decrementa o pipe_count sinalizando que uma mensagem foi lida. Caso n�o
     * tenha mensagem para ser lida ele bloqueia a tarefa.
     */
    
    if (pipe->pipe_count > 0)
    {
        *msg = pipe->pipe_msg[pipe->read_pos];
        pipe->read_pos = (pipe->read_pos + 1) % PIPE_SIZE;
        pipe->pipe_count--;
        
        /* Verifia se a tarefa que estava escrevendo foi travada por n�o ter mais
         * espa�o para escrever e ent�o a libera. 
         */
        if (Queue.task_READY[pipe->pos_task_write].task_state == WAITING) 
        {
            Queue.task_READY[pipe->pos_task_write].blocked--;
            if(Queue.task_READY[pipe->pos_task_write].blocked <= 0 && Queue.task_READY[pipe->pos_task_write].time_to_delay <= 0)
            {
                Queue.tasks_ready++;
                Queue.task_READY[pipe->pos_task_write].task_state = READY;
                dispatcher(READY);
            }
        }
    }
    else 
    {
        Queue.tasks_ready--;
        Queue.task_READY[Queue.task_running].blocked++;
        dispatcher(WAITING);
    }
    ENABLE_GLOBAL_INTERRUPTS();
}

/* A fun��o recebe o endere�o do pipe e a mensagem do pipe. */

u_int pipe_write(pipe_t * pipe, char msg)
{
    u_int flag = 0;
    DISABLE_GLOBAL_INTERRUPTS();
    
    /* Recebe o indice da fun��o que tentou escrever algo no pipe */
    pipe->pos_task_write = Queue.task_running;
    
    /* Verifica se tem espa�o no pipe para escrever a mensagem. Caso n�o tenha
     * ele bloqueia a tarefa. Tendo espa�o o pipe recebe a mensagem, a posi��o de 
     * escrita avan�a uma posi��o e incrementa o pipe_count sinalizando que tem 
     * uma mensagem para ser lida. Caso a tarefa que deva ler o pipe esteja bloqueada
     * a fun��o libera a tarefa para a leitura.
     */
    
    if (pipe->pipe_count < PIPE_SIZE) 
    {
        pipe->pipe_msg[pipe->write_pos] = msg;
        pipe->write_pos = (pipe->write_pos + 1) % PIPE_SIZE;
        pipe->pipe_count++;
        flag = 1;
        // Verificar se tem algu�m esperando msg no pipe.
        if (Queue.task_READY[pipe->pos_task_read].task_state == WAITING) 
        {
            Queue.task_READY[pipe->pos_task_read].blocked--;
            if(Queue.task_READY[pipe->pos_task_read].blocked <= 0 && Queue.task_READY[pipe->pos_task_read].time_to_delay <= 0)
            {
                Queue.tasks_ready++;
                Queue.task_READY[pipe->pos_task_read].task_state = READY;
                dispatcher(READY);
            }
        }
    }
    else 
    {
        Queue.tasks_ready--;
        Queue.task_READY[Queue.task_running].blocked++;
        dispatcher(WAITING);
    }
    return flag;
    ENABLE_GLOBAL_INTERRUPTS();
}