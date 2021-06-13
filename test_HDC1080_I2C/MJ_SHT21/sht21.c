/*                  e-gadget.header
 * sht21.c
 *
 *  Created on: 10.06.2021
 *    Modyfied: 10.06.2021 15:38:32
 *      Author: Marcel Juchelka
 *
 * Project name: "test_HDC1080_I2C"
 *
 *
 *          MCU: ATmega328P
 *        F_CPU: 8 000 000 Hz
 *
 *    Flash: 3 588 bytes   [ 10,9 % ]
 *      RAM:  11 bytes   [ 0,5 % ]
 *   EEPROM:  0 bytes   [ 0,0 % ]
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "../MK_I2C/mk_i2c.h"
#include "../MK_LCD/mk_lcd44780.h"
#include "sht21.h"





