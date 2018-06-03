/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <xc.h>
#include "tasks.h"
#include "kernel.h"
#include "config_timer.h"
#include "pipe.h"

asm("GLOBAL _task_one, _task_two, _idle");

pipe_t * p;

void main(void) 
{
    SRAMInitHeap();
    config_user();
    OS_start();
    init_uart();
    init_timer();

    p = pipe_create();

    task_create(1, 1, task_one);
    task_create(2, 10, task_two);

    while (1){
    }

    return;
}
