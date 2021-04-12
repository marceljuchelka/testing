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
#include "ULP/ulp.h"

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
	ULP_init();
	_delay_ms(2000);
	lcd_cls();
	lcd_float(0,0,ULP_promenne_global.M_span,2,_left);
	lcd_float(1,0,ULP_Battery_check(),2,_left);
	lcd_str_al(0,5,"mV",_left);
	lcd_str_al(0,12,"mV",_left);
	_delay_ms(2000);
	lcd_cursor_off();
	ULP_start();
		lcd_cls();
	float PPM_loc;
	lcd_str_P(PSTR("nakl         PPM"));
	while(1){
		PPM_loc = ULP_Vgas_read_PPM();
		lcd_float(1,15,PPM_loc,2,_right);
		lcd_float(1,0,linreg(PPM_loc),2,_left);
//		_delay_ms(300);
//		lcd_int_al(1,5,ULP_pins_U_global.Vgas_U,_left);
//		lcd_int_al(1,15,ULP_pins_U_global.Voffset_U,_right);

//		lcd_show_ADS(vypocet,0);
//		vypocet = ads_U_input_single(ulp_Vref_read);
//		lcd_show_ADS(vypocet,7);
//		lcd_float(1,0,ads_U_input_single(ulp_Vref_read),4,_left);
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

void lcd_float(uint8_t y,uint8_t x,float val, uint8_t len,uint8_t align){
	char buf[17];
	dtostrf(val,7,len,buf);
	lcd_str_al(y,x,buf,align);
}
