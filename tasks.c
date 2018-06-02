/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <pic18f4520.h>
#include <math.h>
#include "tasks.h"
#include "kernel.h"
#include "pipe.h"

extern pipe_t * p;

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
        BCD(p->write->sem_v);
        PORTDbits.RD0 = ~PORTDbits.RD0;
        pipe_write(p, 'a');
        task_delay(100);
    }
    return 0;
}

TASK task_two()
{
    while(1)
    {
        BCD(p->read->sem_v);
        PORTDbits.RD1 = ~PORTDbits.RD1;
        char * msg;
        pipe_read(p, msg);
        task_delay(1000);
    }
    return 0;
}