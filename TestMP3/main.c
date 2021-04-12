/*
 * main.c
 *
 *  Created on: 21. 7. 2020
 *      Author: Marcel Juchelka
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
#include "SoftUART/soft_uart.h"
#include "DF_PLAYER/df_player.h"
#include "DF_PLAYER/command.h"
#include "SIM800L/sim_command.h"
#include "UART/uart.h"
#include "SIM800L/sim800l.h"
#include "MJ_AM2320B/mj_am2320b.h"
#include "ADS_1115/ads_1115.h"

volatile uint8_t sekundy=1, proces=1;
PROGMEM const char build_info[] = "Build:";
PROGMEM const char build_date[] = __DATE__;
PROGMEM const char build_time[] = __TIME__;
volatile uint8_t pocitadlo_sekundy;
volatile uint16_t milisekundy = 100;
char sernum_buffer[15];
float linreg(float ppm);


int main(void){

//	DDRC = 0;
//	PORTC = 0;
//	uint16_t vysledek = 0;
	DDRC|= DIR_conv;
	PORTC&=~DIR_conv;
	lcd_init();
	lcd_cursor_on();
	suart_init();
	uart_init(UART_BAUD_SELECT(38400,8000000UL));
	sei();
	MP3_init();
	MP3_play_track_folder(sampl_ozone_cleaner_pro,folder_info);
	lcd_cls();
	lcd_str_al_P(0,0,PSTR("TEST MP3"),_left);
	_delay_ms(1000);
	lcd_cls();
	lcd_str_al_P(0,0,build_info,_left);
	lcd_str_al_P(0,15,build_time,_right);
	lcd_str_al_P(1,15,build_date,_right);
	_delay_ms(2000);
	keypad_init();
//	ads_init();
//	lcd_str_al(0,0,"                ",_left);
//	lcd_str_al(0,5,"mV",_left);
//	lcd_str_al(0,12,"mV",_left);
//	while(1){
//		float vypocet = 0.1875*((float)ads_read_single_mux(ADS_MUX0));
//		lcd_show_ADS(vypocet,0);
//		vypocet = 0.1875*(float)ads_read_single_mux(ADS_MUX3);
//		lcd_show_ADS(vypocet,7);
//	}





	sim800l_at_com_send(GSM_reset,0);
	_delay_ms(2000);
	sim800l_check();

	while(1){
		linreg(1.23);
		sekundy--;
		lcd_int_al(0,12,sekundy,_right);
		if(sekundy<10)lcd_int_al(0,11,0,_right);
		lcd_int_al(0,9,proces,_right);
		sim800l_read();
		MP3_queue_FIFO_play(0,0);
		while(milisekundy--){
			if(klav_OK){
				int8_t tlacitko = key_read();
				if(tlacitko >=0)	{
					lcd_str_al(0,0,"   ",_left);
					lcd_int_al(0,0,tlacitko,_left);
				}
				if(tlacitko == 10) {
					(key_sn_enter());
				}
			}
			_delay_ms(10);
		}
		milisekundy = 80;


		if(sekundy == 0) {
			proces++;
			if (proces==3)proces = 0;
			sekundy = 60;

		}
		sim800l_signal_value_icon();
//		lcd_int_al(0,0,necinnost,_left);

//		i2c_init(100);
//		int8_t teplota = am2320_getdata(temperat);
//		_delay_ms(10);
//		lcd_str_al(1,15,"   ",_right);
//		lcd_int_al(1,15,teplota,_right);
//
//		_delay_ms(10);
//		int8_t vlhkost = am2320_getdata(humidy);
//		i2c_init(800);
//		lcd_str_al(1,12,"  ",_right);
//		lcd_int_al(1,12,vlhkost,_right);

	}
}

void lcdprint_buffer(){
	lcd_cls();
	lcd_str("CMD:");
	lcd_hex(recv_buff.CMD);
	lcd_locate(1,0);
	lcd_str("DH:");
	lcd_hex(recv_buff.para_H);
	lcd_str(" DL:");
	lcd_hex(recv_buff.para_L);
}

void MP3_test(){
	suart_init();
	MP3_init();
	lcd_str("MP3....");
	uint16_t pocitadlo = 0;
	lcd_cls();
	MP3_init();
	MP3_command_queery(number_of_track_sd,0);
	lcdprint_buffer();
	_delay_ms(2000);
	for(pocitadlo = 0; pocitadlo<60;pocitadlo++){
		lcd_int_al(15,0,pocitadlo,_right);
		_delay_ms(100);
		MP3_play_track_folder(sampl_ozone_cleaner_pro,folder_info);
		lcdprint_buffer();
		_delay_ms(2000);
		MP3_play_track_folder(sampl_vyberte_dotaz,folder_info);
		lcdprint_buffer();
		_delay_ms(2000);
		MP3_play_track_folder(sampl_do_konce_ukonceni_zbyva,folder_info);
		lcdprint_buffer();
		_delay_ms(2000);
//			MP3_play_track(pocitadlo);
		MP3_play_track_folder(59-pocitadlo,folder_minuty);

		_delay_ms(10000);
		}

}

uint8_t i2c_scan(){
	uint8_t i=0xff;
	while(i--){
		i2c_start();
		if(i2c_write(i)){
			i2c_stop();
			lcd_hex_al(1,0,i,_left);
			_delay_ms(3000);
		}
	}
return 0;
}

int8_t i2c_test_address(uint8_t adresa){
	i2c_start();
	if(i2c_write(adresa)) {
		i2c_stop();
		return 1;
	}
	return 0;
}

void lcd_show_ADS(uint16_t hodnota,uint8_t pozice){
	if(hodnota<10)	lcd_str_al(0,pozice,"    ",_left);
	else if(hodnota<100)	lcd_str_al(0,pozice,"   ",_left);
	else if(hodnota<1000)	lcd_str_al(0,pozice,"  ",_left);
	else if(hodnota<10000)	lcd_str_al(0,pozice," ",_left);
	else if(hodnota<100000)lcd_locate(0,pozice);
	lcd_int(hodnota);
}

float linreg(float ppm) {
//    float y[7]= {4, 6, 15, 12, 34, 68}; //11.4571
	static float pole_PPM[6] = {6,5,4,3,2,1};
	uint8_t n = 6,ii;
		for(ii = 0; ii<6; ii++){
			pole_PPM[ii] = pole_PPM[ii+1];
		}
		pole_PPM[5] = ppm;

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
