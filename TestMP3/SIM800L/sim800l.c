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

EEMEM	char tel_number_init[] = {"+420608111111"};
		char tel_number[] = {"+420608111111"};



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
	if(hlavicka == H_DTMF) sim800l_dtmf_select(rx_string);
	if(hlavicka == H_CMGR) sim800l_sms(rx_string);

	return 1;
}


int8_t sim800l_ringign(char *rx_string){
	char tel_num_ring[15];
//	parse_string(rx_string,pars_telnum,tel_num_ring);

	lcd_cls();
	lcd_str_al(0,0,"telefon ring",_left);
	lcd_str_al(1,0,tel_num_ring,_left);
	sim800l_tel_num_check(tel_num_ring);
	return -1;
}

int8_t sim800l_dtmf_select (char *rx_string){
	char dtmf_select_val = 0;
	char *ptr;
	ptr = strstr_P(rx_string,head_from_sim[H_DTMF]);
	dtmf_select_val = (*(ptr+7) - 48);
	lcd_cls();
	lcd_str_al(0,0,"dtmf = ",_left);
	lcd_int(dtmf_select_val);

	if (dtmf_select_val>=0 && dtmf_select_val <10) return dtmf_select_val;

	return -1;
}

int8_t sim800l_sms(char *rx_string){
	char 	tel_num_sms[15] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
//	char	tel_num_init[15]= "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	char *ptr_init;
	ptr_init = (strstr(rx_string,pars_tel_sms));					//najde parsovaci retezec
	strncpy(tel_num_sms,(ptr_init+3),13);								//zkopiruje sms telefon do telnumsms
	ptr_init = (strstr((rx_string+32),tel_num_sms));					//zkontroluje jestli se rovna telefon z SMS a obsahu SMS
//	strncpy(tel_num_init,(ptr_init+2),13);								//zkopiruje sms telefon do telnuminit
	if(ptr_init >0) sim800l_tel_num_write(tel_num_sms);

	//	parse_string(rx_string+10,pars_sms,tel_num_sms);


	lcd_cls();
	lcd_str_al(0,15,"ini",_right);
	lcd_str_al(1,15,"sms",_right);
	lcd_str_al(0,0,ptr_init,_left);
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


//	uart_puts(ptr);
//	uart_putc('\n');
//	}
	return -1;
}

int8_t sim800l_read_uart(char *buf){
	uint16_t error_znak;
	uint8_t pozice = 0;
	while(pozice<128){
		error_znak = uart_getc();
		if(error_znak& 0xFF00) {
			*(buf+pozice) = 0;
			return strlen(buf);
		}
		*(buf+pozice) = (char)error_znak;
//		uart_putc(*(buf+pozice));
//		if(*(buf+pozice-1) == 0x0d && *(buf+pozice) == 0x0a) {
//			*(buf+pozice+1) = '\0';
//			return strlen(buf);
//		}
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
