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
//#include <stdint.h>
#include "system.h"

extern unsigned char roverUploadReq, canSendUART;
unsigned char nextByte;
unsigned char startCollection, stopCollection;
unsigned char PORTA_shadow, PORTB_shadow, PORTC_shadow;
unsigned char uploadReq0, uploadReq1;


/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* Define global vars */
    roverUploadReq = 0;
    canSendUART = 0;
    nextByte = 0;
    startCollection = 0;
    stopCollection = 0;
    uploadReq0 = 0;
    uploadReq1 = 0;
    
    // SRAM
    PORTA_shadow = 0x00;
    PORTB_shadow = 0x00;
    PORTC_shadow = 0x00;

    /* TODO Initialize User Ports/Peripherals/Project here */

    /* Setup analog functionality and port direction */

    /* Initialize peripherals */

    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */

    /* Enable interrupts */
}

void uploadToLand() {
    // Upload to land station, if we have permission
    //  while we wait for the ADC reading
    if (canSendUART)
    {
        unsigned char data;
        int i;
        for (i = 0; i < 1024; i++)
        {
            data = sram_read(i);
            putc1USART(data);
            delay(100);
        }
        canSendUART = 0;
    } 
}

unsigned char SpiRead(void)
{
	/* Read a byte, send dummy byte */
    return spiSendRead(0x00);

}

void SpiWrite(unsigned char byte)
{
	/* Send a byte, ignore read value */
	spiSendRead(byte);
}

unsigned char spiSendRead(unsigned char byte)
{
//    SSP2BUF = nextByte;
//    while (!SSP2STATbits.BF);
//    if (!uploadReq0 && !uploadReq1)
//    {
//        uploadReq0 = (SSP2BUF == UPLOAD_REQ0);
//        nextByte = UPLOAD_ACK1; //byte;
//    } else if (uploadReq0 && !uploadReq1)
//    {
//        uploadReq1 = (SSP2BUF == UPLOAD_REQ1);
//        nextByte = byte; //UPLOAD_ACK0;
//    } else 
//    {
//        nextByte = UPLOAD_ACK0; //UPLOAD_ACK1;
//    }
    
    SSP2BUF = nextByte;
    while (!SSP2STATbits.BF);
    
    // Check for upload request
    if (SSP2BUF == UPLOAD_REQ)
    {
        // Upload request has been received, send acknowledgement
        nextByte = UPLOAD_ACK;
        roverUploadReq = 1;
    // Check for UART commands to relay to rover
    } else if (startCollection)
    {
        // Send command to rover
        nextByte = START_RX;
        startCollection = 0;
    } else if (stopCollection)
    {
        // Send command to rover
        nextByte = STOP_RX;
        stopCollection = 0;
    } else
    {
        nextByte = byte;
    }

    return SSP2BUF;
}

char readcUSART()
{
    char c = getc1USART();
    // Check for commands from land station
    if (c == START_RX)
    {
        putc1USART(START_ACK);
        //puts1USART("Data collection started\n\0");
        startCollection = 1;
    } else if (c == STOP_RX)
    {
        putc1USART(STOP_ACK);
        //puts1USART("Data collection started\n\0");
        stopCollection = 1;
    } else if (c == UPLOAD_REQ)
    {
        putc1USART(UPLOAD_ACK);
        canSendUART = 1;
    }
    return c;
}

/* The following USART code was taken from http://www.microchip.com/forums/m608981.aspx 
 * Credit to user roish
 */
// asynchronous 8 bit mode only 
void OpenUSART1(unsigned int rate)
{ 
    // Configure interrupts
    PIE3bits.RC2IE = 1;
    INTCONbits.PEIE_GIEL = 1;
    INTCONbits.GIEH = 1;
    // Reset USART registers to POR state
    TXSTA1 = 0;
    RCSTA1 = 0;
    // Set baud rate
    SPBRG1 = rate;
    SPBRGH1 = 0;
    TXSTA1bits.BRGH = 1;    // Baud rate select
    TXSTA1bits.SYNC = 0;    // Asynchronous mode
    TXSTA1bits.TX9 = 1;     // Enable 9th bit for parity
    BAUDCON1bits.BRG16 = 0;
    RCSTA1bits.ADDEN = 0;
    RCSTA1bits.CREN = 1;    // Enable receiver 
    TXSTA1bits.TXEN = 1;    // Enable transmitter 
    RCSTA1bits.SPEN = 1;    // Enable USART1
}

char getc1USART(void) 
{ 
  char flags;
  char data;        // Holds received data
  flags = RCSTA1;
  data = RCREG1;    // Read data
  RCREG1 = 0x00;
  if (RCSTA1bits.OERR)
  { 
      RCSTA1bits.CREN = 0;
      RCSTA1bits.CREN = 1;
  } 
  return (data);    // Return the received data 
}

void putc1USART(char data) 
{ 
    // Compute and transmit an even parity bit as 9th bit
    TXSTA1bits.TX9D = getEvenParity(data);
    // Write the data byte to the USART1
    TXREG1 = data;
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

void putrs1USART(const char *data) 
{ 
  do 
  {  // Transmit a byte 
    while(Busy1USART()); 
    putc1USART(*data); 
  } while( *data++ ); 
} 

void puts1USART(char *data) 
{ 
  do 
  {  // Transmit a byte 
    while(Busy1USART()); 
    putc1USART(*data); 
    ++data;
    delay(100);
  } while(*data != '\0'); 
} 

/* Code adapted from user rpg7 on
 * http://www.microchip.com/forums/m587239.aspx */
char getEvenParity(char data)
{
    // 10101010 ^ 00010011 = 10111001
    // 10111001 ^ 00100111 = 10011110
    // 10011110 & 00000001 = 00000000
    data ^= ((data >> 4) | (data << 4));
    data ^= (data >> 2);
    data ^= (data >> 1);
    return (data & 0x01);
}

unsigned char sram_read(unsigned int address)
{
    PORTB_shadow = PORTB_shadow | (1 << 7);
    PORTB = PORTB_shadow;
    // we = 1
    PORTC_shadow = PORTC_shadow | (1 << 4);
    PORTC = PORTC_shadow;
    
    address_select(address);
    unsigned char data = get_data_p2s_register();
    
    // oe = 0
//    PORTA_shadow = PORTA_shadow & ~(1 << 3);
//    PORTA = PORTA_shadow;
//    // oe = 1
//    PORTA_shadow = PORTA_shadow | (1 << 3);
//    PORTA = PORTA_shadow;
    return data;

}

void sram_write(unsigned int data, unsigned int address)
{
    
    PORTB_shadow = PORTB_shadow & ~(1 << 7);
    PORTB = PORTB_shadow;
    // oe = 1
    PORTC_shadow = PORTC_shadow | (1 <<5);
    PORTC = PORTC_shadow;
    address_select(address);
    set_s2p_shift_register(data);
    // we = 0
    PORTC_shadow = PORTC_shadow & ~(1 << 4);
    PORTC = PORTC_shadow;
    delay(1000);
    // we = 1
    PORTC_shadow = PORTC_shadow | (1 << 4);
    PORTC = PORTC_shadow;
    delay(1000);
    return;
}

void address_select(unsigned int n) {
        PORTA_shadow = PORTA_shadow  & 0x80;
        PORTA_shadow = PORTA_shadow | (n & 0x7F);
        PORTB_shadow = (PORTB_shadow & ~(0x30)) | ((n >> 4) & 0x30);
        PORTA = PORTA_shadow;
        PORTB = PORTB_shadow;
        delay(1000);
        return;
}

unsigned char get_data_p2s_register() {
    // oe = 0
    PORTC_shadow = PORTC_shadow & ~(1 << 5);
    PORTC = PORTC_shadow;
    delay(1000);
    // PL = 0
    PORTC_shadow = PORTC_shadow & ~(1 << 2);
    PORTC = PORTC_shadow;
    delay(1000);
    // PL = 1;
    PORTC_shadow = PORTC_shadow | (1 << 2);
    PORTC = PORTC_shadow;
    delay(1000);
     // oe = 1
//    PORTA_shadow = PORTA_shadow | (1 << 3);
//    PORTA = PORTA_shadow;
    int i;
    unsigned char data = 0x00;// = PORTAbits.RA4;
    int i;
    for (i = 0; i < 8; i++) {
        data = data | (PORTCbits.RC3 << i);
        toggle_buffer_clock();
    }
    
    // oe = 1
    PORTC_shadow = PORTC_shadow | (1 << 5);
    PORTC = PORTC_shadow;
    return data;
    
}

void set_s2p_shift_register(unsigned int data) {
    int i;
    unsigned char serial_out = 0;
    int shift = 7;
    for (i = 0; i < 8; i++) {
        serial_out = (data >> shift) & 0x1;
        PORTC_shadow = (PORTC_shadow & ~(1 << 1)) | (serial_out << 1);
        PORTC = PORTC_shadow;
        toggle_buffer_clock();
        shift = shift - 1; 
    }
    return;
}

void toggle_buffer_clock() {
    PORTC_shadow = PORTC_shadow | 1;
    PORTC = PORTC_shadow;
    delay(1000);
    PORTC_shadow = PORTC_shadow & ~1;
    PORTC = PORTC_shadow;
    delay(1000);
    return;
}
