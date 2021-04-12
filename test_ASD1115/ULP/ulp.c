/*
 * ulp.c
 *
 *  Created on: 17. 5. 2020
 *      Author: Marcel Juchelka
 */


#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "ulp.h"
#include "../ADS_1115/ads_1115.h"


volatile ULP_pins_U ULP_pins_U_global;
volatile ULP_var_struct ULP_promenne_global;

/*inicializace ULP*/
void ULP_init(){
	ads_i2c_address = ads_i2c_add_0x90;
	ads_init();
	ULP_promenne_global.sens_code = Sens_code_def;
	ULP_promenne_global.M_span=(((float)ULP_promenne_global.sens_code/(float)1000) * (float)499)/(float)1000;				//vypocet mV/PPM
}

/*vypocet PPM*/
float ULP_vypocet_ppm(uint16_t Vref,uint16_t Vgas){
float PPM;
PPM = ((float)Vref - (float)Vgas)/ULP_promenne_global.M_span;
//return ((float)_Vref - (float)Vgas)/M_spam;
PPM = round(PPM*100)/100;
if(PPM < 0) PPM = 0;
return PPM;
}


/*kontrola baterie*/
float ULP_Battery_check(){										//kontrola baterie je li pod 1400 (2.8V) vraci -1
	ULP_pins_U napeti;
	napeti.Vref_U = ads_U_input_single(Vref_read);					//napeti Vgas v mV
	if(napeti.Vref_U < U_Bat_min) return ulp_error;
return napeti.Vref_U*2;
}


/* nacitani hodnot po startu*/
int8_t ULP_start(){
	ULP_pins_U pracovni;
	uint32_t VGas_prumer = 0;
	uint8_t i;
	pracovni.Vref_U = ads_U_input_single(ulp_Vref_read);
	for(i = 0;i<64;i++){
	VGas_prumer = VGas_prumer + ads_U_input_single(ulp_Vgas_read);
	}
	pracovni.Vgas_U = VGas_prumer/i;
	pracovni.Voffset_U = pracovni.Vref_U - pracovni.Vgas_U;
	if(pracovni.Voffset_U < 0) pracovni.Voffset_U = 0;
	ULP_pins_U_global = pracovni;
	return ulp_enable;
}

float ULP_Vgas_read_PPM(){
	float PPM;
	PPM = (ULP_pins_U_global.Vref_U - ads_U_input_cont(ulp_Vgas_read))/ULP_promenne_global.M_span;
	if(PPM<0)PPM = 0;
	return PPM;
}

/* vypocet naklonu krivky */
float linreg(float ppm) {
#define pocetvpoli 16
	static float pole_PPM[pocetvpoli];
	uint8_t n = pocetvpoli;
		for(uint8_t ii = 0; ii<pocetvpoli-1; ii++){
			pole_PPM[ii] = pole_PPM[ii+1];
		}
		pole_PPM[pocetvpoli-1] = ppm;

    float   sumx = 0.0;                      /* sum of x     */
    float   sumx2 = 0.0;                     /* sum of x**2  */
    float   sumxy = 0.0;                     /* sum of x * y */
    float   sumy = 0.0;                      /* sum of y     */

    for (int i=0;i<n;i++){
		sumx  += i;
		sumx2 += i*i;
		sumxy += i * pole_PPM[i];
		sumy  += pole_PPM[i];
    }

    float denom = (n * sumx2 - sumx*sumx);

    if (denom == 0) {
//		printf("Sing.");

    }

    return (n * sumxy  -  sumx * sumy) / denom;
}
