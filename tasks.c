/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <pic18f4520.h>
#include <math.h>
#include "tasks.h"
#include "kernel.h"
#include "pipe.h"
#include <string.h>

extern pipe_t * p;

void config_user()
{
    TRISD = 0x0;
    PORTD = 0x0;
}

TASK task_one()
{
    int i = 0;
    while(1)
    {
        if(i < 3){
            char m[25];
            sprintf(m, "Oi, mundo");
            printf("W: %s", m);
            PORTDbits.RD0 = ~PORTDbits.RD0;
            pipe_write(p, m);
            //task_delay(1);
            i++;
        }
    }
    return 0;
}

TASK task_two()
{
    while(1)
    {
        char * msg;
        PORTDbits.RD1 = ~PORTDbits.RD1;
        msg = pipe_read(p);
        printf("R: %s", msg);
        task_delay(100);
    }
    return 0;
}