/*
 * soft_uart_init.c
 *
 *  Created on: 2017-02-02
 *      Autor: Miros³aw Kardaœ
 *      www.atnel.pl
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "soft_uart_cfg.h"
#include "soft_uart.h"


void suart_init(void) {

#if USE_RX == 1
    SUART_DDR_RX &= ~(1<<RxD);
    SUART_PORT_RX |= (1<<RxD);    // podci¹ganie do VCC
    _delay_ms(10);
#endif

#if USE_TX == 1
    SUART_DDR_TX |= (1<<TxD);
    SUART_PORT_TX |= (1<<TxD);
#endif

}

#if USE_TX == 1
void suart_puts( char * str ) {
     while( *str ) sputchar( *str++ );
}

void suart_putint( int val ) {
     char buf[7];
     itoa( val, buf, 10 );
     suart_puts( buf );
}
void suart_putc( char val){
	sputchar (val);
}
#endif


#if USE_RX == 1
char * suart_gets( char * buf, uint8_t size ) {

    uint8_t idx=0;
    char znak;

    do {
        znak = sgetchar();

        if( 13 == znak ) break;
        else {
            if( znak > 31 ) buf[ idx++ ] = znak;
        }

        if( idx >= size ) break;

    } while( znak );

    buf[ idx ] = 0;

    return buf;
}

char suart_getc (){
	char znak;
	znak = sgetchar();
	return znak;


}
#endif
