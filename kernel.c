/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include "kernel.h"
#include "scheduler.h"
#include "config.h"
#include "types.h"
#include "delay.h"

Queue_t * Queue;

void start_queue()
{
    Queue = SRAMalloc(sizeof(Queue_t));
    Queue->head = NULL;
    Queue->idle = NULL;
    Queue->running = NULL;
    Queue->tail = NULL;
    Queue->tasks_installed = 0;
    Queue->tasks_ready = 0;
}

TASK idle() {   
    while (1) 
    {    
        Nop();
    }
    return 0;
}

void OS_start()
{
    start_queue();
    
    tcb_t * t;
    t = SRAMalloc(sizeof(tcb_t));
    
    t->task_ID = 0;
    t->task_prior = 99;
    t->time_to_delay = 0;
    t->blocked = 0;
    t->task_exec = 0;
    t->task_state = RUNNING;
    t->task_ptr = idle;
    t->task_stack.stack_size = 0;
    t->task_stack.pointer = NULL;
    t->task_stack.head = NULL;
    t->task_stack.tail = NULL;
    t->next = NULL;
    
    Queue->idle = t;
    Queue->running = t;
}

void task_create(u_int id, u_int prior, task_ptr_t ptr_f)
{
    DISABLE_GLOBAL_INTERRUPTS();
    tcb_t * t;
    t = SRAMalloc(sizeof(tcb_t));
    
    t->task_ID = id;
    t->task_prior = prior;
    t->time_to_delay = 0;
    t->blocked = 0;
    t->task_exec = 0;
    t->task_state = READY;
    t->task_ptr = ptr_f;
    t->task_stack.stack_size = 0;
    t->task_stack.pointer = NULL;
    t->task_stack.head = NULL;
    t->task_stack.tail = NULL;
    
    if(Queue->head == NULL)
    {
        Queue->head = t;
        Queue->tail = t;
        Queue->tail->next = t;
        Queue->idle->next = t;
    } 
    else
    {
        t->next = Queue->head;
        Queue->tail->next = t;
        Queue->tail = t;
    }
    Queue->tasks_installed++;
    Queue->tasks_ready++;
    ENABLE_GLOBAL_INTERRUPTS();
    if(!RR_SCHEDULER) dispatcher(READY);
}

void task_remove()
{
    Queue->tasks_ready--;
    dispatcher(FINISHED);
}

void task_delay(u_int t)
{
    DISABLE_GLOBAL_INTERRUPTS();
    Queue->running->time_to_delay = t;
    Queue->tasks_ready--;
    dispatcher(WAITING);
    ENABLE_GLOBAL_INTERRUPTS();
}

/* A função recebe o próximo estado da tarefa em execução, troca pela para o 
 * estado passado, verifica se o estado passado é finalizado, se for, reorganiza
 * a fila a fim de remover a tarefa atual da fila de instalados,
 */

void create_stack()
{
    u_int p = STKPTR - 1;
    
    stack_PIC18_t * cabeca;
    cabeca = SRAMalloc(sizeof(stack_PIC18_t));
    
    cabeca->next = NULL;
    cabeca->previous = NULL;
    
    Queue->running->task_stack.head = cabeca;
    Queue->running->task_stack.tail = cabeca;
    p--;
    
    while(p)
    {   
        stack_PIC18_t * s, * c;
        s = SRAMalloc(sizeof(stack_PIC18_t));

        c = Queue->running->task_stack.head;
        
        s->next = c;
        s->previous = c->previous;
        
        c->previous = s;
        Queue->running->task_stack.head = s;
        p--;
    }
    Queue->running->task_stack.pointer = Queue->running->task_stack.head;
}

void destroy_stack(){
    while(Queue->running->task_stack.head != NULL)
    {
        stack_PIC18_t * s;
        s = Queue->running->task_stack.head;
        Queue->running->task_stack.head = s->next;
        SRAMfree(s);
    }
}

void dispatcher(state_t state)
{    
    DISABLE_GLOBAL_INTERRUPTS();
    
    Queue->running->task_state = state;

    create_stack();
    
    SAVE_CONTEXT();
    
    if(RR_SCHEDULER)
        rr_scheduler();
    else
        priority_scheduler();
    
    Queue->running->task_state = RUNNING;
    
    RESTORE_CONTEXT();
    
    destroy_stack();
    
    ENABLE_GLOBAL_INTERRUPTS();
}

void BCD(int n)
{
    switch(n)
    {
        case 0:
            PORTDbits.RD2 = 0;
            PORTDbits.RD3 = 0;
            PORTDbits.RD4 = 0;
            PORTDbits.RD5 = 0;
            break;
        case 1:
            PORTDbits.RD2 = 1;
            PORTDbits.RD3 = 0;
            PORTDbits.RD4 = 0;
            PORTDbits.RD5 = 0;
            break;
        case 2:
            PORTDbits.RD2 = 0;
            PORTDbits.RD3 = 1;
            PORTDbits.RD4 = 0;
            PORTDbits.RD5 = 0;
            break;
        case 3:
            PORTDbits.RD2 = 1;
            PORTDbits.RD3 = 1;
            PORTDbits.RD4 = 0;
            PORTDbits.RD5 = 0;
            break;
        case 4:
            PORTDbits.RD2 = 0;
            PORTDbits.RD3 = 0;
            PORTDbits.RD4 = 1;
            PORTDbits.RD5 = 0;
            break;
        case 5:
            PORTDbits.RD2 = 1;
            PORTDbits.RD3 = 0;
            PORTDbits.RD4 = 1;
            PORTDbits.RD5 = 0;
            break;
        case 6:
            PORTDbits.RD2 = 0;
            PORTDbits.RD3 = 1;
            PORTDbits.RD4 = 1;
            PORTDbits.RD5 = 0;
            break;
        case 7:
            PORTDbits.RD2 = 1;
            PORTDbits.RD3 = 1;
            PORTDbits.RD4 = 1;
            PORTDbits.RD5 = 0;
            break;
        case 8:
            PORTDbits.RD2 = 0;
            PORTDbits.RD3 = 0;
            PORTDbits.RD4 = 0;
            PORTDbits.RD5 = 1;
            break;
        case 9:
            PORTDbits.RD2 = 1;
            PORTDbits.RD3 = 0;
            PORTDbits.RD4 = 0;
            PORTDbits.RD5 = 1;
            break;
        default:
            PORTDbits.RD2 = 1;
            PORTDbits.RD3 = 1;
            PORTDbits.RD4 = 1;
            PORTDbits.RD5 = 1;
    }
}
