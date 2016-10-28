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
unsigned char spi_Send_Read(unsigned char byte);

void main(void)
{    
    // Need B0, B1, B2 as input for SPI_SS, SPI_CLK, SPI_IN
    TRISB = 0x07;
    TRISC = 0x10;
    TRISA = 0x10;
    
    ANSELB = 0x00;
    
    SPI1_Init();
    SPI1_Enable();
    
    while(1)
    {
        spi_Send_Read(0x78);
        //delay(10000);
    }
}

void delay(int s) {
    int a = 0;
    int i;
    for (i = 0; i < s; i++) {
        a++;
    }
}
