/* 
 * File:   interrupt.c
 * Author: schwab_n
 *
 * Created on 20. Mai 2014, 15:33
 */

#include <stdio.h>
#include <stdlib.h>
#include <htc.h>
#include "uart.h"
#include "tlc59711_driver.h"
#include "interrupt.h"

/*
 * 
 */
void interrupt fInterruptRoutine() {
    if(PIR1bits.RCIF) UART_fRxData();
    if(INTCONbits.TMR0IF) TMR0_fInterrupt();
}

