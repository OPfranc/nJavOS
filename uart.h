/* 
 * File:   uart.h
 * Author: teabr
 *
 * Created on 2 de Junho de 2018, 21:45
 */

#ifndef UART_H
#define	UART_H

#include <xc.h>
#include <pic18f4520.h>

void init_uart();
void putch(char c);
char read_char(void);

#endif	/* UART_H */

