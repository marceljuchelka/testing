/*
 * main.c
 *
 *  Created on: 21. 8. 2020
 *      Author: marcel
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED			(1<<PD6)
#define PCB_LED 	(1<<PB5)



int main (void){
	uint8_t a,b;
	DDRD|=(LED);
	DDRB|=(PCB_LED);
	TCCR0A|=(1<<COM0A1) | (1<<WGM00) |(1<<WGM01);
	TCCR0B|=(CS00)| (1<<CS02);

	while(1){
		for(a=255;a>125;a--){
			OCR0A=a;
		_delay_ms(10);
		}
		PORTB ^=PCB_LED;
		for(b=125;b<256;b++){
			OCR0A=b;
		_delay_ms(10);
		}

//
	}
}
