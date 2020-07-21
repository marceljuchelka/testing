/*
 * soft_uart_sgetchar.S
 *
 *  Created on: 2017-02-02
 *      Autor: Miros³aw Kardaœ
 *      www.atnel.pl
 */
#include <avr/io.h>

#include "soft_uart_cfg.h"

.global sgetchar

sgetchar:
        push    R16
        push    R17
        push    R18

        ldi     R16,8+sb

getchar1:
        sbic     _SFR_IO_ADDR( SUART_PIN_RX ), RxD
        rjmp     getchar1

        rcall UART_delay

getchar2:
        rcall UART_delay
        rcall UART_delay

        clc
        sbic     _SFR_IO_ADDR( SUART_PIN_RX ), RxD
        sec            ;

        dec     R16
        breq     getchar3

        ror     R18
        rjmp     getchar2

getchar3:

        mov        r24, R18
        pop        R18
        pop        R17
        pop        R16
        ret



UART_delay:
        ldi        R17, b_value
UART_delay1:
        dec        R17
        brne    UART_delay1
        ret
