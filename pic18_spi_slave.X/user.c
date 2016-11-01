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

#define _XTAL_FREQ 20000000

#include "usart.h"
#include "user.h"
#include "system.h"

extern unsigned char nextByte;
extern unsigned char uploadReq0, uploadReq1;

void OpenUSART1(unsigned int spbrg);
char getc1USART(void);
char DataRdy1USART(void);
void gets1USART(char *buffer, unsigned char len);
char Busy1USART(void);
void putrs1USART(const rom char *data);
void puts1USART(char *data);


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
    SSP2BUF = nextByte;
    while (!SSP2STATbits.BF);
    // Check for first upload request
    if (!uploadReq0 && !uploadReq1)
    {
        uploadReq0 = (SSP2BUF == UPLOAD_REQ0);
        nextByte = byte;
    // Check for second upload request
    } else if (uploadReq0 && !uploadReq1)
    {
        uploadReq1 = (SSP2BUF == #UPLOAD_REQ1);
        nextByte = UPLOAD_ACK0;
    // Upload request has been received, send final acknowledgement
    } else
    {
        nextByte = UPLOAD_ACK1;
    }

    return SSP2BUF;
}

//char UART_Init(const long int baudrate)
//{
//    unsigned char UART1Config = USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH;
//    unsigned int baud = (_XTAL_FREQ - baudrate*16)/(baudrate*16);   //SPBRG for High Baud Rate
//    //Open1USART(UART1Config, baud);
//    getc1USART(); 
//    RCSTA1bits.CREN = 1;    // Enable receiver 
//    RCSTA1bits.SPEN = 1;    // Enable serial port
//    TXSTA1bits.TXEN = 1;    // Enable transmitter 
//    TXSTA1bits.BRGH = 1;    // High baud rate
//    TXSTA1bits.SYNC = 0;    // Asynchronous mode
//    BAUDCON1bits.BRG16 = 1;
//    SPBRG = 511;
//    DataRdy1USART();
//    return 1;
//}
/* The following USART code was taken from http://www.microchip.com/forums/m608981.aspx 
 * Credit to user roish
 */

// asychronous 8 bit mode only 
void OpenUSART1(unsigned int spbrg)
{ 
    /* Enable interrupt priority */ 
    //RCONbits.IPEN = 1; 
    /* Make receive interrupt high priority */ 
    //IPR1bits.RCIP = 1; 
    /* Enable all high priority interrupts */ 
    //ANSELC=0; 
    //ANSELA=0; 
    //INTCONbits.PEIE_GIEL = 1; 
    //INTCONbits.GIEH = 1; 
    //TRISAbits.RA0 = 0; 
    //TRISAbits.RA1= 0; 
    //PORTAbits.RA0 = 0; 
    //PORTAbits.RA1 = 0; 

    TXSTA1 = 0;          // Reset USART registers to POR state 
    RCSTA1 = 0; 
    SPBRG1 = spbrg; 
    SPBRGH1 = 0; 
    TXSTA1bits.BRGH = 0;  // Baud rate select 
    TXSTA1bits.SYNC = 0;    // Asynchronous mode
    BAUDCON1bits.BRG16 = 0; 
    //PIE1bits.RC1IE = 1; 
    //TRISCbits.RC6 = 1; 
    //TRISCbits.RC7 = 1; 
    RCSTA1bits.CREN = 1;  // Enable receiver 
    TXSTA1bits.TXEN = 1;  // Enable transmitter 
    RCSTA1bits.SPEN = 1; 
}

char getc1USART (void) 
{ 
  char flags; 
  char data;   // Holds received data 
  flags = RCSTA1; 
  data = RCREG1;                      // Read data 
  if (RCSTA1bits.OERR) 
  { 
      RCSTA1bits.CREN = 0; 
      RCSTA1bits.CREN = 1; 
  } 
  return (data);                     // Return the received data 
} 

void putc1USART (char data) 
{ 
  TXREG1 = data;      // Write the data byte to the USART2 
}	 

char DataRdy1USART(void) 
{ 
  if(PIR1bits.RC1IF)  // If RCIF is set 
    return 1;  // Data is available, return TRUE 
  return 0;  // Data not available, return FALSE 
} 

void gets1USART(char *buffer, unsigned char len) 
{ 
  char i;    // Length counter 
  unsigned char data; 

  for(i=0;i<len;i++)  // Only retrieve len characters 
  { 
    while(!DataRdy1USART());// Wait for data to be received 

    data = getc1USART();    // Get a character from the USART 
                           // and save in the string 
    *buffer = data; 
    buffer++;              // Increment the string pointer 
  } 
} 

char Busy1USART(void) 
{ 
  if(!TXSTA1bits.TRMT) // Is the transmit shift register empty 
    return 1;          // No, return FALSE 
  return 0;            // Return TRUE 
} 

void putrs1USART(const rom char *data) 
{ 
  do 
  {  // Transmit a byte 
    while(Busy1USART()); 
    putc1USART(*data); 
  } while( *data++ ); 
} 

void puts1USART( char *data) 
{ 
  do 
  {  // Transmit a byte 
    while(Busy1USART()); 
    putc1USART(*data); 
  } while( *data++ ); 
} 