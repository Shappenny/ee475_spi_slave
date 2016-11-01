/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
unsigned char nextByte;  // Holds next sending byte for acknowledgement
unsigned char uploadReq0, uploadReq1;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void delay(int s);
void testUSART();
unsigned char spi_Send_Read(unsigned char byte);
//void OpenUSART1(unsigned int n);
//char getc1USART();

void main(void)
{    
    unsigned int baudRate = (20000000 / (19200 / 64)) - 1;

    TRISA = 0x10;
    // Need B0, B1, B2 as input for SPI_SS, SPI_CLK, SPI_IN
    TRISB = 0x07;
    // For USART need R7 in, R6 out; need R4 in for something else
    //  1001 0000
    TRISC = 0x90;
    
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    LATCbits.LATC0 = 1;
    delay(100000);
    PORTCbits.RC6 = 1;
    delay(100000);
    
    //SPI1_Init();
    //SPI1_Enable();
    OpenUSART1(baudRate);
    
    testUSART();
        
    while(1)
    {
        //spi_Send_Read(0x78);
        //delay(10000);
        //testUSART();
    }
}

void testUSART()
{
    char c = 27;
    while (1)//(DataRdy1USART())
    {
        putc1USART('U');
        //LATCbits.LATC6 = 1;
        //PORTCbits.RX1 = 1;
        //PORTCbits.RC0 = 1;
        delay(10000);
        //PORTCbits.RX1 = 0;
        //PORTCbits.RC0 = 0;
        //delay(10);
        
        
    }
}

void delay(int s)
{
    int a = 0;
    int i;
    for (i = 0; i < s; i++) {
        a++;
    }
}
