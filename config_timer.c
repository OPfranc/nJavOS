/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <pic18f4520.h>

#include "config_timer.h"
#include "kernel.h"
#include "scheduler.h"
#include "delay.h"

int quantum = 0;
void interrupt timer_zero()
{
    INTCONbits.TMR0IF = 0;
    if(RR_SCHEDULER) quantum++;
    delay_remove();
    if (quantum == QUANTUM && RR_SCHEDULER) {
        quantum = 0;
        dispatcher(READY);
    }  
    TMR0L = 252;
}

void init_timer()
{
    INTCONbits.TMR0IE   = 1;
    INTCONbits.TMR0IF   = 0;
    INTCONbits.GIE      = 1;
    T0CONbits.T0CS      = 0;
    T0CONbits.PSA       = 0;
    T0CONbits.T0PS      = 0b111;
    TMR0L = 252;
    T0CONbits.TMR0ON    = ON;
    /**
     * 1 ms:
     *  256 - (0,001 * 4000000) / (256 (preescaler) * 4 (MCY))
     *  = 252 
     */    
}
