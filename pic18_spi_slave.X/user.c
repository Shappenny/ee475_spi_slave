/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */

    /* Setup analog functionality and port direction */

    /* Initialize peripherals */

    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */

    /* Enable interrupts */
}

unsigned char SpiRead(void)
{
	/* Read a byte, send dummy byte */
    return spi_Send_Read(0x00);

}

void SpiWrite(unsigned char byte)
{
	/* Send a byte, ignore read value */
	spi_Send_Read(byte);
}

unsigned char spi_Send_Read(unsigned char byte)
{
    SSP2BUF = byte;
    while (!SSP2STATbits.BF);//(!PIR1bits.SSP1IF);
    //PIR1bits.SSP1IF = 0; 
    return SSP2BUF;
}
