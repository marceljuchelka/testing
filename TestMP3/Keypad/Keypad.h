/*
 * Keypad4x4.h
 *
 *  Created on: 3. 11. 2020
 *      Author: marcel
 *
 *      ovladani klavesnice 4x4 pres PCF8574
 */
/*	PCF8574 - Base addr 0x40

	A6|A5|A4|A3|A2|A1|A0|RW|ADR
	------------------------
	 0| 1| 0| 0|  |  |  |  |0x40 <-- base addr

	A6|A5|A4|A3|A2|A1|A0|RW|ADR
	------------------------
	 0| 1| 0| 0| 0| 0| 0|  |0x40
	 0| 1| 0| 0| 0| 0| 1|  |0x42
	 0| 1| 0| 0| 0| 1| 0|  |0x44
	 0| 1| 0| 0| 0| 1| 1|  |0x46
	 0| 1| 0| 0| 1| 0| 0|  |0x48
	 0| 1| 0| 0| 1| 0| 1|  |0x4A
	 0| 1| 0| 0| 1| 1| 0|  |0x4C
	 0| 1| 0| 0| 1| 1| 1|  |0x4E
*/



#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "../main.h"

void keypad_init();
int8_t vypocet_tlacitka(uint8_t radek,uint8_t sloupec);
int8_t key_read();
int8_t key_read_str(char *ptr, uint8_t len);
int8_t key_sn_enter();
int8_t key_test_address(uint8_t adresa);

#define Key_PCF8574_addr 0x42	//adresa expanderu klavesnice

#define L1	0b00010000			//1. bit radku klavesnice
extern uint8_t klav_OK;

#endif /* KEYPAD_H_ */
