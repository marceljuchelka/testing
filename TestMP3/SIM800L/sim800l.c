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
#include "sim800l.h"
#include "../UART/uart.h"
#include "../MK_LCD/mk_lcd44780.h"
#include "sim_command.h"


const PROGMEM char head_from_sim[][15]={
		{"NO CARRIER"},{"+DTMF:"},{"+CMGR:"},{"RING"},{"+CMTI:"},{"+CLIP:"}
};

EEMEM	char tel_number_init[15] = {"+420608111111"};




void sim800l_init(){
	sim800l_at_com_send(GSM_init,1);	//inicializace
//	_delay_ms(200);
//	uart_puts(GSM_init);			//inicializace
//	_delay_ms(200);
	sim800l_at_com_send(GSM_text_mode,1);	//prepnuti na textove SMS
//	_delay_ms(200);
	sim800l_at_com_send(GSM_DTMF_on,1);	//zapni prijem DTMF
//	_delay_ms(200);
	sim800l_at_com_send(GSM_sms_del_all,1);			//smazat vsechny sms
//	_delay_ms(200);
	sim800l_at_com_send(GSM_signal,1);			//signal gsm?
//	_delay_ms(200);
	sim800l_at_com_send(GSM_echo_off,1);
}


int8_t sim800l_read(){							//navrat commandu
	int8_t len, hlavicka;
	char rx_buf[128];
	len = sim800l_read_uart(rx_buf);
	if(len!=-1){
		hlavicka = sim800l_msg_head(rx_buf);
		sim800l_select_command(rx_buf,hlavicka);
		//		sim800l_select_command(rx_buf);
//		parse_string(rx_buf);
	}
	else return -1;
return 0;
}

int8_t sim800l_msg_head(char *message){						//zjisteni hlavicky sms
	char select_string[20];
	strncpy(select_string,message,19);
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

int8_t sim800l_select_command(char *rx_string,uint8_t hlavicka){
	if(hlavicka == H_NO_C)	lcd_str_al(0,0,"polozeno",_left);
	if(hlavicka == H_RING) sim800l_ringign(rx_string);
	if(hlavicka == H_DTMF) sim800l_dtmf_select(rx_string);
//	if(hlavicka == H_CMGR) sim800l_sms(rx_string);
	if(hlavicka == H_CMTI) sim800l_sms_info(rx_string);

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
	sim800l_at_com_send(GSM_zvedni_hovor,0);
	return 1;
	}
	else lcd_str_al(0,0,"call non autor",_left);
	sim800l_at_com_send(GSM_zvedni_hovor,0);
	return -1;
}

int8_t sim800l_dtmf_select (char *rx_string){									//zjisteni DTMF volby
	char dtmf_select_val = 0;
	char *ptr;
	ptr = strstr_P(rx_string,head_from_sim[H_DTMF]);
	dtmf_select_val = (*(ptr+7) - 48);

	lcd_str_al(0,0,"dtmf =   ",_left);
	lcd_int_al(0,7,dtmf_select_val,_left);

	if (dtmf_select_val>=0 && dtmf_select_val <10) return dtmf_select_val;

	return -1;
}

int8_t sim800l_sms_info(char *rx_string){
//	_delay_ms(200);
	sim800l_at_com_send(GSM_sms_read1,0);				//posli mi SMS      AT+CMGR=1
//	_delay_ms(300);
	sim800l_sms(rx_string);
	return 1;
}

int8_t sim800l_sms(char *rx_string){
	char 	tel_num_sms[15] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	char *ptr_init;
	while(sim800l_read_uart(rx_string)==-1);
//	sim800l_read_uart(rx_string);

	ptr_init = (strstr(rx_string,pars_tel_sms));					//najde parsovaci retezec
	strncpy(tel_num_sms,(ptr_init+3),13);						//zkopiruje sms telefon do telnumsms
	tel_num_sms[13]= 0x20;
	lcd_str_al(1,0,tel_num_sms,_left);
	sim800l_read_uart(rx_string);
	ptr_init = (strstr((rx_string),tel_num_sms));					//zkontroluje jestli se rovna telefon z SMS a obsahu SMS
	if(ptr_init >0) {
		ptr_init = (strstr((rx_string),SMS_init));					//zkontroluje jestli se rovna init z SMS a obsahu SMS
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

int8_t sim800l_read_uart(char *buf){		//precte buff uartu. je li konec tak posle len buferu kdyy nic tak -1
	uint16_t error_znak;
	uint8_t pozice = 0;
	lcd_str_al(1,15,"   ",_right);
	while(pozice<127){
		error_znak = uart_getc();			//int s chybou
		if((error_znak& 0xFF00) && (pozice == 0)) {			//filtruji chybu
				return -1;
		}
		*(buf+pozice) = (char)error_znak;
//		uart_putc(*(buf+pozice));								//echo
		if((*(buf+pozice) == '\n' && *(buf+pozice-1) == '\r')){		//||*(buf+pozice) == 0 *(buf+pozice-1) == 0x0d) &&
			return pozice;
		}
		lcd_int_al(1,15,pozice,_right);
		if(*(buf+pozice)>0)	pozice++;

	}
return -1;
}




void sim800l_tel_num_write(char *telnum){
	eeprom_update_block(telnum,tel_number_init,14);
}

int8_t sim800l_at_com_send(char *command, uint8_t ansver){
	char buf[20];
	int8_t len = 0;
	strcpy(buf,command);
	lcd_str_al(0,0,buf,_left);
	strcat(buf,"\r\n\0");
	uart_puts(buf);
	if(ansver == 1){
//		_delay_ms(100);
		while (sim800l_read_uart(buf) == -1);
		len = sim800l_read_uart(buf);
		sim800l_read_uart(buf);
		if (strstr(buf,"OK\r\n")) lcd_str_al(1,0,buf,_left);
//		lcd_int_al(1,0,len,_left);
	}

return 1;
}


