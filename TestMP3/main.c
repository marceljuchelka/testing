/*
 * main.c
 *
 *  Created on: 21. 7. 2020
 *      Author: Marcel Juchelka
 */


#include <avr/io.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "MK_I2C/mk_i2c.h"
#include "MK_LCD/mk_lcd44780.h"
#include "SoftUART/soft_uart.h"
#include "DF_PLAYER/df_player.h"


int main(void){
	lcd_init();
	suart_init();
	lcd_str("MP3....");
	uint16_t pocitadlo = 0;
	_delay_ms(1000);
	lcd_cls();
	while(1){
for(pocitadlo = 0; pocitadlo<100;pocitadlo++){
	lcd_int_al(15,0,pocitadlo,_right);
	suart_putint(pocitadlo);
	_delay_ms(500);
}

	}
}
