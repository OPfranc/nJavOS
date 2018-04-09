/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */

#include <pic18f4520.h>

#include "tasks.h"

TASK task_one()
{
  while (1) {
      if(PORTDbits.RD0)
        PORTDbits.RD0 = 0;
      else 
        PORTDbits.RD0 = 1;
  }
}

TASK task_two()
{
  while (1) {
      if(PORTDbits.RD1)
        PORTDbits.RD1 = 0;
      else 
        PORTDbits.RD1 = 1;
  }
}

