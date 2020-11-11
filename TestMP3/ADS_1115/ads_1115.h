/*
 * ads_1115.h
 *
 *  Created on: 9. 11. 2020
 *      Author: marcel
 */

#ifndef ADS_1115_H_
#define ADS_ADS_1115_H_

#include "../main.h"

#define ads_i2c_address		0x48

//Config register   Default = 8583h  = 1000 0101 1000 0011
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


//pointer register
#define ADS_Conversion_register		0
#define ADS_Config_register			1
#define ADS_Lo_thresh_register		2
#define ADS_Hi_thresh_register		3





void ads_set_gain(uint8_t gain);
void ads_read_config_register();

#endif /* ADS_1115_H_ */
