/*
 * main.c
 *
 *  Created on: 13. 11. 2020
 *      Author: marcel
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "main.h"
#include "MK_I2C/mk_i2c.h"
#include "MK_LCD/mk_lcd44780.h"
#include "ADS_1115/ads_1115.h"

PROGMEM const char build_info[] = "Build:";
PROGMEM const char build_date[] = __DATE__;
PROGMEM const char build_time[] = __TIME__;

int main(void){
	lcd_init();
	lcd_cursor_on();
	lcd_cls();
	lcd_str_al_P(0,0,build_info,_left);
	lcd_str_al_P(0,15,build_time,_right);
	lcd_str_al_P(1,15,build_date,_right);
	ads_init();
	_delay_ms(2000);
	lcd_cls();
	lcd_str_al(0,0,"                ",_left);
	lcd_str_al(0,5,"mV",_left);
	lcd_str_al(0,12,"mV",_left);
	while(1){
		float vypocet = 0.125*((float)ads_read_single_mux(Vgas_read));
		lcd_show_ADS(vypocet,0);
		vypocet = 0.125*(float)ads_read_single_mux(Vref_read);
		lcd_show_ADS(vypocet,7);
	}

}

void lcd_show_ADS(uint16_t hodnota,uint8_t pozice){
	if(hodnota<10)	lcd_str_al(0,pozice,"    ",_left);
	else if(hodnota<100)	lcd_str_al(0,pozice,"   ",_left);
	else if(hodnota<1000)	lcd_str_al(0,pozice,"  ",_left);
	else if(hodnota<10000)	lcd_str_al(0,pozice," ",_left);
	else if(hodnota<100000)lcd_locate(0,pozice);
	lcd_int(hodnota);
}
