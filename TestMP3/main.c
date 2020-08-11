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
#include "SIM800L/sim_command.h"
#include "UART/uart.h"
#include "SIM800L/sim800l.h"

volatile uint8_t sekundy=1, proces=1;


int main(void){

	lcd_init();
	lcd_cursor_on();
	suart_init();
	MP3_init();


	uart_init(UART_BAUD_SELECT(9600,8000000UL));
	sei();
	MP3_play_track_folder(sampl_ozone_cleaner_pro,folder_info);
	_delay_ms(2000);
	lcd_cls();
	lcd_str("start");
	_delay_ms(2000);
	if(sim800l_init() == -1){
		lcd_cls();
		lcd_str("neni modul");
		while(1);
	}




	while(1){
		sekundy--;
		lcd_int_al(0,12,sekundy,_right);
		if(sekundy<10)lcd_int_al(0,11,0,_right);
		lcd_int_al(0,9,proces,_right);
		sim800l_read();
		MP3_queue_FIFO_play(0,0);

		_delay_ms(1000);
		if(sekundy == 0) {
			proces++;
			if (proces==3)proces = 0;
			sekundy = 60;
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
