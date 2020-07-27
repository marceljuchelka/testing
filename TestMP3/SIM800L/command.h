/*
 * command.h
 *
 *  Created on: 27. 7. 2020
 *      Author: Marcel Juchelka
 */

#ifndef COMMAND_H_
#define COMMAND_H_

	//info na seriove lince
	//volani na modul
	//RING
	//
	//+CLIP: "+420608100114",145,"",0,"",0
	//
	//RING
	//
	//+CLIP: "+420608100114",145,"",0,"",0
	//
	//RING
	//
	//+CLIP: "+420608100114",145,"",0,"",0




//AT prikazy
//informacni
#define GSM_signal			AT+CSQ
#define
//DTMF
#define GSM_DTMF_on			AT+DDET=1
//volani
#define GSM_Vytoc_cislo		ATD			//ATD6241xxxx; stredniik je volani
#define GSM_zvedni_hovor	ATA			//yvednuti hovoru pri zvoneni
#define GSM_ukonceni_hovoru	ATH			//ukonceni pri vyzvaneni nebo polozeni hovoru

//SMS
#define GSM_text_mode		AT+CMGF=1	//rezim psani DTMF
#define GSM_text_read		AT+CMGL		//cteni SMS


#endif /* COMMAND_H_ */
