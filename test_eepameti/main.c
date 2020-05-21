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


void odesli( unsigned char data );

int main(void){
	uart_init();

	UBRR0 = 103;
	UCSR0B = (1<<TXEN0);  // zapnout vysílaè
// 1 stop bit,žádná parita, 8bit zpráva
	UCSR0C =  (1<<UCSZ01) | (1<<UCSZ00);
	DDRD = 0;// vstup pro tlaèítko
	DDRB |=	(1<<PB5);
	PORTB |= (1<<PB5);
	char text[]= "AHOJ";

uint32_t rychlost = 1000;
	while(1){

		if (!(PIND& (1<<PD7))) rychlost = 250000;
		else rychlost = 1000000;
		PORTB^= (1<<PB5);
		while(rychlost--);
uart_putchar(&text,'\n');
	}

}

void odesli( unsigned char data ){

}
