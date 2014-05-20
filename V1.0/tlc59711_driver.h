/* 
 * File:   tlc59711_driver.h
 * Author: schwab_n
 *
 * Created on 16. Mai 2014, 11:29
 */

#ifndef TLC59711_DRIVER_H
#define	TLC59711_DRIVER_H

#define _XTAL_FREQ 32000000
#define C_SETCOLOR 0x10
#define TLC59711_MODULES 1
#define RGB_CHANNEL_NUMBER TLC59711_MODULES*4
#define RGB_MODES 2 // Number of implemented modes

    void SPI_fInit(void);
    void SPI_fWrite(unsigned char data);
    void TLC_fWrite(void);
    void TLC_fControl(unsigned char command);
    void delay_ms(unsigned int ms);
    void delay_s(unsigned char s);

#endif	/* TLC59711_DRIVER_H */

