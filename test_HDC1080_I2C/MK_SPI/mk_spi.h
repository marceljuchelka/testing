/*
 * mk_spi.h
 *
 *  Created on: 2018-01-10
 *      Author: Miros³aw Kardaœ
 *
 *      Publikacja kodów Ÿród³owych bez zgody autora stanowi naruszenie
 *      praw autorskich i jest zabroniona.
 *      Biblioteka przeznaczona do celów komercyjnych i niekomercyjnych.
 *
 *      MK_SPI Lib ver: 1.3
 */

#ifndef MK_SPI_MK_SPI_H_
#define MK_SPI_MK_SPI_H_

//********************************************** SPI CONFIG ******************
#define USE_SOFT_SPI 		0	// 1-tryb Software SPI, 0-tryb Hardware SPI

#define SEND_ONLY			0	// 1-jeœli maj¹ byæ tylko wysy³ane dane, linia MISO nie u¿ywana (tylko dla trybu MASTER)

/* PINY dla SPI, w przypadku SOFT SPI mog¹ byæ dowolne na dowolnym porcie! */
#define aSS           		PB2                     // SS (CS Chip Select)
#define aMOSI    			PB3                     // MOSI pin
#define aMISO    			PB4                     // MISO pin
#define aSCK     			PB5                     // SCK pin


#define MOSI_DIR			DDRB	// jeœli u¿ywasz hardware SPI
#define MOSI_PORT			PORTB	// wystarczy skonfigurowaæ tylko te dwie linie

//------------------------------- czêœæ konfiguracji tylko na potrzeby soft spi ---
#define MISO_DIR			DDRB
#define MISO_PORT			PORTB
#define MISO_PIN			PINB

#define SCK_DIR				DDRB
#define SCK_PORT			PORTB
//-------------------------------- koniec ------------------------

#define SS_DIR				DDRB
#define SS_PORT				PORTB


/* Sck Speed options
 * 		SCK_FOSC_DIV_4		(default)
 * 		SCK_FOSC_DIV_16
 * 		SCK_FOSC_DIV_64
 * 		SCK_FOSC_DIV_128
 *
 * 		SCK_FOSC_DIV_2
 * 		SCK_FOSC_DIV_8
 * 		SCK_FOSC_DIV_32
 */
#define SCK_SPEED				SCK_FOSC_DIV_32	// taktowanie SPI

#define	SPI_DATA_ORDER			MSB_FIRST		// MSB_FIRST 					(default), LSB_FIRST
#define SCK_POLARITY 			0				// 0-Low when idle 				(default), 1-High when idle
#define SCK_PHASE				0				// 0-Data Sampled on First Edge (default), 1-Data Sampled on Second Edge

#define MASTER_SPI_DELAY_US		0

//----------------------------------- SPI CONFIG as SLAVE only in hardware mode -------------------

#define USE_SLAVE_MODE_HW_SPI	0

//********************************************** SPI CONFIG END ***********************************








//-----------------------------------------------------------------------------

typedef uint8_t (*THWSPI_CALLBACK_ISR)( uint8_t byte );


#ifdef SPCR0
	#define SPCR	SPCR0
	#define SPSR	SPSR0
	#define SPIF    SPIF0
	#define SPDR	SPDR0
	#define SPE		SPE0
	#define MSTR	MSTR0
	#define CPOL	CPOL0
	#define DORD	DORD0
	#define CPHA	CPHA0
#endif




#define SPRxMASK			0x03
#define SPI2XMASK			0x01

#define MSB_FIRST			0x00
#define LSB_FIRST			0x01

#define SCK_FOSC_DIV_4		0x00
#define SCK_FOSC_DIV_16		0x01
#define SCK_FOSC_DIV_64		0x02
#define SCK_FOSC_DIV_128	0x03

#define SCK_FOSC_DIV_2		0x10
#define SCK_FOSC_DIV_8		0x11
#define SCK_FOSC_DIV_32		0x12

#define SPI_DIR		 		MOSI_DIR
#define SPI_PORT     		MOSI_PORT
#define SPI_PIN      		MISO_PIN

#define SS           		(1<<aSS)                     // SS (CS Chip Select)
#define MOSI    			(1<<aMOSI)                   // MOSI pin
#define MISO    			(1<<aMISO)                   // MISO pin
#define SCK     			(1<<aSCK)                    // SCK pin


#define SS_LO				SS_PORT &= ~SS
#define SS_HI				SS_PORT |= SS

#define SCK_LO				SCK_PORT &= ~SCK
#define SCK_HI				SCK_PORT |= SCK

#define MOSI_LO				MOSI_PORT &= ~MOSI
#define MOSI_HI				MOSI_PORT |= MOSI

#define MISO_LO				MISO_PORT &= ~MISO
#define MISO_HI				MISO_PORT |= MISO






//-----------------------------------------------------------------------------

/* inicjalizacja SPI */
void spi_init( void );

#if USE_SLAVE_MODE_HW_SPI == 1 && USE_SOFT_SPI == 0
	void register_spi_hw_callback_isr( THWSPI_CALLBACK_ISR callback );
#endif


#if SEND_ONLY == 0
	/* nadawanie i odbiór jednego bajtu */
	uint8_t spi_xmit_byte( uint8_t byte );
#else
	/* nadawanie jednego bajtu */
	void spi_xmit_byte( uint8_t byte );
#endif

/* nadawanie s³owa 16-bitowego */
void spi_send_word( uint16_t word );

/* nadawanie danych z bufora */
void spi_send_buf( void * buf_out, uint8_t size );

/* nadawanie i odbieranie danych do i z - bufora. Bufor mo¿e byæ ten sam, nast¹pi wymiana danych */
void spi_xmit_buf( void * buf_in, void * buf_out, uint8_t size );

/* wys³anie stringa */
void spi_master_puts( char * str );

/* odebranie stringa (zakoñczonego CR, albo LF albo NULL */
char * spi_master_gets( char * buf );

#if USE_SOFT_SPI == 1
	uint16_t spi_xmit_n_bits( uint8_t bits_cnt, uint16_t bits_word );
#endif

#endif /* MK_SPI_MK_SPI_H_ */
