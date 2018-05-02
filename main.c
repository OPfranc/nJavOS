/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <xc.h>
#include "tasks.h"
#include "kernel.h"
#include "config_timer.h"
#include "semaphore.h"

// Variáveis globais
sem_t c1, c0;
asm("GLOBAL _task_one, _task_two, _idle");

void main(void) {

    // Faz configuração de usuário
    config_user();

    OS_start();
    init_timer();

    sem_init(&c1, 1, 1);
    sem_init(&c0, 0, 1);

    // Cria as tarefas
    task_create(1, 10, task_one);
    task_create(2, 10, task_two);

    while (1){
    }

    return;
}
