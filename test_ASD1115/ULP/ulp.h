/*
 * ulp.h
 *
 *  Created on: 17. 5. 2020
 *      Author: Marcel Juchelka
 */

#ifndef ULP_ULP_H_
#define ULP_ULP_H_

#include "../ADS_1115/ads_1115.h"

#define ULP_ADS_address 		ads_i2c_add_0x90				//adresa prevodniku pro ULP

#define Vgas_pin 	1			//vystup Vgas na senzoru ULP
#define Vref_pin 	2			//vystup Vref na senzoru ULP
#define Vtemp_pin	3			//vystup Vtemp na senzoru ULP

#define ulp_Vgas_read  	ADS_MUX4	//hodnota z Vgas
#define ulp_Vref_read	ADS_MUX5	//hodnota z Vref
#define ulp_Vtemp_read	ADS_MUX6	//hodnota z Vtemp

#define 	U_Bat_min	1400		//minimalni napeti baterie (Vref * 2)
#define 	_TIAgain	499
#define 	PPM_sel_def		3
#define 	Sens_code_def	42100		//53000	49320	46030				//hodnota sensivity code ze senzoru
#define 	M_span_def		(((float)Sens_code_def/1000)*((float)_TIAgain))/1000		//defaultni napeti		mV/PPM

enum {ulp_error = -1,ulp_disable,ulp_enable};

typedef struct{
	float Vgas_U;
	float Vref_U;
	float Vtemp_U;
	float Voffset_U;
} ULP_pins_U;					//napeti hna pinech ULP

typedef struct{
	float 		M_span;				//vypoctene mV/PPM
	uint32_t	sens_code;			//citlivost z QR code ze sensoru
} ULP_var_struct;

extern volatile ULP_pins_U ULP_pins_U_global;
extern volatile ULP_var_struct ULP_promenne_global;

float ULP_vypocet_ppm(uint16_t Vref,uint16_t Vgas);
float ULP_Battery_check();
int8_t ULP_start();
void ULP_init();
float ULP_Vgas_read_PPM();
float linreg(float ppm);

#endif /* ULP_ULP_H_ */
