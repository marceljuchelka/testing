/*
 * command.h
 *
 *  Created on: 27. 7. 2020
 *      Author: Marcel Juchelka
 */

#ifndef SIM_COMMAND_H_
#define SIM_COMMAND_H_


//AT prikazy
//informacni
#define GSM_reg_net			"AT+CREG?"		//je modul pripojen k siti?
#define GSM_signal			"AT+CSQ"		//sila signalu
#define GSM_init			"AT"			//komunikace s modulem
#define GSM_echo_off		"ATE0"			//vypnuti opakovani AT prikazu

//DTMF
#define GSM_DTMF_on			"AT+DDET=1"		//povolit DTMF
//volani
#define GSM_Vytoc_cislo		"ATD"			//ATD6241xxxx; stredniik je volani
#define GSM_zvedni_hovor	"ATA"			//zvednuti hovoru pri zvoneni
#define GSM_ukonceni_hovoru	"ATH"			//ukonceni pri vyzvaneni nebo polozeni hovoru
#define GSM_micr_gain		"AT+CMIC=0,5"		//zesileni mikrofonu

//SMS
#define GSM_text_mode		"AT+CMGF=1"		//rezim psani SMS
#define GSM_sms_read1		"AT+CMGR=1"		//cteni SMS + cislo
#define GSM_sms_del_all		"AT+CMGD=1,4"		//smazani vsech sms
#define GSM_sms_del_all1	"AT+CMGL=\"ALL\""	//smazani vsech sms
#define GSM_send_sms_num	"AT+CMGS=\""		//posli sms na telefon 420xxxyyyzzz bez +

//hlavicky z SIM800L

#define 		H_NO_C		 	0		//ukonceni hovoru
#define 		H_DTMF			1
#define 		H_CMGR			2
#define 		H_CLIP			3		//info o volani
#define 		H_CMTI			4		//info o prichozi SMS
#define 		H_RING			5

//parsovaci oddelovace
#define 		pars_telnum		"\""	//	",\""	//zjisteni telef. cisla
#define 		pars_dtfm		" "		//dtmf volba
#define 		pars_sms		",\""	//sms volba
#define 		pars_tel_sms	"\",\""	//sms telefon
#define 		pars_tel_init	"\r\n"	//telefon s init

//bezpecnostni stringy
#define 		SMS_init		"init "	//co se musi napsat do inicializacni sms je treba ulozit

//DTMF prikazy pod kterym cislem DTMF je jaky prikaz
#define 	dtmf_machine_OFF		10			//vypne stroj
#define  	dtmf_time_end			01			//mluvi kolik zbyva do konce a jaky proces
#define 	dtmf_sms_on_off			03			//zapnuti sms


#endif /* SIM_COMMAND_H_ */
