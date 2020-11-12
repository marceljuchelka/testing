/*
 * ads_1115.c
 *
 *  Created on: 9. 11. 2020
 *      Author: marcel
 *
 *      Data Rate Setting   DRS
 */

#include <avr/io.h>
#include "../main.h"
#include "ads_1115.h"
#include "../MK_I2C/mk_i2c.h"

volatile uint16_t Buf_Config_register;
volatile uint8_t ads_OK;



void ads_init(){
	if(ads_test_address(ads_i2c_address)) {
		ads_OK = 1;
		ads_write_register(ADS_Config_register,0x8583);						//reset config
		Buf_Config_register = ads_read_register(ADS_Config_register);
		lcd_hex_al(0,0,Buf_Config_register,_left);
	//	_delay_ms(3000);
		Buf_Config_register |= (1<<ADS_MUX2);
	//	lcd_cls();
		ads_write_register(ADS_Config_register,Buf_Config_register);
		ads_set_gain(ADS_FSR7);
		Buf_Config_register = ads_read_register(ADS_Config_register);
		lcd_hex_al(1,0,Buf_Config_register,_left);
		_delay_ms(10000);
	}

}

void ads_set_gain(uint8_t FSR){
	Buf_Config_register|= ((uint16_t)FSR << 9);
	ads_write_register(ADS_Config_register,Buf_Config_register);
}

int8_t ads_test_address(uint8_t adresa){
	i2c_start();
	if(i2c_write(adresa)) {
		i2c_stop();
		return 1;
	}
	return 0;
}
//void ads_read_config_register(){
//	uint8_t buffer[3];
//	uint8_t *buf = &buffer[0];
//	i2c_send_byte(ads_i2c_address,ADS_Config_register);
//	i2c_read_buf1(ads_i2c_address,2,buf);
//	Buf_Config_register=((buffer[0]<<8) + buffer[1]);
//	lcd_hex_al(1,0,Buf_Config_register,_left);
//	_delay_ms(5000);
//}

uint16_t ads_read_register(uint8_t APR){
	uint8_t buffer[3];
	uint8_t *buf = &buffer[0];
	i2c_send_byte(ads_i2c_address,APR);
	i2c_read_buf1(ads_i2c_address,2,buf);
	return ((buffer[0]<<8) + buffer[1]);

}
void ads_write_register(uint8_t APR, uint16_t data){
	i2c_start();
	i2c_write(ads_i2c_address);
	i2c_write(APR);
	i2c_write( data >> 8 );
	i2c_write( data >> 0 );
	i2c_stop();
}


