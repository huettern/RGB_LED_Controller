/* 
 * File:   uart.h
 * Author: schwab_n
 *
 * Created on 16. Mai 2014, 11:33
 */

#ifndef UART_H
#define	UART_H

#define UART_RX_BUFFER_SIZE 10
extern unsigned char UARTRxBuffer[10];
extern unsigned char UARTwriteIndex;
extern unsigned char ErrorFlags;
extern unsigned char NewData;

#endif	/* UART_H */