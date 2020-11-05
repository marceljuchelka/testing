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

#define Key_PCF8574_addr 0x40

#define C1	0b00000001
//#define C2	0b01000000
//#define C3	0b00100000
//#define C4	0b00010000



#endif /* KEYPAD_H_ */
