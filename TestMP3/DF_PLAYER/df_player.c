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
#include <util/delay.h>
#include "df_player.h"
#include "command.h"
#include "../SoftUART/soft_uart.h"
#include "../UART/uart.h"
#include "../main.h"
#include "../SIM800L/sim800l.h"

TCOMMAND send_buff = {{0x7E, 0xFF, 06, 00, 00, 00, 00, 00, 00, 0xEF}};
TCOMMAND recv_buff;


int8_t MP3_play_track(uint16_t track){
	uint8_t *swap = (uint8_t*)(&track);
//	send_buff.Feedback = 1;
	send_buff.para_H = *(swap+1);
	send_buff.para_L = *(swap);
	send_buff.CMD = track_num;
	MP3_send_buffer(&send_buff);
	return 1;
}

int8_t MP3_play_track_folder(uint8_t track,uint8_t folder){
	send_buff.Feedback = 0;
	send_buff.para_H = folder;
	if(track == 0) track = 99;
	send_buff.para_L = track;
	send_buff.CMD = play_folder;
	MP3_send_buffer(&send_buff);
	return 1;
}
int8_t MP3_command(uint8_t command, uint16_t value){
	uint8_t *swap = (uint8_t*)(&value);
	send_buff.Feedback = 0;
	send_buff.para_H = *(swap+1);
	send_buff.para_L = *(swap);
	send_buff.CMD = command;
	MP3_send_buffer(&send_buff);
	return 1;

}

uint8_t MP3_command_queery(uint8_t command,uint16_t value){
	uint8_t znak=0;
	uint8_t idx=0;
	uint8_t *swap = (uint8_t*)(&value);
	send_buff.Feedback = 0;
	send_buff.para_H = *(swap+1);
	send_buff.para_L = *(swap);
	send_buff.CMD = command;
	MP3_send_buffer(&send_buff);
//	_delay_ms(200);
	while(znak!=0xEF){
	znak = suart_getc();
		recv_buff.bytes[idx++] = znak;
		if(idx>10) {
			break;
		}
	}
	return 1;

}

//odeslani bufferu do MP3 modulu
int MP3_send_buffer(TCOMMAND *command){
//	char hex[3];
	checksum(command);
	PORTC|= DIR_conv;
	for (uint8_t i = 0; i<10; i++){
//		itoa(command->bytes[i],hex,16);
//		uart_puts(hex);
//		uart_puts(",");
//		uart_putc(command->bytes[i]);
		suart_putc(command->bytes[i]);
	}
	PORTC&=~DIR_conv;
//	uart_puts("\r\n");
	return 1;

}

//inicializace MP3 modulu
void MP3_init(){
	DDRC|= DIR_conv;
	PORTC&= ~DIR_conv;
	_delay_ms(1);
	MP3_command(device_source,device_init);
	_delay_ms(200);
	MP3_command(volume_level,volume_init);
	_delay_ms(200);
	MP3_command(specify_EQ,_eq_Normal);
	_delay_ms(100);


}
void checksum (TCOMMAND *command){

		command->checksum = 0xFFFF - (command->VER + command->Len +command->CMD + command->Feedback + command->para_H + command->para_L) +1;
		uint8_t *swap = (uint8_t*)(&command->checksum);
		uint8_t i = *swap;
		*swap = *(swap+1);
		*(swap+1) = i;
}
// nacita pozadavky na prehrani mp3 samplu a pak je prehrava pri kazdem zavolani s 0,0
//kontroluje MP3 BUSY pin jestli je 1 pak je mozno prehrat dalsi sampl
//buffer je na 10 pozadavku
int8_t MP3_queue_FIFO_play(uint8_t track, uint8_t folder){			//fifo zasobnik na odesilani samlu pozdeji
	static uint8_t buf_track[10], buf_folder[10];					//je li track folder 0 znamena prehrat
	static uint8_t receive_ptr = 0,send_ptr = 0;								// je li vetsi nez 0 znamena ulozit
	if(folder==255){
		for (uint8_t i = 0;i<10;i++){
			buf_folder[i]=0;
			buf_track[i]=0;
			receive_ptr = 0;
			send_ptr = 0;
		}
	}
	if (track > 0 && folder >0){									// uloz do fifo
		buf_track[receive_ptr] = track;
		buf_folder[receive_ptr] = folder;
		if(++receive_ptr == 10) receive_ptr = 0;
	}
	else {
		if(buf_track[send_ptr]>0 && buf_folder[send_ptr]>0 && (PINC& MP3_ready)){		//je li zasobnik vetsi nez 0 pin na MP3 pripraven
			MP3_play_track_folder(buf_track[send_ptr], buf_folder[send_ptr]);			//prehraj
			buf_track[send_ptr] = 0;													//vynuluj
			buf_folder[send_ptr] = 0;
			if(++send_ptr == 10) send_ptr = 0;											//je li 10 tak je 0
		}																				//jinak preskoc
	}

	return -1;

}

