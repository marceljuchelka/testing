/*
 * main.c
 *
 *  Created on: 21. 5. 2020
 *      Author: Marcel Juchelka
 */

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "UART/uart.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

void copy_progmem2ram(void *kam_ram, void* odkud_prog);
void copy_eemem2ram(void *kam_ram, void *odkud_eemem, uint8_t delka);
void copy_ram2eemem(void *kam_eemem, void *odkud_ram, uint8_t delka);
uint8_t test_eeprom_data(void* odkud_eemem, uint8_t delka);
void uart_put_i(int hodnota);

typedef struct {
	uint32_t	motohod_min;		//celkovy pocet minut provozu
	uint32_t	ozon_min;			//celkovy pocet minut ozonu
	uint16_t	ozon_def;			//kontrolni hodnota namereneho ozonu pri kalibraci
	uint16_t	time_def;			//kontrolni cas pro kalibraci
	uint32_t	serial_num;			//seriove cislo vyrobku
} STATIS_DATA;

PROGMEM	const 	STATIS_DATA Data_default = {1,2,1645,4,5};		//promenne v programove pameti
EEMEM			STATIS_DATA	Data_save_to_eeprom = {1,2,1645,4,5};					//promenne v EEPROM
				STATIS_DATA		statistika;								//pracovni promenna
PROGMEM	const	char  	progdata[20]={" progmem data "};
PROGMEM	const	char  	eepdata_def[20]={" eeprom data_def "};
EEMEM			char	eepdata[20]={" eeprom data "};
				char	ramdata[20]={" ram data "};



int main(void){
uart_init(UART_BAUD_SELECT(9600,16000000));
sei();
	UBRR0 = 103;
	DDRD = 0;// vstup pro tlaèítko
	DDRB |=	(1<<PB5);
	PORTB |= (1<<PB5);
	char text[]= " AHOJ ";

uint32_t rychlost = 1000;
//copy_progmem2ram(&ramdata,&progdata);
//copy_eemem2ram(&ramdata,&eepdata);
	while(1){

		if (!(PIND& (1<<PD7))) {
			rychlost = 2500000;
			if(test_eeprom_data(&eepdata, sizeof(eepdata)) == 0){
				copy_progmem2ram(&ramdata,&eepdata_def);
				copy_ram2eemem(&eepdata,&ramdata, sizeof(ramdata));
			}
			copy_eemem2ram(&ramdata,eepdata,sizeof(eepdata));
		}
		else {
			rychlost = 1000000;
			copy_progmem2ram(&ramdata,&progdata);
//			eeprom_read_block(ramdata,eepdata,sizeof(eepdata));
		}
		PORTB^= (1<<PB5);
		while(rychlost--);
//		uart_puts_p(progdata);
//		uart_puts(text);
		uart_puts(ramdata);
		uart_put_i(test_eeprom_data(&eepdata,sizeof (eepdata)));




	}

}
//kopiruje z progmem do ram (kam, odkud)
void copy_progmem2ram(void* kam_ram, void* odkud_prog){
	strcpy_P(kam_ram,odkud_prog);
}

//kopiruje z eeprom do ram (kam,odkud,delka) delka sizeof
void copy_eemem2ram(void *kam_ram, void *odkud_eemem, uint8_t delka){
	eeprom_read_block((void*)kam_ram,(void*)odkud_eemem,delka);
}

//kopiruje z ram do eeprom (kam,odkud,delka) delka sizeof
void copy_ram2eemem(void *kam_eemem, void *odkud_ram, uint8_t delka){
	eeprom_update_block((void*)odkud_ram,(void*)kam_eemem, delka);
}


//test obsazeni dat v eeprom pameti (odkud, delka) navraci 1 kdyz je obsazena 0 kdyz prazdna;
uint8_t test_eeprom_data(void *odkud_eemem, uint8_t delka){
	uint8_t i, ram[delka];
	uint8_t *ptr = ram;
	eeprom_read_block(ram,(void*)odkud_eemem,delka);
	for(i=0;i<delka;i++){
		if(*ptr == 0xff) break;
	}
	if( i==delka)return 1;
	else return 0;
}

//prevod
void uart_put_i(int hodnota){
	char buf[10];
	itoa(hodnota,buf,10);
	uart_puts(buf);
}
