/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* General */
#define nop() 		_asm nop _endasm	/* no-op */
#define NULL		0x00
/* SPI */
#define SPI_SCK		PORTBbits.RB1//LATCbits.LATC3		/* Clock pin, PORTB pin 0, PORTC pin 3 */
#define SPI_SI		PORTBbits.RB2//LATCbits.LATC4//PORTCbits.RC4//		/* Serial input pin, PORTB pin 1, PORTC pin 4 */
#define SPI_SO		LATBbits.LATB3//LATCbits.LATC5//		/* Serial output pin, PORTB pin 2, PORTC pin 5 */
#define SPI_CSN		PORTBbits.RB0//LATAbits.LATA5//		/* CSN output pin, PORTB pin 3, PORTC pin 2 */
#define SPI_CE		LATAbits.LATA6//LATBbits.LATB4		/* CE output pin, PORTB pin 4, PORTC pin 1 */
//#define SPI_IRQ		PORTBbits.RB0		/* IRQ input pin, PORTB pin 0 */
#define SPI_SCALE	4					/* postscaling of signal */
#define SYNC_SEQ	0x2C8				/* SPI synchronize sequence sent before data */

// MSSP1: SPI1 (MCP923S17 + LCD / Serial EEPROM ) 
 #define    SPI1_TRIS    TRISCbits 
 #define    SPI1_CLK    RC3     
 #define    SPI1_SDI    RC4 
 #define    SPI1_SDO    RC5 
 
 // INIT I/O for MSSP1 SPI bus, includes I/O for attached devices 
 #define    INIT_SPI1_IO()    {SPI_SCK=0; SPI_SO=0; SPI_SI=1;} 
 
 // ------------------ 
 // SPI Configuration 
 // ------------------ 
 
 // MSSP1: SPI1 Config and control 
//#define   SPI_FOSC_4    0b00000000              // SPI Master mode, clock = Fosc/4
//#define   SPI_FOSC_16   0b00000001              // SPI Master mode, clock = Fosc/16
//#define   SPI_FOSC_64   0b00000010              // SPI Master mode, clock = Fosc/64
//#define   SPI_FOSC_TMR2 0b00000011              // SPI Master mode, clock = TMR2 output/2
#define   SLV_SSON      0b00000100              // SPI Slave mode, /SS pin control enabled
//#define   SLV_SSOFF     0b00000101              // SPI Slave mode, /SS pin control disabled
 #define    SPI1_CONFIG         SLV_SSON    // Slave, SS control enabled 
 #define    SPI1_Enable()       {SSP2CON1bits.SSPEN=1;} 
 #define    SPI1_Disable()		{SSP2CON1bits.SSPEN=0;} 
 // SPI1 interrupt control 
 #define SPI1_IntEnable()		{PIR1bits.SSP1IF = 0; PIE1bits.SSP1IE=1; INTCONbits.PEIE = 1;}
 #define SPI1_IntDisable()    	{PIE1bits.SSP1IE=0;}
 
 // Init MSSP1 in SPI mode and I/O pins 
 #define    SPI1_Init()            {SSP2STAT = 0x40; SSP2CON1 = SLV_SSON;}

//2F 00101111
//78 01111000
//5E 01011110

/* TODO Application specific user parameters used in user.c may go here */
#define UPLOAD_REQ0 0xAB
#define UPLOAD_REQ1 0xCD
#define UPLOAD_ACK0	0xEF
#define UPLOAD_ACK1 0x01

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

unsigned char SpiRead(void);
void SpiWrite(unsigned char byte);
unsigned char spi_Send_Read(unsigned char byte);
