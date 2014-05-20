/* 
 * File:   uart.c
 * Author: schwab_n
 *
 * Created on 16. Mai 2014, 11:32
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
unsigned char UARTRxBuffer[UART_RX_BUFFER_SIZE] = {0x42, 0x10, 0x00, 0xEF, 0xFF, 0xEF, 0xFF, 0xEF, 0xFF, 0x55}; // Example Uart Rx Data for testing.
                                // R G and B of channel 0 (first strip) should run with 50% duty cycle.
unsigned char UARTwriteIndex;
unsigned char ErrorFlags;
unsigned char NewData;

int UART_fInit (void) {

    TXSTA   = 0x04; // High baud rate
    RCSTA   = 0x90; // Serial Port and Receiver enable

    BAUDCON = 0x08; // 16-bit baud gen
    SPBRGH  = 0x00; // High baud rate register
    SPBRGL  = 0x44; // Baudrate = 115200 (effective: 115942, 0.64% deviation)

    INTCON  |= 0xC0; // Global interrupt and peripheral interrupt enable
    PIE1    |= 0x20; // EUSART Interrupt enable
    
//    TXSTA = 0x00;
//    RCSTA = 0x90;
//    SPBRGH = 0x00;
}

void UART_fRxData(void){
    unsigned char dummy;
    if (RCSTA & 0x06){ // If RxBuffer overflow or frame error
        dummy = RCREG; // Reading RCREG clears interrupt flag
        ErrorFlags = RCSTA>>1 & 0x03;
        CREN = 0;  // Clear error flags
        CREN = 1;  // Re-enable receiver
    }else{
         UARTRxBuffer[UARTwriteIndex] = RCREG;
         UARTwriteIndex++;
         UARTwriteIndex %= UART_RX_BUFFER_SIZE;         
    }
    if (UARTwriteIndex == 0){
        NewData = 0x01;
    }
}