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
	uint8_t vertikal = C1, horizontal = 0, i;
	for (i=0;i<4;i++){
		lcd_bin_al(0,0,~vertikal,8,_left);
		pcf8574_write(Key_PCF8574_addr,~vertikal);
		_delay_ms(100);
		horizontal = ~(pcf8574_read(Key_PCF8574_addr)&0xF0);
		if(horizontal >0){
			lcd_bin_al(1,0,horizontal,8,_left);
//			_delay_ms(1000);
		}
		vertikal = vertikal<<1;
	}

}






