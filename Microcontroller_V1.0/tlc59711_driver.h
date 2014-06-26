/* 
 * File:   tlc59711_driver.h
 * Author: schwab_n
 *
 * Created on 16. Mai 2014, 11:29
 */

#ifndef TLC59711_DRIVER_H
#define	TLC59711_DRIVER_H

#define _XTAL_FREQ 32000000
#define UART_STARTBYTE 0x42
#define C_SETCOLOR 0x10
#define TLC59711_MODULES 1
#define CHANNELS_PER_MODULE 4
#define RGB_CHANNEL_NUMBER TLC59711_MODULES*CHANNELS_PER_MODULE
#define RGB_MODES 2 // Number of implemented modes

typedef union {
    unsigned char uc_rgb_channel_color[RGB_CHANNEL_NUMBER][6];
    unsigned int  ui_rgb_channel_color[RGB_CHANNEL_NUMBER][3];
} MyUnion;

extern unsigned char channel_mode[RGB_CHANNEL_NUMBER];
extern  MyUnion color;

//extern unsigned char SPITxBuffer[24];

    void SPI_fInit(void);
    void SPI_fWrite(unsigned char data);
    void TLC_fUpdate(void);
    void TLC_fControl(void);
    void TLC_fFadeOneColor(unsigned char channel);
    void TMR0_fInit(void);
    void TMR0_fInterrupt(void);
    void delay_ms(unsigned int ms);
    void delay_s(unsigned char s);

#endif	/* TLC59711_DRIVER_H */

