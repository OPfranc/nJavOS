/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <xc.h>
#include "tasks.h"
#include "kernel.h"
#include "config_timer.h"
#include "semaphore.h"
#include "pipe.h"

// Variáveis globais
//sem_t p;
pipe_t p;
asm("GLOBAL _task_one, _task_two, _idle");

void main(void) {

    /* Faz as configurações iniciais do usuário como:
     * - Seta as portas como entradas e saídas
     * - Define as tarefas
     */
    
    config_user();

    /* Inicializa o sitema operacional criando a fila de tarefas instaladas */
    OS_start();
    
    /* Inicializa o timer do PIC */
    init_timer();

    
    //sem_init(&p, 1);
    pipe_create(&p);
    
    // Cria as tarefas
    task_create(1, 1, task_one);
    task_create(2, 10, task_two);

    while (1){
    }

    return;
}
