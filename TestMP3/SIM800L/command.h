/*
 * command.h
 *
 *  Created on: 27. 7. 2020
 *      Author: Marcel Juchelka
 */

#ifndef COMMAND_H_
#define COMMAND_H_


//AT prikazy
//informacni
#define GSM_signal			"AT+CSQ \r\n"

//DTMF
#define GSM_DTMF_on			"AT+DDET=1 \r\n"
//volani
#define GSM_Vytoc_cislo		"ATD \r\n"			//ATD6241xxxx; stredniik je volani
#define GSM_zvedni_hovor	"ATA \r\n"			//zvednuti hovoru pri zvoneni
#define GSM_ukonceni_hovoru	"ATH \r\n"			//ukonceni pri vyzvaneni nebo polozeni hovoru

//SMS
#define GSM_text_mode		"AT+CMGF=1 \r\n"		//rezim psani DTMF
#define GSM_sms_read		"AT+CMGR=1 \r\n"		//cteni SMS + cislo
#define GSM_sms_del_all		"AT+CMGD=1,4 \r\n"		//smazani vsech sms
#define GSN_sms_del_all1	"AT+CMGL=\"ALL\" \r\n"	//smazani vsech sms

//hlavicky z SIM800L

#define 		H_NO CARRIER 	0
#define 		H_DTMF			1
#define 		H_CMGR			2
#define 		H_RING			3		//info o volani
#define 		H_CMTI			4		//info o prichozi SMS
#define 		H_CLIP			5

//parsovaci oddelovace
#define 		pars_telnum		"\""	//	",\""	//zjisteni telef. cisla
#define 		pars_dtfm		" "		//dtmf volba
#define 		pars_sms		",\""	//sms volba

#endif /* COMMAND_H_ */
