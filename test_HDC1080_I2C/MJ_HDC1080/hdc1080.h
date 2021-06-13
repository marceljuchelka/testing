/*                  e-gadget.header
 * hdc1080.h
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

#ifndef MJ_HDC1080_HDC1080_H_
#define MJ_HDC1080_HDC1080_H_

#define hdc_1080_address 0b10000000

typedef union{
	uint16_t BYTES2;
	struct{
		uint8_t LSB;
		uint8_t	MSB;
	};
}SWAP16;

enum {
	_i2c_write,
	_i2c_read,

};

/* mapa registru obsahuje datové záznamy, které obsahují informace o konfiguraci,
 * výsledky mìøení teploty a vlhkosti a stavové informace. */
enum register_map{
	Temperature = 	0x00,
	Humidity = 		0x01,
	Configuration =	0x02,
	SerialID1 = 	0xFB,
	SerialID2 = 	0xFC,
	SerialID3 =		0xFD,
	ManufacturerID=	0xFE,
	DeviceID = 		0xFF,
};

/* nasaveni konfiguracniho registru v bitech
 * Tento registr konfiguruje funkènost zaøízení a vrací stav. */
enum {
	HRES = 	8,				//0x0100,
	TRES = 	10,				//0x0400,
	BTST = 	11,				//0x0800,
	MODE = 	12,				//0x1000,
	HEAT =	13,				//0x2000,
	RST = 	15,				//0x8000,
};

enum temp_resolution{
	TempMR_11_bit = 0,
	TempMR_14_bit = 1,
};

enum hum_resolution {
	HumMR_14_bit = 0b00,
	HumMR_11_bit = 0b01,
	HumMR_8_bit = 0b10,
};


uint16_t swap_uint16(uint16_t swap_num);
void hcd1080_init();
uint16_t hdc1080_read_register(uint8_t set_register);
float hcd1080_read_hum();
float hcd1080_read_temp();

#endif /* MJ_HDC1080_HDC1080_H_ */

