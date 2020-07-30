/*
 * main.c
 *
 *  Created on: 21. 7. 2020
 *      Author: Marcel Juchelka
 */


#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "main.h"
#include "MK_I2C/mk_i2c.h"
#include "MK_LCD/mk_lcd44780.h"
#include "SoftUART/soft_uart.h"
#include "DF_PLAYER/df_player.h"
#include "DF_PLAYER/command.h"
#include "UART/uart.h"


int main(void){
	lcd_init();

	uart_init(UART_BAUD_SELECT(9600,8000000UL));\
	sei();
	uint8_t pozice = 0;
	char znak;
	char tx_buf[]="toto je testovaci text\n\r";
	char rx_buf[32];

	uart_puts(tx_buf);
	uint8_t pocetznaku;
	lcd_cls();
	lcd_str("start ...");
	_delay_ms(2000);
	lcd_cls();

	while(1){
		znak = uart_getc();

		if(((znak & 0xff00) == 0)) {
			if (znak& 0x00ff){
				rx_buf[pozice] = znak;
				uart_puts(&rx_buf[pozice]);
				lcd_char(rx_buf[pozice++]);
				}
//			else{
//				lcd_cls();
//				lcd_str(rx_buf);
//				pozice = 0;
//			}
		}







	}
}

void lcdprint_buffer(){
	lcd_cls();
	lcd_str("CMD:");
	lcd_hex(recv_buff.CMD);
	lcd_locate(1,0);
	lcd_str("DH:");
	lcd_hex(recv_buff.para_H);
	lcd_str(" DL:");
	lcd_hex(recv_buff.para_L);
}

void MP3_test(){
	suart_init();
	MP3_init();
	lcd_str("MP3....");
	uint16_t pocitadlo = 0;
	lcd_cls();
	MP3_init();
	MP3_command_queery(number_of_track_sd,0);
	lcdprint_buffer();
	_delay_ms(2000);
	for(pocitadlo = 0; pocitadlo<60;pocitadlo++){
		lcd_int_al(15,0,pocitadlo,_right);
		_delay_ms(100);
		MP3_play_track_folder(sampl_ozone_cleaner_pro,folder_info);
		lcdprint_buffer();
		_delay_ms(2000);
		MP3_play_track_folder(sampl_vyberte_dotaz,folder_info);
		lcdprint_buffer();
		_delay_ms(2000);
		MP3_play_track_folder(sampl_do_konce_ukonceni_zbyva,folder_info);
		lcdprint_buffer();
		_delay_ms(2000);
//			MP3_play_track(pocitadlo);
		MP3_play_track_folder(59-pocitadlo,folder_minuty);

		_delay_ms(10000);
		}

}
