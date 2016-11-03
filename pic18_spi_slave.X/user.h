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
 
 // INIT I/O for MSSP1 SPI bus, includes I/O for attached devices 
 #define    INIT_SPI1_IO()    	{SPI_SCK=0; SPI_SO=0; SPI_SI=1;} 
 
 // ------------------ 
 // SPI Configuration 
 // ------------------ 
 
#define   SLV_SSON      0b00000100              // SPI Slave mode, /SS pin control enabled
 #define    SPI1_CONFIG         SLV_SSON    // Slave, SS control enabled 
 #define    SPI1_Enable()       {SSP2CON1bits.SSPEN=1;} 
 #define    SPI1_Disable()		{SSP2CON1bits.SSPEN=0;} 
 // SPI1 interrupt control 
 //#define SPI1_IntEnable()		{PIR1bits.SSP1IF = 0; PIE1bits.SSP1IE=1; INTCONbits.PEIE = 1;}
 //#define SPI1_IntDisable()    	{PIE1bits.SSP1IE=0;}
 
 // Init MSSP1 in SPI mode and I/O pins 
 #define    SPI1_Init()         {SSP2STAT = 0x40; SSP2CON1 = SLV_SSON;}

/* TODO Application specific user parameters used in user.c may go here */
/* Commands */
#define START_RX 	0x53  // S: Start data collection
#define START_ACK 	0x54  // T
#define STOP_RX 	0x45  // E: End data collection
#define STOP_ACK 	0x46  // F
#define UPLOAD_REQ 	0x55  // U: Upload permission
#define UPLOAD_ACK	0x56  // V
#define UPLOAD_REQ0 	0xC2
#define UPLOAD_ACK0	0xC3
#define UPLOAD_REQ1 	0xD2
#define UPLOAD_ACK1	0xD3

/* USART */
#define _XTAL_FREQ 	20000000
#define BAUDRATE 	19200

/* SPI */
#define SPI_IDLE 0x01

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
/* SRAM */
void sram_write(unsigned int data, unsigned int address);
unsigned char sram_read(unsigned int address);

/* SPI */
unsigned char SpiRead(void);
void SpiWrite(unsigned char byte);
unsigned char spiSendRead(unsigned char byte);

/* USART */
char readcUSART(void);
void OpenUSART1(unsigned int spbrg);
char getc1USART(void);
void putc1USART(char data);
char DataRdy1USART(void);
void gets1USART(char *buffer, unsigned char len);
char Busy1USART(void);
void putrs1USART(const char *data);
void puts1USART(char *data);

char getEvenParity(char data);
void address_select(unsigned int n);
unsigned char get_data_p2s_register();
void set_s2p_shift_register(unsigned int data);
void toggle_buffer_clock(void);
void delay(unsigned long s);

