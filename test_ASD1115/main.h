/*
 * main.h
 *
 *  Created on: 13. 11. 2020
 *      Author: marcel
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "MK_I2C/mk_i2c.h"
#include "MK_LCD/mk_lcd44780.h"
#include "ADS_1115/ads_1115.h"

void lcd_show_ADS(uint16_t hodnota,uint8_t pozice);
void lcd_float(uint8_t y,uint8_t x,float val, uint8_t len,uint8_t align);

#define Vgas_read  	ADS_MUX4
#define Vref_read	ADS_MUX5



#endif /* MAIN_H_ */
