/*                  e-gadget.header
 * main.c
 *
 *  Created on: 10.06.2021
 *    Modyfied: 10.06.2021 15:38:32
 *      Author: Marcel Juchelka
 *
 * Project name: "test_HDC1080_I2C"
 *
 *
 *          MCU: ATmega328P
 *        F_CPU: 8 000 000 Hz
 *
 *    Flash: 3 588 bytes   [ 10,9 % ]
 *      RAM:  11 bytes   [ 0,5 % ]
 *   EEPROM:  0 bytes   [ 0,0 % ]
 *
 */


#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "MK_I2C/mk_i2c.h"
#include "MK_LCD/mk_lcd44780.h"
#include "Keypad/Keypad.h"
#include "MJ_HDC1080/hdc1080.h"
#include "MK_PRESSURE_HUMIDITY_LIB/mk_pressure_cfg.h"
#include "MJ_SHT21/sht21.h"
#include "MJ_AM2320B/mj_am2320b.h"

void testI2C();
void lcd_float(uint8_t y,uint8_t x,float val, uint8_t len,uint8_t align);

enum{_false,_true};

int main (void){
	uint16_t test_swap = 0x1234;
	int8_t teplota = 0;
	uint8_t desetiny_t = 0 , desetiny_v, vlhkost;
	lcd_init();
//	testI2C();
//	lcd_hex_al(1, 0, hdc1080_read_register(Configuration), _left);
	hcd1080_init();
	mk_press_hum_init();
	while(1){
		lcd_str_al_P(0, 0, PSTR("T "), _left);
		lcd_str_al_P(1, 0, PSTR("H "), _left);
//		lcd_hex_al(0, 0, hcd1080_read_temp(),_left);

//		lcd_bin_al(0, 0, hdc1080_read_register(Configuration),16, _left);
		lcd_float(0, 2, hcd1080_read_temp(),1,_left);
//		lcd_int_al(0, 2, hcd1080_read_temp(),_left);
		lcd_float(1, 2, hcd1080_read_hum(),1,_left);
		lcd_int_al(0, 7, am2320_getdata(temperat), _left);
		lcd_int_al(1, 7, am2320_getdata(humidy), _left);
		mkp_read_temp(st_hih6131, &teplota, &desetiny_t);
		_delay_ms(50);
		mkp_read_temp(st_hih6131, &teplota, &desetiny_t);
		lcd_int_al(0, 10, teplota, _left);
		lcd_str_P(PSTR("."));
		lcd_int(desetiny_t);
		mkp_read_humidity(st_hih6131, &vlhkost, &desetiny_v);
		lcd_int_al(1, 10, vlhkost, _left);
		lcd_str_P(PSTR("."));
		lcd_int(desetiny_v);
	_delay_ms(3000);
	lcd_cls();
	}
}

void testI2C(){
	lcd_str_al_P(0,0,PSTR("TEST I2C"),_left);
	_delay_ms(2000);
	lcd_cls();

	for(uint8_t i = 0;i<0xff;i++){
		lcd_hex_al(0,0,i,_left);
		if(key_test_address(i)) {
			lcd_str_al_P(1,0,PSTR("adresa:"),_left);
			lcd_hex_al(1,9,i,_left);
			_delay_ms(3000);
		}
	}
}

void lcd_float(uint8_t y,uint8_t x,float val, uint8_t len,uint8_t align){
	char buf[17];
	dtostrf(val,2,len,buf);
	lcd_str_al(y,x,buf,align);
}



