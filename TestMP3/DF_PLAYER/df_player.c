/*
 * df_player.c
 *
 *  Created on: 22. 7. 2020
 *      Author: Marcel Juchelka
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include "df_player.h"
#include "command.h"
#include "../SoftUART/soft_uart.h"

TCOMMAND send_buff = {{0x7E, 0xFF, 06, 00, 00, 00, 00, 00, 00, 0xEF}};
TCOMMAND rec_buff;


int8_t MP3_play_track(uint16_t track){
	uint8_t *swap = (uint8_t*)(&track);
//	send_buff.Feedback = 1;
	send_buff.para_H = *(swap+1);
	send_buff.para_L = *(swap);
	send_buff.CMD = track_num;
	MP3_send_buffer(&send_buff);
	return 1;

}
int8_t MP3_command(uint8_t command, uint16_t value){
	uint8_t *swap = (uint8_t*)(&value);
//	send_buff.Feedback = 1;
	send_buff.para_H = *(swap+1);
	send_buff.para_L = *(swap);
	send_buff.CMD = command;
	MP3_send_buffer(&send_buff);
	return 1;

}
int MP3_send_buffer(TCOMMAND *command){
	char hex[3];
	checksum(command);
	for (uint8_t i = 0; i<10; i++){
		itoa(command->bytes[i],hex,16);
		suart_puts(hex);
		suart_puts(",");
//		suart_putint(command->bytes[i]);
	}

	suart_puts("\r\n");
	return 1;

}

void checksum (TCOMMAND *command){

		command->checksum = 0xFFFF - (command->VER + command->Len +command->CMD + command->Feedback + command->para_H + command->para_L) +1;
		uint8_t *swap = (uint8_t*)(&command->checksum);
		uint8_t i = *swap;
		*swap = *(swap+1);
		*(swap+1) = i;
}
