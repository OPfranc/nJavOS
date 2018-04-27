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
  quantum++;
  // Verifica se tem tarefa aguardando delay
  delay_remove();
  // verifica se acabou o quantum
  if (quantum == QUANTUM) {
    quantum = 0;
    nJavOS_dispatcher(READY);
  }  
  TMR0L = 252;
}

void nJavOS_init_timer()
{
  INTCONbits.TMR0IE   = 1;
  INTCONbits.TMR0IF   = 0;
  INTCONbits.GIE      = 1;

  T0CONbits.T0CS      = 0;
  T0CONbits.PSA       = 0;
  T0CONbits.T0PS      = 0b111;   // 001

  /**
   * 1 ms:
   *  256 - (0,001 * 4000000) / (256 (preescaler) * 4 (MCY))
   *  = 252 
   */
  TMR0L = 252;
  
  // Inicializa o timer
  T0CONbits.TMR0ON    = ON;
}
