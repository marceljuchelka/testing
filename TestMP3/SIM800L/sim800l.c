/*
 * sim800l.c
 *
 *  Created on: 27. 7. 2020
 *      Author: Marcel Juchelka
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include "command.h"
#include "sim800l.h"
#include "../UART/uart.h"
#include "../MK_LCD/mk_lcd44780.h"

const PROGMEM char head_from_sim[][10]={
		{"NO CARRIER"},{"+DTMF:"},{"+CMGR:"},{"RING"},{"+CMTI:"},{"+CLIP:"}
};

EEMEM	char tel_number_init[15] = {"0420608100114"};
		char tel_number[15] = {"0420608100114"};



void sim800l_init(){
	uart_puts(GSM_text_mode);		//prepnuti na textove SMS
	uart_puts(GSM_DTMF_on);			//zapni prijem DTMF

}


int8_t sim800l_read(){							//navrat commandu
	int8_t len, hlavicka;
	char rx_buf[100];
	len = sim800l_read_uart(rx_buf);
	if(len>0){
		hlavicka = sim800_msg_head(rx_buf);
		sim800l_select_command(rx_buf,hlavicka);
		//		sim800l_select_command(rx_buf);
//		parse_string(rx_buf);
	}
	else return -1;
return 0;
}

int8_t sim800l_select_command(char *rx_string,uint8_t hlavicka){

	if(hlavicka == H_RING) sim800l_ringign(rx_string);
	if(hlavicka == H_DTMF) sim800l_dtmf(rx_string);
	if(hlavicka == H_CMGR) sim800l_sms(rx_string);

	return 1;
}


int8_t sim800l_ringign(char *rx_string){
	char tel_num_ring[15];
	parse_string(rx_string,pars_telnum,tel_num_ring);
	lcd_cls();
	lcd_str_al(0,0,"telefon ring",_left);
	lcd_str_al(1,0,tel_num_ring,_left);
	sim800l_tel_num_check(tel_num_ring);
	return -1;
}

int8_t sim800l_dtmf (char *rx_string){
	char dtmf_select = 0;
	parse_string(rx_string,pars_dtfm,&dtmf_select);
	lcd_cls();
	lcd_str_al(0,0,"dtmf",_left);
	lcd_int_al(1,0,dtmf_select,_left);
	return -1;
}

int8_t sim800l_sms(char *rx_string){
	char tel_num_sms[15];
	parse_string(rx_string+10,pars_sms,tel_num_sms);
	lcd_cls();
	lcd_str_al(0,0,"telefon sms",_left);
	lcd_str_al(1,0,tel_num_sms,_left);


	return -1;
}
int8_t parse_string(char *rx_string,char *splitter, char *navrat){
	char buffer[128];
	char *ptr = buffer;
	strcpy(buffer,rx_string);
	ptr = strtok(buffer,splitter);
//	while(*ptr > 0){
	ptr = strtok(NULL,splitter);
	if(strcmp(splitter,pars_telnum) == 0) strcpy(navrat,ptr);
	if(strcmp(splitter,pars_sms) == 0) strcpy(navrat,ptr);
	if(strcmp(splitter,pars_dtfm) == 0) strcpy(navrat,ptr);

//	uart_puts(ptr);
//	uart_putc('\n');
//	}
	return -1;
}

int8_t sim800l_read_uart(char *buf){
	uint16_t error_znak;
	uint8_t pozice = 0;
	while(pozice<100){
		error_znak = uart_getc();
		if(error_znak& 0xFF00) return -1;
		*(buf+pozice) = (char)error_znak;
//		uart_putc(*(buf+pozice));
		if(*(buf+pozice-1) == 0x0d && *(buf+pozice) == 0x0a) {
			*(buf+pozice+1) = '\0';
			return strlen(buf);
		}
		pozice++;
	}
return 0;
}

int8_t sim800_msg_head(char *message){						//zjisteni hlavicky sms
	char select_string[10];
	strlcpy(select_string,message,9);
	char *head;
	uint8_t pozice = 0;
	for (pozice = 0;pozice<6;pozice++){
		head = strstr_P(message, head_from_sim[pozice]);
		if (head > 0){
			return pozice;
		}
	}
	return -1;												//nenasla se hlavicka
}


void sim800l_tel_num_write(char *telnum){
	eeprom_update_block(telnum,tel_number_init,14);
}

int8_t sim800l_tel_num_check(char *telnum){
	eeprom_read_block(tel_number,tel_number_init,14);
	return strcmp(tel_number,telnum);
}
