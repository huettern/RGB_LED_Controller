/* 
 * File:   uart.h
 * Author: schwab_n
 *
 * Created on 16. Mai 2014, 11:33
 */

#ifndef UART_H
#define	UART_H

#include "tlc59711_driver.h"

int UART_fInit(void);
void UART_fRxData(void);
void RGB_fGetNewChannel(unsigned char* source, unsigned char* channel);
void RGB_fGetNewData(unsigned char* source, unsigned char* mode, unsigned int* r_data, unsigned int* g_data, unsigned int* b_data);
void UART_fTelehandler (MyUnion* color, unsigned char* mode);


#endif	/* UART_H */