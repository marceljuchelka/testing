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

volatile uint16_t Buf_Config_register;



void ads_init(uint8_t samply, uint8_t DRS){

}

void ads_set_gain(uint8_t gain){


}

void ads_read_config_register(){
	uint8_t conf_register[3];
	i2c_start();
	i2c_write(ads_i2c_address);
	i2c_write(ADS_Config_register);
	i2c_stop();
	i2c_start();
	i2c_write(ads_i2c_address);
//	i2c_read();


	lcd_hex_al(1,0,conf_register[0],_left);
	lcd_hex_al(1,5,conf_register[1],_left);

	_delay_ms(5000);
}
