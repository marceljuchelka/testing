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
#include "../DF_PLAYER/command.h"
#include "../DF_PLAYER/df_player.h"
#include "../main.h"

extern volatile uint8_t sekundy, proces;

const PROGMEM char head_from_sim[][15]={
		{"NO CARRIER"},{"+DTMF:"},{"+CMGR:"},{"+CLIP:"},{"+CMTI:"},{"RING"}
};

EEMEM	char tel_number_init[15] = {"+420608111111"};




int8_t sim800l_init(){
	if(sim800l_at_com_send(GSM_init,1)== -1) return - 1;				//inicializace
	sim800l_at_com_send(GSM_text_mode,1);								//prepnuti na textove SMS
	sim800l_at_com_send(GSM_DTMF_on,1);									//zapni prijem DTMF
	sim800l_at_com_send(GSM_sms_del_all,1);								//smazat vsechny sms
	sim800l_at_com_send(GSM_signal,1);									//signal gsm?
	sim800l_at_com_send(GSM_echo_off,1);
	sim800l_at_com_send(GSM_micr_gain,0);
return 0;
}


int8_t sim800l_read(){							//navrat commandu
	lcd_str_al(0,15,"  ",_right);
	int8_t len, hlavicka;
	char rx_buf[129];
	len = sim800l_read_uart(rx_buf);				//nacte data z UARTu
	if(len!=-1){									//neni li nic tak skoci nazpet
		hlavicka = sim800l_msg_head(rx_buf);		//zjisti podle hlavicky o jakou informaci jde
		sim800l_select_command(rx_buf,hlavicka);
		//		sim800l_select_command(rx_buf);
//		parse_string(rx_buf);
	}
	else {
		lcd_str_al(0,15,"RK",_right);
		return -1;
	}

return 0;
}

int8_t sim800l_msg_head(char *message){						//zjisteni hlavicky dat z SIM800l
	char select_string[20];
	strncpy(select_string,message,19);
	char *head;
	uint8_t pozice = 0;
	for (pozice = 0;pozice<6;pozice++){
		head = strstr_P(select_string, head_from_sim[pozice]);
		if (head > 0){
//			if(pozice == H_CMTI) lcd_str_al(1,0,select_string,_left);
			return pozice;
		}
	}
	return -1;												//nenasla se hlavicka
}

int8_t sim800l_select_command(char *rx_string,uint8_t hlavicka){
	if(hlavicka == H_NO_C)	lcd_str_al(0,0,"polozeno",_left);
	if(hlavicka == H_CLIP) sim800l_ringign(rx_string);
	if(hlavicka == H_DTMF) sim800l_dtmf_select(rx_string);
	if(hlavicka == H_CMGR) sim800l_sms(rx_string);
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
	_delay_ms(300);
	MP3_queue_FIFO_play(sampl_ozone_cleaner_pro,folder_info);
	MP3_queue_FIFO_play(sampl_vyberte_dotaz,folder_info);
	MP3_queue_FIFO_play(menu_cleaner_off,folder_menu);
	while ((sim800l_read_uart(rx_string))> -1);							//aby vyprazdnil uart
	MP3_queue_FIFO_play(menu_proces_minut,folder_menu);
	MP3_queue_FIFO_play(menu_SMS_info_on,folder_menu);

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

	lcd_str_al(0,0,"dtmf =   ",_left);
	lcd_int_al(0,7,dtmf_select_val,_left);

	if (dtmf_select_val>=0 && dtmf_select_val <10) sim800l_dtmf_command(dtmf_select_val);

	return -1;
}

int8_t sim800l_dtmf_command(uint8_t dtmf_val){					//vykonani dtmf prikazu
	if(dtmf_val == 0) dtmf_val = 10;
	if(dtmf_val==dtmf_time_end){
		MP3_queue_FIFO_play(proces+7,folder_info);
//		_delay_ms(2000);
		MP3_queue_FIFO_play(sekundy,folder_minuty);
	}
	if(dtmf_val == dtmf_machine_OFF){
		MP3_queue_FIFO_play(sampl_cleaner_je_vypnut,folder_info);
	}
	if(dtmf_val == dtmf_sms_on_off){
		MP3_queue_FIFO_play(sampl_info_sms_on_off,folder_info);
//		sim800l_sms_send("+420608100114","text\26\0");
	}
return -1;
}

int8_t sim800l_sms_info(char *rx_string){
	sim800l_at_com_send(GSM_sms_read1,0);				//posli mi SMS      AT+CMGR=1
	return 1;
}

int8_t sim800l_sms(char *rx_string){					//inicializacni SMS musi byt ve tvaru "init +420608100114 init" (bez uvozovek)
	char 	tel_num_sms[15] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	char *ptr_init;
	ptr_init = (strstr(rx_string,pars_tel_sms));					//najde parsovaci retezec
	strncpy(tel_num_sms,(ptr_init+3),13);						//zkopiruje sms telefon do telnumsms
	tel_num_sms[13]= 0x20;
	lcd_str_al(0,0,tel_num_sms,_left);
	sim800l_read_uart(rx_string);
	ptr_init = (strstr((rx_string),tel_num_sms+4));					//zkontroluje jestli se rovna telefon z SMS a obsahu SMS
	if(ptr_init >0) {
		strlwr(rx_string);
		ptr_init = (strstr((rx_string),SMS_init));					//zkontroluje jestli se rovna init z SMS a obsahu SMS
		if(ptr_init >0){
			tel_num_sms[13] = 0;
			sim800l_tel_num_write(tel_num_sms);
			lcd_str_al(15,0,"in",_right);
			lcd_str_al(1,0,tel_num_sms,_left);
		}
	}
	sim800l_at_com_send(GSM_sms_del_all,1);
	return -1;
}

int8_t sim800l_read_uart(char *buf){		//precte buff uartu. je li konec tak posle len buferu kdyy nic tak -1
	uint16_t error_znak;
	uint8_t pozice = 0;
//	uint32_t delay = 0;
//	lcd_str_al(1,15,"   ",_right);
	while(pozice<128){
		error_znak = uart_getc();			//int s chybou
		if((error_znak& 0xFF00) && (pozice == 0)) {			//filtruji chybu
				return -1;
		}
		*(buf+pozice) = (char)error_znak;
//		uart_putc(*(buf+pozice));								//echo
		if((*(buf+pozice) == '\n' && *(buf+pozice-1) == '\r')){		//||*(buf+pozice) == 0 *(buf+pozice-1) == 0x0d) &&
			*(buf+pozice+1) = '\0';
			return pozice;
		}
//		lcd_int_al(1,15,pozice,_right);
		if(*(buf+pozice)>0)	pozice++;

	}
return -1;
}




void sim800l_tel_num_write(char *telnum){
	eeprom_update_block(telnum,tel_number_init,14);
}

int8_t sim800l_at_com_send(char *command, uint8_t ansver){
	char buf[20];
//	int8_t len = 0;
	strcpy(buf,command);
//	lcd_str_al(0,0,buf+3,_left);
	strcat(buf,"\r\n\0");
	PORTC|= DIR_conv;
	uart_puts(buf);
	while(1){
		if (!(UCSR0B& (1<<UDRIE0))) break;
	}
	PORTC&= ~DIR_conv;
	if(ansver == 1){
		_delay_ms(10);
//		while (sim800l_read_uart(buf) == -1);
		if(sim800l_read_uart(buf) == -1) return -1;
//		sim800l_read_uart(buf);
		if (strstr(buf,"OK")) return 1;
		if (strstr(buf,"ERROR")) return -1;
	}

return 0;
}

int8_t sim800l_sms_send(char* tel_num, char *text){
	char buf[23];
	strcpy(buf,GSM_send_sms_num);
	eeprom_read_block(buf+9,tel_number_init,13);
	*(buf+21) = '\"';
	*(buf+22) = '\0';
	sim800l_at_com_send(buf,0);
	uart_puts(text);
	_delay_ms(50);




	return -1;
}
