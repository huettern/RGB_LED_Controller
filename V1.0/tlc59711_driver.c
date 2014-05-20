/* 
 * File:   tlc59711_driver.c
 * Author: schwab_n
 *
 * Created on 16. Mai 2014, 11:30
 */

#include <stdio.h>
#include <stdlib.h>
#include <htc.h>
#include <xc.h>
#include "uart.h"
#include "tlc59711_driver.h"
#include "interrupt.h"



/*
 * 
 */
unsigned char channel_mode[RGB_CHANNEL_NUMBER][RGB_MODES];
unsigned char channel_color[RGB_CHANNEL_NUMBER][6];

unsigned char SPITxBuffer[24];
unsigned char vBCr = 0x7F;
unsigned char vBCg = 0x7F;
unsigned char vBCb = 0x7F;
const unsigned int FadeValue [] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
    3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7,
    7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 14, 15,
    15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    31, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 49, 52, 54, 56, 59,
    61, 64, 67, 70, 73, 76, 79, 83, 87, 91, 95, 99, 103, 108, 112,
    117, 123, 128, 134, 140, 146, 152, 159, 166, 173, 181, 189, 197,
    206, 215, 225, 235, 245, 256, 267, 279, 292, 304, 318, 332, 347,
    362, 378, 395, 412, 431, 450, 470, 490, 512, 535, 558, 583, 609,
    636, 664, 693, 724, 756, 790, 825, 861, 899, 939, 981, 1024, 1069,
    1117, 1166, 1218, 1272, 1328, 1387, 1448, 1512, 1579, 1649, 1722,
    1798, 1878, 1961, 2048, 2139, 2233, 2332, 2435, 2543, 2656, 2773,
    2896, 3025, 3158, 3298, 3444, 3597, 3756, 3922, 4096, 4277, 4467,
    4664, 4871, 5087, 5312, 5547, 5793, 6049, 6317, 6596, 6889, 7194,
    7512, 7845, 8192, 8555, 8933, 9329, 9742, 10173, 10624, 11094,
    11585, 12098, 12634, 13193, 13777, 14387, 15024, 15689, 16384,
    17109, 17867, 18658, 19484, 20346, 21247, 22188, 23170, 24196,
    25267, 26386, 27554, 28774, 30048, 31378, 32768, 34218, 35733,
    37315, 38967, 40693, 42494, 44376, 46340, 48392, 50534, 52772,
    55108, 57548, 60096, 62757, 65535
};

void SPI_fInit(void) {
    SSP1CON1 = 0x20;
}

void TMR0_fInit(){
    // TMR0 Reg
    // Let by default

    // TMR0 Interrupt
    INTCON |= 0xE0; // Global, Peripheral and TMR0 Interrupt enable
}

void SPI_fWrite (unsigned char data){
    SSP1BUF = data;
    while(!SSP1IF);
    SSP1IF = 0;
}

void TLC_fControl (unsigned char command){
    unsigned char channel;  // At first channel, channel = 0 and so on
    channel = UARTRxBuffer[2];
    if(command & 0xF0 == C_SETCOLOR){   // Only Change Color once
        for (unsigned char n = 6; n > 0; n--){
            SPITxBuffer[channel*6+n-1] = UARTRxBuffer[n+2];
        }
       TLC_fWrite();
    }
}

void TLC_fFadeOneColor (unsigned int red, unsigned int green, unsigned int blue, unsigned char speed){

}

void TLC_fWrite(void) {
    unsigned long command;

      // Magic word for write
    command = 0x25;

    command <<= 5;
    //OUTTMG = 1, EXTGCK = 0, TMGRST = 1, DSPRPT = 1, BLANK = 0 -> 0x16
    command |= 0x16;

    command <<= 7;
    command |= vBCr;

    command <<= 7;
    command |= vBCg;

    command <<= 7;
    command |= vBCb;

  //cli();
    SPI_fWrite(command >> 24);
    SPI_fWrite(command >> 16);
    SPI_fWrite(command >> 8);
    SPI_fWrite(command);

    // 12 channels per TLC59711
    for (signed char c=11; c >= 0 ; c--) {
        // 16 bits per channel, send MSB first
        SPI_fWrite(SPITxBuffer[c*2+1]);
        SPI_fWrite(SPITxBuffer[c*2]);
    }
}

void interrupt TMR0_fInterrupt (){
    INTCONbits.TMR0IF = 0; // Clear interrupt flag
    TMR0 = 0x83; // With prescaler 1:256: 1ms until overflow & interrupt
}

void delay_ms (unsigned int ms){
    for(unsigned int jk = ms; jk > 0; jk--){
        __delay_ms(1);
    }
}