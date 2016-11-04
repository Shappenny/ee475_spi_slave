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
//unsigned char nextByte;  // Holds next sending byte for acknowledgement
unsigned char roverUploadReq, canSendUART, startCollection, stopCollection;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void testUSART(void);

void main(void)
{    
    // 19.2Kb/s: baudRate = ((18432000 / (19200 * 16)) - 1) / 2, BRGH = 0
    unsigned int baudRate = ((18432000 / (19200 * 16)) - 1) / 2;

    TRISA = 0x10;
    // Need B0, B1, B2 as input for SPI_SS, SPI_CLK, SPI_IN
    TRISB = 0x07;
    // For USART need R7 in, R6 out; 
    // Need RC3 for p2s register input (reading from sram))
    //  1000 1000
    TRISC = 0x88;
    
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    InitApp();
    SPI1_Init();
    SPI1_Enable();
    OpenUSART1(baudRate);
    

    
    unsigned char cmd, data;
    while(1)
    {

        
        //char flag = 1;
        // Receive from USART if data is available
        while (DataRdy1USART()) 
        {
            cmd = readcUSART();           

            // Parse command
            if (cmd == START_RX)
            {
                startCollection = 1;
                //stopCollection = 0;
            } else if (cmd == STOP_RX)
            {
                //startCollection = 0;
                stopCollection = 1;
            }
        }
        // Send/read SPI
        //data = spiSendRead(SPI_IDLE);
        spiSendRead(0x78);
        delay(10);
        // If rover requested SPI upload, store incoming data bytes in SRAM
        if (!roverUploadReq)
        {
            int i;
            for (i = 0; i < 1024; ++i)
            {
                data = spiSendRead(SPI_IDLE);
                sram_write(i, data);
                delay(10);
            }
            // Upload complete
            roverUploadReq = 0;
        }
        // Upload data to land station if allowed
        if (canSendUART)
        {
            int i;
            for (i = 0; i < 1024; ++i)
            {
                data = sram_read(i);
                putc1USART(data);
                delay(10);
            }
        }
        //delay(10000);
        //testUSART();
    }
}

void testUSART()
{
    char c = 'A';  // A = 65 = 0x41
    while (1)
    {
        if (DataRdy1USART()) 
        {
            c = readcUSART();
            delay(10000);
            putc1USART(c);
        }
//        putc1USART(c++);
//        delay(100000);
//        if (c == 'Z')
//            c = 'A';
        
    }
}

void delay(unsigned long s)
{
    unsigned long a = 0;
    unsigned long i;
    for (i = 0; i < s; i++) {
        a++;
    }
}
