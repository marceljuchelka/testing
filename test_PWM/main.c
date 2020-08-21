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
	DDRD|=(LED);
	DDRB|=(PCB_LED);
	TCCR0A|=(1<<COM0A1) | (1<<WGM00) |(1<<WGM01);
	TCCR0B|=(CS00)| (1<<CS02);

	while(1){
		for(int8_t a=0;a<256;a++){
			OCR0A=a;
		_delay_ms(10);
		}
		PORTB ^=PCB_LED;
	}
}
