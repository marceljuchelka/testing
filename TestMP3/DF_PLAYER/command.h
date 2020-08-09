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
#define play_folder				0x0F

//hierarchie SD card
#define folder_hodiny					02
#define folder_minuty					03
#define folder_info						04
#define folder_menu						05


//informacni samply folder info
#define sampl_cisteni_ukonceno			0x01
#define sampl_cleaner_je_vypnut			0x02
#define sampl_do_konce_ukonceni_zbyva	0x03
#define sampl_ozone_cleaner_pro			0x04
#define sampl_vyberte_dotaz				0x05
#define sampl_dokonce_cisteni			0x06
#define sampl_cisteni_skonci_za			0x07
#define sampl_priprava_skoci_za			0x08
#define sampl_ukonceni_skonci_za		0x09
#define sampl_info_sms_on_off			10

//menu samply
#define menu_cleaner_off				10
#define menu_proces_minut				1
#define menu_SMS_info_on				2



//dotazy
#define inicializace				0x3F		//inicializace karty
#define volume_query				0x43		//Query current volume
#define number_of_track_sd			0x48		//kolik souboru v SD



enum {_eq_Normal,_eq_Pop,Rock,_eq_Jazz,_eq_Classic,Base};
enum {play_repeat,play_folder_repeat,play_single_repeat,play_random};
enum {U,TF,AUX,SLEEP,FLASH};




#endif /* COMMAND_H_ */
