/* 
 * File:   main.c
 * Author: schwab_n & huetter_n
 *
 * Created on 16. Mai 2014, 11:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <htc.h>
#include "tlc59711_driver.h"
#include "uart.h"
#include "interrupt.h"


// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
/*
 * 
 */

void main(void) {

    unsigned char rgb_channel;

    OSCCON = 0xF0;  // Enable 8MHz Intosc + 4xPLL --> 32MHz
    while (!HFIOFL || !PLLR);  //Wait until PLL and Intosc are ready, 32MHz locked
    SPI_fInit();
    UART_fInit();
    TMR0_fInit();

    while(1){
        UART_fTelehandler(&color,
                channel_mode);
    }
/*        TLC_fControl(C_SETCOLOR);
        UARTRxBuffer[2] = 0x01;
        TLC_fControl(C_SETCOLOR);
        UARTRxBuffer[2] = 0x02;
        TLC_fControl(C_SETCOLOR);
        UARTRxBuffer[2] = 0x03;
        TLC_fControl(C_SETCOLOR);
       // while(1);
    }*/
}

