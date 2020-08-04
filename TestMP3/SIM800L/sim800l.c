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
		{"NO CARRIE"},{"+DTMF:"},{"+CMGR:"},{"RING"},{"+CMTI:"},{"+CLIP:"}
};

EEMEM	char tel_number_init[15] = {"+420608111111"};




void sim800l_init(){
	char odpoved[15];
	uint8_t len = 0;
	uart_puts(GSM_init);			//inicializace
	lcd_str_al(0,0,GSM_init,_left);
	_delay_ms(1000);
	uart_puts(GSM_text_mode);		//prepnuti na textove SMS
	_delay_ms(100);
	uart_puts(GSM_DTMF_on);			//zapni prijem DTMF
	_delay_ms(100);
	uart_puts(GSM_sms_del_all);			//smazat vsechny sms
	_delay_ms(100);
	uart_puts(GSM_signal);			//signal gsm?
	_delay_ms(100);

}


int8_t sim800l_read(){							//navrat commandu
	int8_t len, hlavicka;
	char rx_buf[128];
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
		pozice++;
	}
return -1;
}

int8_t sim800l_select_command(char *rx_string,uint8_t hlavicka){

	if(hlavicka == H_RING) sim800l_ringign(rx_string);
	if(hlavicka == H_DTMF) sim800l_dtmf_select(rx_string);
//	if(hlavicka == H_CMGR) sim800l_sms(rx_string);
	if(hlavicka == H_CMTI) sim800l_sms_info(rx_string);

	return 1;
}

int8_t sim800l_sms_info(char *rx_string){
	_delay_ms(200);
	uart_puts(GSM_sms_read1);
	_delay_ms(300);
	sim800l_sms(rx_string);
	return 1;
}

int8_t sim800l_ringign(char *rx_string){
	char tel_num_compare[15];
//	parse_string(rx_string,pars_telnum,tel_num_ring);
	eeprom_read_block(tel_num_compare,tel_number_init,sizeof(tel_number_init));
	if (strstr(rx_string,tel_num_compare)) {
	lcd_cls();
	lcd_str_al(0,0,"calling",_left);
	lcd_str_al(1,0,tel_num_compare,_left);
	return 1;
	}
	else lcd_str_al(0,0,"call non autor",_left);

	return -1;
}

int8_t sim800l_dtmf_select (char *rx_string){									//zjisteni DTMF volby
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
	char *ptr_init;
	ptr_init = (strstr(rx_string,pars_tel_sms));					//najde parsovaci retezec
	strncpy(tel_num_sms,(ptr_init+3),13);								//zkopiruje sms telefon do telnumsms
	tel_num_sms[13]= 0x20;
	ptr_init = (strstr((rx_string+32),tel_num_sms));					//zkontroluje jestli se rovna telefon z SMS a obsahu SMS
	if(ptr_init >0) {
		ptr_init = (strstr((rx_string+32),SMS_init));					//zkontroluje jestli se rovna init z SMS a obsahu SMS
		if(ptr_init >0){
			tel_num_sms[13] = 0;
			sim800l_tel_num_write(tel_num_sms);
			lcd_cls();
			lcd_str_al(0,0,"init tlf. cislo",_left);
			lcd_str_al(1,0,tel_num_sms,_left);
		}
	}
return -1;
}


int8_t sim800_msg_head(char *message){						//zjisteni hlavicky sms
	char select_string[10];
	strncpy(select_string,message,10);
	char *head;
	uint8_t pozice = 0;
	for (pozice = 0;pozice<6;pozice++){
		head = strstr_P(select_string, head_from_sim[pozice]);
		if (head > 0){
			return pozice;
		}
	}
	return -1;												//nenasla se hlavicka
}


void sim800l_tel_num_write(char *telnum){
	eeprom_update_block(telnum,tel_number_init,14);
}

