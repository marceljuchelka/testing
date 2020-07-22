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
#include "../SoftUART/soft_uart.h"

TCOMMAND command_all = {{0x7E, 0xFF, 06, 00, 01, 00, 00, 00, 00, 0xEF}};



int send_command(TCOMMAND *command){
	for (uint8_t i = 0; i<10; i++){
		suart_putint(command->bytes[i]);
	}


	return 1;

}
