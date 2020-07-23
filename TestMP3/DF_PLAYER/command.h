/*
 * command.h
 *
 *  Created on: 22. 7. 2020
 *      Author: Marcel Juchelka
 */

#ifndef COMMAND_H_
#define COMMAND_H_

//prikazy ovladani

#define track_num 				0x03
#define volume_plus				0x04
#define volume_minus			0x05
#define volume_level			0x06		//specifikace hlasitosti
#define specify_EQ				0x07
#define device_source			0x09		//1 usb 2 sdcard
#define amplif_set				0x10		//msb ampl on lsb 0-31
#define reset					0x0C
#define play					0x0D
#define pause					0x0E

//dotazy
#define number_of_track_sd		0x48		//kolik souboru v root



enum {_eq_Normal,_eq_Pop,Rock,_eq_Jazz,_eq_Classic,Base};
enum {play_repeat,play_folder_repeat,play_single_repeat,play_random};
enum {U,TF,AUX,SLEEP,FLASH};


#endif /* COMMAND_H_ */
