/*
 * ads_1115.h
 *
 *  Created on: 9. 11. 2020
 *      Author: marcel
 */

#ifndef ADS_1115_H_
#define ADS_ADS_1115_H_

#include "../main.h"

#define ads_i2c_address		0x90

//Config register   Default = 0x8583h  = 1000 0101 1000 0011
#define ADS_ADS_COMP_QUE0		0
#define ADS_COMP_QUE1 			1
#define ADS_COMP_LAT 			2
#define ADS_COMP_POL 			3
#define ADS_COMP_MODE 			4
#define ADS_DR0 				5
#define ADS_DR1 				6
#define ADS_DR2 				7
#define ADS_MODE				8
#define ADS_PGA0				9
#define ADS_PGA1				10
#define ADS_PGA2				11
#define ADS_MUX0				12
#define ADS_MUX1				13
#define ADS_MUX2				14
#define ADS_OS					15

#define ADS_FSR0				0	//±6.144 V
#define ADS_FSR1				1	//±4.096 V
#define ADS_FSR2				2	//±2.048 V (default)
#define ADS_FSR3				3	//±1.024 V
#define ADS_FSR4				4	//±0.512 V
#define ADS_FSR5				5	//±0.256 V
#define ADS_FSR6				6	//±0.256 V
#define ADS_FSR7				7	//±0.256 V


//pointer register
#define ADS_Conversion_register		0
#define ADS_Config_register			1
#define ADS_Lo_thresh_register		2
#define ADS_Hi_thresh_register		3




void ads_init();
void ads_set_gain(uint8_t gain);
//void ads_read_config_register();
int8_t ads_test_address(uint8_t adresa);
void ads_write_register(uint8_t APR, uint16_t data);
uint16_t ads_read_register(uint8_t APR);

extern volatile uint16_t Buf_Config_register;

#endif /* ADS_1115_H_ */
