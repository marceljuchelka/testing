/*
 * mj_am2320b.h
 *
 *  Created on: 1. 10. 2020
 *      Author: marcel
 */

#ifndef MJ_AM2320B_MJ_AM2320B_H_
#define MJ_AM2320B_MJ_AM2320B_H_

#define ACK 1
#define NACK 0
#define AM2320B_address 0xB8
#define I2C_w 0
#define I2C_r 1
#define temperat 1
#define humidy	2
#define AM2320_ERROR 3

int am2320_getdata(uint8_t select);

#endif /* MJ_AM2320B_MJ_AM2320B_H_ */
