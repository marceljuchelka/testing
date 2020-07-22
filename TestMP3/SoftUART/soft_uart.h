/*
 * soft_uart.h
 *
 *  Created on: 2017-02-02
 *      Autor: Miros³aw Kardaœ
 *      www.atnel.pl
 */

#ifndef SOFT_UART_H_
#define SOFT_UART_H_




void sputchar( char zn );
char sgetchar( void );
void suart_init( void );

void suart_puts( char * str );
void suart_putint( int val );

char * suart_gets( char * buf, uint8_t size );



#endif /* SOFT_UART_H_ */
