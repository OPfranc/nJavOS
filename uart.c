#include "uart.h"
#include "kernel.h"

void putch(char c)
{
    DISABLE_GLOBAL_INTERRUPTS();
    // wait the end of transmission
    while (TXSTAbits.TRMT == 0);
    TXREG = c; // send the new byte
    ENABLE_GLOBAL_INTERRUPTS();
}

char read_char(void)
{
    while (PIR1bits.RC1IF == 0)
    {
        // wait for char
        if (RCSTAbits.OERR == 1)
        {
            RCSTAbits.OERR = 0; // clear overrun if it occurs
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        }
    }
    return RCREG;
}

void init_uart()
{
    TRISC = 0x80;
    TXSTAbits.SYNC = 0;
    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
    
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN = 1;
    // Setting for 19200 BPS
    BAUDCONbits.BRG16 = 0; // Divisor at 8 bit
    TXSTAbits.BRGH = 0; // No high-speed baudrate
    SPBRG = 12; // divisor value for 19200
}