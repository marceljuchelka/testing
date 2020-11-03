/*
 * Keypad.c
 *
 *  Created on: 3. 11. 2020
 *      Author: marcel
 */

#include <avr/io.h>
#include "../main.h"
#include "../main.h"
#include "../Keypad/Keypad.h"

void keypad_init(){
	uint8_t vysledek = 0;
	lcd_str_al(0,0,"start I2C",_left);


	lcd_hex_al(1,0,PCF8574_addr,_left);
			lcd_hex_al(1,4,vysledek,_left);
			_delay_ms(1000);

}






