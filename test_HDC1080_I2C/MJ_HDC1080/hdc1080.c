/*                  e-gadget.header
 * hdc1080.c
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
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "hdc1080.h"
#include "../MK_I2C/mk_i2c.h"
#include "../MK_LCD/mk_lcd44780.h"

uint16_t swap_uint16(uint16_t swap_num){
	uint8_t byte_A;
	SWAP16 byte_swap;
	byte_swap.BYTES2 = swap_num;
	byte_A = byte_swap.MSB;
	byte_swap.MSB = byte_swap.LSB;
	byte_swap.LSB = byte_A;
	return byte_swap.BYTES2;
}

void hdc1080_write_register(uint8_t set_register, uint16_t reg_value){
i2c_start();
i2c_write(hdc_1080_address + _i2c_write);
i2c_write(set_register);
i2c_write(reg_value>>8);
i2c_write(reg_value);
i2c_stop();
}

void hdc1080_set_register(uint8_t set_register){
	i2c_start();
	i2c_write(hdc_1080_address + _i2c_write);
	i2c_write(set_register);
	i2c_stop();
}


uint16_t hdc1080_read_register(uint8_t set_register){
	uint16_t res = 0;
	uint8_t * buf = (uint8_t*)&res;
	uint8_t len = 2;
	hdc1080_set_register(set_register);
	i2c_start();
	i2c_write(hdc_1080_address + _i2c_read);
	while (len--) *buf++ = i2c_read( len ? ACK : NACK );
	i2c_stop();
	return swap_uint16(res);
}


void hcd1080_init(){
	uint16_t conf_register = hdc1080_read_register(Configuration);
	conf_register|= (1<<TRES) | (HumMR_11_bit << HRES) | (TempMR_11_bit << TRES);
	hdc1080_write_register(Configuration, conf_register);
}


float hcd1080_read_temp(){
	uint16_t temp = 0;
//	uint32_t temp32 = 0;
	float temp_F = 0;
	hdc1080_set_register(Temperature);
	_delay_ms(8);
	temp = i2c_read_word(hdc_1080_address);
	temp = swap_uint16(temp);
	temp_F = ((((float)temp)/(float)65536)*(float)165)-(float)40;
//	temp32 = ((uint32_t)temp * 165 /65536)-40;
//	temp = (uint16_t) temp_F;
	return temp_F;
}


float hcd1080_read_hum(){
	uint16_t hum = 0;
//	uint32_t hum32 = 0;
	hdc1080_set_register(Humidity);
		_delay_ms(8);
	hum = i2c_read_word(hdc_1080_address);
	hum = swap_uint16(hum);
//	hum32 = ((uint32_t)hum*100l/65536l);
	float hum_F  = ((float)hum*100)/65536;;
 	return hum_F;
}





