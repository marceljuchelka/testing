/*
 * df_player.h
 *
 *  Created on: 22. 7. 2020
 *      Author: Marcel Juchelka
 */

#ifndef DF_PLAYER_H_
#define DF_PLAYER_H_

//init nastaveni
#define volume_init 	30						//nastaveni hlasitosti
#define device_init		1						//TF sdcard
#define pin_busy 		PC2						//pin pripojeny na busy vystup MP3
#define MP3_ready		(1<<pin_busy)			//makro  je li 1 je ready
//#define MP3_ready		if(pinc& MP3_busy)		//makro - pripraven prehrava



typedef union{
	uint8_t bytes[10];
	struct {
		uint8_t SS;
		uint8_t VER;
		uint8_t Len;
		uint8_t CMD;
		uint8_t Feedback;
		uint8_t para_H;
		uint8_t para_L;
		uint16_t checksum;
		uint8_t End_bit;
	};
} TCOMMAND;

TCOMMAND send_buff;
TCOMMAND recv_buff;

int MP3_send_buffer(TCOMMAND *command);
void checksum (TCOMMAND *command);
int8_t MP3_play_track(uint16_t track);
int8_t MP3_command(uint8_t command, uint16_t value);
int8_t MP3_play_track_folder(uint8_t track,uint8_t folder);
uint8_t MP3_command_queery(uint8_t command,uint16_t value);
int8_t MP3_queue_FIFO_play(uint8_t track, uint8_t folder);
void MP3_init();


#endif /* DF_PLAYER_H_ */
