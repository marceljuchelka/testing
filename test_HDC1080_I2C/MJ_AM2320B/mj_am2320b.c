/*
 * mj_am2320b.c
 *
 *  Created on: 1. 10. 2020
 *      Author: marcel
 */


#include <avr/io.h>
#include <stdlib.h>
#include "../MK_I2C/mk_i2c.h"
#include "mj_am2320b.h"
#include <util/delay.h>


int am2320_getdata(uint8_t select)
{
	uint16_t temp, humid, crc;
	uint8_t t_hi, t_lo; 						//, t_lo, h_hi,h_lo ;
	int teplota;
	i2c_start();								//probuzeni sensoru 800us az 3ms
	i2c_write(AM2320B_address + I2C_w);
	i2c_stop();
	//while ((TWSR & 0b11111000) == 72);
	_delay_us(900);                            // kdyz tak povol 9.5.2020

	i2c_start();								//start i2c prenosu
	i2c_write(AM2320B_address + I2C_w);			//zapise na adresu zarizeni 0
	i2c_write(0x03);							//prikaz 03 = cteni udaju z registru
	i2c_write(0x00);							// od kter0 adresy
	i2c_write(0x04);							// kolik byte
	i2c_stop();									//stop i2c prenosu
	i2c_start();								//start i2c prenosu
	i2c_write(AM2320B_address +I2C_r);			//dresa zarizeni + 1 tzn, ze se bude cist
	t_hi = i2c_read(ACK);						//nacti 0 b
	t_lo = i2c_read(ACK);						//nacti 1 b
	humid = i2c_read(ACK);						//nacti 2 b
	humid = (humid << 8 ) + i2c_read(ACK);		// roluj vlevo a nacti 3 b
	temp = i2c_read(ACK);		//				// nacti 4 byte
	temp = (temp << 8) + i2c_read(ACK);			// roluj vlevo a nactio 5 b
	crc = i2c_read(ACK);						// nacti CRC
	crc = (crc << 8) + i2c_read(NACK);			// roluj vlevo a nacti CRC
	i2c_stop();									//stop i2c prenosu

//	t_hi = t_hi-t_lo;
//	temp = (((t_hi << 8) + t_lo));
//	humid = ((h_hi << 8) + h_lo);
	if (temp & 0x8000) teplota = 0 - (temp & 0x7fff);	// zaporna teplota?
	else teplota = (int)temp;							//kladna teplota
	if (select == temperat) return teplota/10;			//navrat teploty / 10
	else return humid/10;								//navrat vlhkosti /10

	return AM2320_ERROR;								//jinak navrat error
	}
