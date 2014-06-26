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

unsigned char channel_mode[RGB_CHANNEL_NUMBER];

//union MyUnion color;

//unsigned char SPITxBuffer[24];
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

const unsigned char BCr = 0x7F;
const unsigned char BCg = 0x7F;
const unsigned char BCb = 0x7F;


void SPI_fInit() {
    TRISCbits.TRISC0 = 0;   // SCK
    TRISCbits.TRISC2 = 0;   // SDO
    
    SSP1CON1bits.SSPEN = 1; // Enable Serial Ports for SPI Mode (f=Fosc/4)
                            // for f=Fosc/64 --> SSP1CON1 = 0x22
}

void TMR0_fInit(){
    // TMR0 Reg
    OPTION_REGbits.PSA = 0;     // Enable Prescaler
    OPTION_REGbits.TMR0CS = 0;  // Chose Fosc/4 as clk

    // TMR0 Interrupt
    INTCON |= 0xE0; // Global, Peripheral and TMR0 Interrupt enable
}

void SPI_fWrite (unsigned char data){
    unsigned char dummy;
    SSP1BUF = data;
    while(!SSP1STATbits.BF);
    dummy = SSP1BUF;
}

void TLC_fControl (void){
    unsigned char channel;
    for (channel = 0; channel < RGB_CHANNEL_NUMBER; channel++){
        switch(channel_mode[channel] & 0xF0){
            case 0x10:  //Just update color once
                        break;
            case 0x20:  //TLC_fFadeOneColor(channel);
                        break;
        }
       TLC_fUpdate();
    }
}

void TLC_fFadeOneColor (unsigned char channel){
    unsigned speed = channel_mode[channel] & 0x0F;
    static unsigned char time = 0;
    static unsigned char step = 0;
    static unsigned char counting_up;
    if(time == step*(16-speed)){
        for (unsigned char rgb = 0; rgb < 3; rgb++){
            color.ui_rgb_channel_color [channel][rgb] *= FadeValue[step] / 65535;
        }
        if(counting_up) step++;
        else step--;
        if(step == 0xFF) counting_up = 0;
        else if(step == 0x00) counting_up = 1;
        time = 0;
    } else {
        time++;
    }
}

void TLC_fUpdate(void) {
    unsigned long command;

    // Magic word for write
    command = 0x25;

    command <<= 5;
    //OUTTMG = 1, EXTGCK = 0, TMGRST = 1, DSPRPT = 1, BLANK = 0 -> 0x16
    command |= 0x16;

    command <<= 7;
    command |= BCr;

    command <<= 7;
    command |= BCg;

    command <<= 7;
    command |= BCb;

  //cli();
    for (signed char modul = TLC59711_MODULES-1; modul >= 0; modul--){
        SPI_fWrite(command >> 24);
        SPI_fWrite(command >> 16);
        SPI_fWrite(command >> 8);
        SPI_fWrite(command);

        for (signed char channel = CHANNELS_PER_MODULE-1; channel >= 0; channel--){
        // 12 channels per TLC59711
            for (signed char rgb=5; rgb >= 0 ; rgb--) {
                // 16 bits per channel, send MSB first
                SPI_fWrite(color.uc_rgb_channel_color[channel + CHANNELS_PER_MODULE * modul][rgb]);
            }
        }
    }
}

void TMR0_fInterrupt (){ // 1ms tick
    INTCONbits.TMR0IF = 0; // Clear interrupt flag
    TMR0 = 0xE1; // With prescaler 1:256: 1ms until overflow & interrupt
    TLC_fControl();
    /*/SPI_fWrite(0xAA);
    SPI_fWrite(0x55);
    SPI_fWrite(0x00);
    SPI_fWrite(0xFF);*/
}

void delay_ms (unsigned int ms){
    for(unsigned int jk = ms; jk > 0; jk--){
        __delay_ms(1);
    }
}