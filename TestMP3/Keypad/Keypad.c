/*
 * Keypad.c
 *
 *  Created on: 3. 11. 2020
 *      Author: marcel
 */


#include <avr/io.h>
#include "../main.h"
#include "../Keypad/Keypad.h"

//tlacitka na klavesnici
PROGMEM const uint8_t tlacitko[4][4] =
		{	{1,2,3,10},
			{4,5,6,11},
			{7,8,9,12},
			{13,0,14,15}
		};
uint8_t klav_OK;

void keypad_init(){
	if(key_test_address(1)){
		klav_OK = 1;
	}



}

int8_t key_read(){
	uint8_t vertikal = L1, horizontal = 0, i,vysledek=0;
	for (i=0;i<4;i++){
//		lcd_int_al(0,10,i,_left);
		vertikal = (vertikal=L1)<<i;
//		lcd_bin_al(0,0,~vertikal&0xF0,8,_left);
		pcf8574_write(Key_PCF8574_addr,~vertikal);
		horizontal =((~(pcf8574_read(Key_PCF8574_addr)))&0x0F);
		if(horizontal >0){
//			lcd_bin_al(1,0,horizontal,8,_left);
			vysledek = vypocet_tlacitka(~horizontal,~vertikal&0xF0);
			return vysledek;
		}
	}
	return -1;
}
int8_t vypocet_tlacitka(uint8_t radek,uint8_t sloupec){
	uint8_t i;
	sloupec = sloupec>>4;
	for(i=0;i<4;i++){
		if(!(radek& (1<<i))){
			radek = i;
			break;
		}
	}
	for(i=0;i<4;i++){
		if(!(sloupec& (1<<i))){
			sloupec = i;
			break;
		}
	}
	lcd_str_al(0,13,"   ",_left);
	return pgm_read_byte(&(tlacitko[sloupec][radek]));

}

int8_t key_read_str(char *ptr){ 						//nacti cislo 13 znaku do stringu a uloz * = 13
	int8_t vysledek,pozice = 0;
	while(key_read()>=0);
	lcd_cls();
	lcd_str_al(0,0,"zadej SN:",_left);
	lcd_locate(1,0);
	while(1){
		vysledek = key_read();
		if(vysledek == 14 || pozice >10) {
			lcd_cls();
			return 1;
		}
		if(vysledek >=0 && vysledek<10){
			while(key_read()>=0);
			*(ptr+pozice++) = vysledek+48;
			*(ptr+pozice) = '\0';
			lcd_str_al(1,0,ptr,_left);
		}
	}
return -1;
}

int8_t key_sn_enter(){
	char buffer[15];
	char *ptr = buffer;
	int8_t vysledek,pozice = 0;
	while(key_read()>=0);
	lcd_cls();
	lcd_str_al(0,0,"zadej SN:",_left);
	lcd_locate(1,0);
	while(1){
		vysledek = key_read();
		if(vysledek == 14 || pozice >10) {
			lcd_cls();
			return 1;
		}
		if(vysledek >=0 && vysledek<10){
			while(key_read()>=0);
			*(ptr+pozice++) = vysledek+48;
			*(ptr+pozice) = '\0';
			lcd_str_al(1,0,ptr,_left);
		}
	}
return -1;
}

int8_t key_test_address(uint8_t adresa){
	adresa = Key_PCF8574_addr;
	i2c_start();
	if(i2c_write(adresa)) {
		i2c_stop();
		return 1;
	}
	return 0;
}
