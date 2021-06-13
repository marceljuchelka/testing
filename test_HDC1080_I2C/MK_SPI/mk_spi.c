/*
 * mk_spi.c
 *
 *  Created on: 2018-01-10
 *      Author: Miros³aw Kardaœ
 *
 *      Publikacja kodów Ÿród³owych bez zgody autora stanowi naruszenie
 *      praw autorskich i jest zabroniona.
 *      Biblioteka przeznaczona do celów komercyjnych i niekomercyjnych.
 *
 *      MK_SPI Lib ver: 1.3
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "mk_spi.h"



#if USE_SOFT_SPI == 0

void spi_init( void ) {

#if USE_SLAVE_MODE_HW_SPI == 0
        SPI_DIR |=  ( MOSI | SCK | SS );

        SPCR = (1<<SPE) | (1<<MSTR);		// SPI Enable, SPI MASTER Mode
        SPCR |= (SCK_SPEED & SPRxMASK) 	|	// SPI SPEED
        		(SCK_POLARITY << CPOL) 	|	// High or Low Edge in idle
				(SPI_DATA_ORDER << DORD)|	// Data Order: MSB or LSB first
				(SCK_PHASE << CPHA);		// Data Update on First or Second SCK Edge

        SPSR |= ( (SCK_SPEED >> 4) & SPI2XMASK );	// SPI SPEED

        SS_HI;
#else
        SPI_DIR |=  MISO;
        SPCR = (1<<SPE) | (0<<MSTR) | (1<<SPIE);// SPI Enable, SPI SLAVE Mode (enable SPI interrupt)
        SPCR |= (SCK_SPEED & SPRxMASK) 	|		// SPI SPEED
        		(SCK_POLARITY << CPOL) 	|		// High or Low Edge in idle
				(SPI_DATA_ORDER << DORD)|		// Data Order: MSB or LSB first
				(SCK_PHASE << CPHA);			// Data Update on First or Second SCK Edge

        SPSR |= ( (SCK_SPEED >> 4) & SPI2XMASK );	// SPI SPEED
#endif
}
//-----------------------------------------------------------------------------

#if SEND_ONLY == 0
	uint8_t spi_xmit_byte( uint8_t data ) {
#else
	void spi_xmit_byte( uint8_t data ) {
#endif

#if MASTER_SPI_DELAY_US > 0
    _delay_us( MASTER_SPI_DELAY_US );
#endif

    SPDR = data;                    // send byte to SPDR register
    while( !(SPSR & (1<<SPIF)) );  	// wait for data shifting

#if SEND_ONLY == 0
    return SPDR;                    // return data from the SPDR
#endif
}
//-----------------------------------------------------------------------------

#endif


#if USE_SOFT_SPI == 1

void spi_init( void ) {

#if SEND_ONLY == 0
	MISO_DIR &= ~MISO;
#endif
	MOSI_DIR |= MOSI;
	SCK_DIR |= SCK;
	SS_DIR |= SS;

#if SEND_ONLY == 0
	MISO_PORT |= MISO;	// podci¹gamy liniê MISO do VCC
#endif

	MOSI_PORT &= ~MOSI;	// po restarcie stan niski
	if( !SCK_POLARITY ) SCK_PORT &= ~SCK;	// po restarcie stan niski
	else SCK_PORT |= SCK;					// po restarcie stan wysoki
	SS_HI;				// po restarcie stan wysoki
}
//-----------------------------------------------------------------------------

#if SEND_ONLY == 0
	uint8_t spi_xmit_byte( uint8_t byte ) {
#else
	void spi_xmit_byte( uint8_t byte ) {
#endif

#if MASTER_SPI_DELAY_US > 0
    _delay_us( MASTER_SPI_DELAY_US );
#endif

	uint8_t rx_data = 0x00;

#if SPI_DATA_ORDER == 0
	uint8_t cnt = 0x80;
#else
	uint8_t cnt = 0x01;
#endif

	/* wysy³amy bajt do uk³adu Slave - pinami CS trzeba sterowaæ samodzielnie */
	while( cnt ) {


		#if SCK_PHASE == 1									// Trailing (Second) Edge
			if( !SCK_POLARITY ) SCK_HI; else SCK_LO;
			// mo¿na zakomentowaæ jeœli sterujemy szybkim uk³adem np rejestrem przesuwnym
			_delay_us(0.1);									// opóŸnienie miêdzy zboczami
			if( !SCK_POLARITY ) SCK_LO; else SCK_HI;
		#endif

		if( byte & cnt ) MOSI_HI;							// TX nadawanie
		else MOSI_LO;

		#if SPI_DATA_ORDER == 0
			rx_data <<= 1;
			if( (MISO_PIN & MISO) ) rx_data |= 0x01;		// RX odbieranie
		#else
			rx_data >>= 1;
			if( (MISO_PIN & MISO) ) rx_data |= 0x80;		// RX odbieranie
		#endif

		#if SCK_PHASE == 0									// Leading (First) Edge
			if( !SCK_POLARITY ) SCK_HI; else SCK_LO;

				#if SPI_DATA_ORDER == 0				// opóŸnienie miêdzy zboczami
					cnt >>= 1;
				#else
					cnt <<= 1;
				#endif

			if( !SCK_POLARITY ) SCK_LO; else SCK_HI;
		#endif

	}

#if SEND_ONLY == 0
	return rx_data;
#endif

}
//-----------------------------------------------------------------------------

/* Zapis i odczyt przez SPI dowolnej iloœci bitów w zakresie od 1 do 16 !!!
 *    ( przydatne np do obs³ugi pamiêci szeregowych 93Cxx )
*/
uint16_t spi_xmit_n_bits( uint8_t bits_cnt, uint16_t bits_word ) {

    uint16_t res = 0;
    bits_word <<= 16 - bits_cnt;

    for( uint8_t i = bits_cnt; i; i-- ) {

#if SCK_PHASE == 0
    	if( !SCK_POLARITY ) SCK_LO; else SCK_HI;

        if( (bits_word & 0x8000) ) MOSI_PORT |= MOSI;
        else MOSI_PORT &= ~MOSI;
        bits_word <<= 1;

        res <<= 1;
        if( (MISO_PIN & MISO) ) res |= 1;

        if( !SCK_POLARITY ) SCK_HI; else SCK_LO;
#else
    	if( !SCK_POLARITY ) SCK_LO; else SCK_HI;
    	_delay_us(0.1);			// mo¿na zakomentowaæ jeœli sterujemy szybkim uk³adem np rejestrem przesuwnym
    	if( !SCK_POLARITY ) SCK_HI; else SCK_LO;

        if( (bits_word & 0x8000) ) MOSI_PORT |= MOSI;
        else MOSI_PORT &= ~MOSI;
        bits_word <<= 1;

        res <<= 1;
        if( (MISO_PIN & MISO) ) res |= 1;
#endif

        res <<= 1;
    }

    return res;
}
//-----------------------------------------------------------------------------
#endif


void spi_send_word( uint16_t word ) {
	spi_xmit_byte( (word>>8) );
	spi_xmit_byte( (word>>0) );
}
//-----------------------------------------------------------------------------

void spi_send_buf( void * buf_out, uint8_t size ) {

	for( uint8_t i = 0; i < size; i++ ) {
       	spi_xmit_byte( *((uint8_t*)buf_out++) );
    }

}
//-----------------------------------------------------------------------------

/*
 * nadawanie i odbieranie danych do i z - bufora. Bufor mo¿e byæ ten sam, nast¹pi wymiana danych
 *
 * usage example:
 * 	spi_xmit_buf( buf1, buf2, 50);	// send and receive data - 50 bytes
 * 	sspi_xmit_buf( NULL, buf2, 50);	// only send data - 50 bytes
 *  spi_xmit_buf( buf1, NULL, 50);	// only receive data, send only 0xFF 50 times
 *  spi_xmit_buf( NULL, NULL, 50);	// only send ZERO 50 times
 *
 */
void spi_xmit_buf( void * buf_in, void * buf_out, uint8_t size ) {

	uint8_t b_out = 0xFF;
	for( uint8_t i = 0; i < size; i++ ) {
		if( (uint8_t*)buf_out )  b_out = *((uint8_t*)buf_out++);
#if SEND_ONLY == 0
		if( (uint8_t*)buf_in ) *((uint8_t*)buf_in++) = spi_xmit_byte( b_out );
#else
		else spi_xmit_byte( b_out );
#endif

#if SEND_ONLY == 0
		else spi_xmit_byte( b_out );
#endif
	}
}
//-----------------------------------------------------------------------------

void spi_master_puts( char * str ) {

	while( *str ) spi_xmit_byte( *(str++) );

}
//-----------------------------------------------------------------------------

#if SEND_ONLY == 0
char * spi_master_gets( char * buf ) {
	char znak;
	char * wsk = buf;

	while( (znak = spi_xmit_byte(0)) ) {
		if( '\n' == znak || '\r' == znak ) break;
		*(wsk++) = znak;
	}
	*wsk = 0;

	return buf;
}
#endif
//-----------------------------------------------------------------------------


