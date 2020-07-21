/*
 * soft_uart_sputchar.S
 *
 *  Created on: 2017-02-02
 *      Autor: Miros³aw Kardaœ
 *      www.atnel.pl
 */
#include <avr/io.h>

#include "soft_uart_cfg.h"

.global sputchar

sputchar:

        push    R16
        push    R17
        push    R18

        mov        R18, R24

        ldi    r16,1+8+sb
        com    r18
        sec

putchar0:
        brcc    putchar1    ;If carry set
        cbi        _SFR_IO_ADDR( SUART_PORT_TX ), TxD
        rjmp    putchar2    ;else

putchar1:
        sbi    _SFR_IO_ADDR( SUART_PORT_TX ), TxD
        nop

putchar2:
        rcall UART_delay
        rcall UART_delay

        lsr        r18
        dec        r16
        brne    putchar0


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
