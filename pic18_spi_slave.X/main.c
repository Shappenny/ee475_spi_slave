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

// Code for master main.c
        for (int i = 0; i < 1024; ++i) 
        {
        int a = sample_adc(11);
        sram_write(a, i);
        // Buffer 80% full; request upload
        if (i == 820) 
        {
            // Ask permission from surface
            SPI_CSN = 0;
            spi_Send_Read(UPLOAD_REQ0);
            spi_Send_Read(UPLOAD_REQ1);
            SPI_CSN = 1;
            // Wait for response
            delay(1000);
            SPI_CSN = 0;
            unsigned char ack0 = spi_Send_Read(UPLOAD_REQ0);
            unsigned char ack1 = spi_Send_Read(UPLOAD_REQ1);
            SPI_CSN = 1;
            canSend = (ack0 == UPLOAD_ACK0) && (ack1 == UPLOAD_ACK1);
        // Buffer 90% full; start collection on second buffer
        } else if (i == 922) 
        {
            // TODO
        }
        
    }
    
    if (canSend)
    {
        // Upload to surface ship, if we have permission
        for (int i = 0; i < 1024; i++)
        {
            int data = sram_read(i);
            SPI_CSN = 0;
            spi_Send_Read(data);
            SPI_CSN = 1;
            delay(10000);
        }
    }

void delay(int s) {
    int a = 0;
    int i;
    for (i = 0; i < s; i++) {
        a++;
    }
}
