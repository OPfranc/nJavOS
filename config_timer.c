#include "config_timer.h"
void nJavOS_init_timer()
{
    INTCONbits.GIE = 1;        //peripharel interrupt enable
    INTCONbits.TMR0IF = 0x0;  //Clear timer0 overflow interrupt flag
    INTCONbits.TMR0IE = 1;    //enable the timer0 by setting TRM0IE flag

    T0CON.TMR0ON = ON;     // Turn Timer0 on.       
    T0CON.T08BIT = 0;     // 16 Bit timer
    T0CON.T0CS = 0;       // Internal clock
    T0CON.PSA = 0;        // Set scaler to 1:4
    T0CON.T0PS = 111;

    TMR0H = 0x0;         // Initial count of 0xF830
    
    /*
     TEMPO EM MS = 256 - (TEMPO EM S * CLOCK)/ (PRE-SCALER * MCY)
     */
    
    TMR0L = 252;
}

int q = QUANTUM;
void interrupt timer_zero(void)
{
   // Reset interrupt flag
   INTCONbits.TMR0IF = 0;
   q--;

   if(q == 0)
   {
       q = QUANTUM
       nJavOS_dispatcher(READY);
   }
}