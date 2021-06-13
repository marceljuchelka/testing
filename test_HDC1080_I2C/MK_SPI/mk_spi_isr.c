/*
 * mk_spi_isr.c
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


#if USE_SLAVE_MODE_HW_SPI == 1 && USE_SOFT_SPI == 0

THWSPI_CALLBACK_ISR spi_hw_callback_isr;

void register_spi_hw_callback_isr( THWSPI_CALLBACK_ISR callback ) {
	spi_hw_callback_isr = callback;
}

/*
 * PROCEDRUA obs³ugi przerwania SPI w trybie SLAVE
 * mo¿na j¹ modyfikowaæ wg w³asnych potrzeb
 *
 */
ISR( SPI_STC_vect ) {

// --------- mo¿na pomin¹æ ten blok i napisaæ w³asn¹ obs³ugê SLAVE -----------------------
	uint8_t reply = 0;
    uint8_t cmd = SPDR;

    if( spi_hw_callback_isr ) reply = spi_hw_callback_isr( cmd );
    SPDR = reply;
// ----------------------------------------------------------------------------------------

}

#endif

