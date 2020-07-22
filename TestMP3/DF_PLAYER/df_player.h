/*
 * df_player.h
 *
 *  Created on: 22. 7. 2020
 *      Author: Marcel Juchelka
 */

#ifndef DF_PLAYER_H_
#define DF_PLAYER_H_

typedef union{
	uint8_t bytes[9];
	struct {
		uint8_t SS;
		uint8_t VER;
		uint8_t Len;
		uint8_t CMD;
		uint8_t Feedback;
		uint8_t para_H;
		uint8_t para_L;
		uint8_t checksum;
		uint8_t End_bit;
	};
} TCOMMAND;

int send_command(TCOMMAND *command);

#endif /* DF_PLAYER_H_ */
