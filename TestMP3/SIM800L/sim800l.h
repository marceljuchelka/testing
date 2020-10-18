/*
 * sim800l.h
 *
 *  Created on: 27. 7. 2020
 *      Author: Marcel Juchelka
 */

#ifndef SIM800L_H_
#define SIM800L_H_


int8_t sim800l_init();
int8_t sim800l_read();
int8_t sim800l_select_command(char *rx_string,uint8_t hlavicka);
int8_t sim800l_read_uart(char *buf);
int8_t sim800l_msg_head(char *message);
void sim800l_tel_num_write(char *telnum);
int8_t sim800l_ringign(char *rx_string);
int8_t sim800l_dtmf_select (char *rx_string);
int8_t sim800l_sms(char *rx_string);
int8_t sim800l_sms_info(char *rx_string);
int8_t sim800l_at_com_send(char *command, uint8_t ansver);
int8_t sim800l_dtmf_command(uint8_t dtmf_val);
int8_t sim800l_sms_send(char* tel_num, char *text);
int8_t sim800l_check();
int8_t sim800l_net_registrace();
int8_t sim800l_signal_qality();
int8_t sim800l_read_uart_answer(char *buf,uint8_t len);
void sim800l_signal_ikon(const uint8_t *znak, uint8_t y,uint8_t x);

#define DIR_conv (1<<PC1);		//pin dir na prevodnik5 vs 3.3V
extern PROGMEM const uint8_t signal[][8];
extern volatile char stav_sim800l;
//stavy na modulu
#define volani 1


//makra

#define stav_volani (1<<volani)




#endif /* SIM800L_H_ */
