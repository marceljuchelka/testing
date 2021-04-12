/*
 * main.h
 *
 *  Created on: 26. 7. 2020
 *      Author: marcel
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "MK_I2C/mk_i2c.h"
#include "MK_LCD/mk_lcd44780.h"
#include "SoftUART/soft_uart.h"
#include "DF_PLAYER/df_player.h"
#include "DF_PLAYER/command.h"
#include "SIM800L/sim_command.h"
#include "UART/uart.h"
#include "SIM800L/sim800l.h"
#include "MJ_AM2320B/mj_am2320b.h"
#include "Keypad/Keypad.h"


void lcdprint_buffer();
void MP3_test();
void lcd_show_ADS(uint16_t hodnota,uint8_t pozice);
uint8_t i2c_scan();
int8_t i2c_test_address(uint8_t adresa);
extern volatile uint8_t sekundy, proces;
extern volatile uint8_t pocitadlo_sekundy;



#endif /* MAIN_H_ */
