/*
 * sim800l.h
 *
 *  Created on: 27. 7. 2020
 *      Author: Marcel Juchelka
 */

#ifndef SIM800L_H_
#define SIM800L_H_


void sim800l_init();
int8_t sim800l_read();
int8_t sim800l_select_command(char *rx_string,uint8_t hlavicka);
int8_t sim800l_read_uart(char *buf);
int8_t sim800_msg_head(char *message);
void sim800l_tel_num_write(char *telnum);
int8_t sim800l_ringign(char *rx_string);
int8_t sim800l_dtmf_select (char *rx_string);
int8_t sim800l_sms(char *rx_string);
int8_t sim800l_sms_info(char *rx_string);








#endif /* SIM800L_H_ */
