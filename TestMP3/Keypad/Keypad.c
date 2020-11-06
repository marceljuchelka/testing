/*
 * Keypad.c
 *
 *  Created on: 3. 11. 2020
 *      Author: marcel
 */


#include <avr/io.h>
#include "../main.h"
#include "../main.h"
#include "../Keypad/Keypad.h"

//tlacitka na klavesnici
PROGMEM const uint8_t tlacitko[4][4] =
		{	{1,2,3,10},
			{4,5,6,11},
			{7,8,9,12},
			{13,0,14,15}
		};


void keypad_init(){


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
	char buf[14];
	int8_t vysledek;
	while(1){
		vysledek = key_read();
		if(vysledek == 13) return 1;
	}
return -1;
}

