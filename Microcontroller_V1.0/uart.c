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

#define TELESIZE 10
#define UART_RX_BUFFER_SIZE 20
#define STARTBYTE 0x42

/*
 * 
 */
static unsigned char UARTRxBuffer[UART_RX_BUFFER_SIZE] = {0x42, 0x20, 0x00, 0x0F, 0xFD, 0x7F, 0x7F, 0x7F, 0x7F, 0x55}; // Example Uart Rx Data for testing.
static unsigned char BufferCounter = 0;

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
    static unsigned char WriteIndex = 0;
    if (RCSTA & 0x06){ // If RxBuffer overflow or frame error
        dummy = RCREG; // Reading RCREG clears interrupt flag
      //  ErrorFlags = RCSTA>>1 & 0x03;
        CREN = 0;  // Clear error flags
        CREN = 1;  // Re-enable receiver
    }else{
        UARTRxBuffer[WriteIndex] = RCREG;
        BufferCounter++;
        WriteIndex++;
        WriteIndex %= UART_RX_BUFFER_SIZE;         
//        if (WriteIndex == 0){
//            NewData = 0x01;
//        }
    }
}


void UART_fTelehandler (MyUnion* color, unsigned char* mode)
{
    static unsigned char ReadIndex = 0;
    unsigned char channel;
    
    if(BufferCounter < TELESIZE) return;
    
    while(UARTRxBuffer[ReadIndex] != STARTBYTE)
    {
        ReadIndex++;
        ReadIndex %= UART_RX_BUFFER_SIZE;
        BufferCounter--;
        if(BufferCounter == 0) return;
    }

    if(BufferCounter < TELESIZE-1) return;

    channel = UARTRxBuffer[ReadIndex];

    ReadIndex++;
    ReadIndex %= UART_RX_BUFFER_SIZE;
    BufferCounter--;

//    mode += channel;
    mode[channel] = UARTRxBuffer[ReadIndex];

    ReadIndex++;
    ReadIndex %= UART_RX_BUFFER_SIZE;
    BufferCounter--;

    for(signed char d = 0; d < 6; d++)
    {
        color->uc_rgb_channel_color[channel][d] = UARTRxBuffer[ReadIndex];
        ReadIndex++;
        ReadIndex %= UART_RX_BUFFER_SIZE;
        BufferCounter--;
    }
}