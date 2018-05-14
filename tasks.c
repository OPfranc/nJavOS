/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <pic18f4520.h>
#include <math.h>
#include "tasks.h"
#include "semaphore.h"
#include "kernel.h"
#include "pipe.h"

//extern sem_t p;
extern pipe_t p;
void config_user()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    TRISDbits.RD3 = 0;
    TRISDbits.RD4 = 0;
    TRISDbits.RD5 = 0;

    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTDbits.RD4 = 0;
    PORTDbits.RD5 = 0;
}

TASK task_one()
{
    while(1)
    {
        /*
        sem_wait(&p);
        PORTDbits.RD0 = 1;
        task_delay(100);
        PORTDbits.RD0 = 0;
        sem_post(&p);
        */
        char msg = 'A';
        while(!pipe_write(&p, msg));
        PORTDbits.RD0 = ~PORTDbits.RD0;
        task_delay(100);
    }
    return 0;
}

TASK task_two()
{
    while(1)
    {
        /*
        sem_wait(&p);
        PORTDbits.RD1 = 1;
        task_delay(10);
        PORTDbits.RD1 = 0;
        sem_post(&p);
        */
        
        char msg;
        pipe_read(&p, &msg);
        if(msg == 'A') PORTDbits.RD2 = ~PORTDbits.RD2;
        PORTDbits.RD1 = ~PORTDbits.RD1;
        task_delay(10);        
    }
    return 0;
}