/*
 * sof_uart_cfg    atmega32    F_CPU = 11059200 Hz
 *
 *  Created on: 2017-02-02
 *      Autor: Miros�aw Karda�
 *      www.atnel.pl
 */

#ifndef SOF_UART_CFG_
#define SOF_UART_CFG_

#define USE_RX            1
#define USE_TX            1

#define SUART_PORT_RX    PORTC
#define SUART_PIN_RX     PINC
#define SUART_DDR_RX     DDRC

#define SUART_PORT_TX    PORTC
#define SUART_PIN_TX     PINC
#define SUART_DDR_TX     DDRC

#define    RxD           PC6
#define    TxD           PC5


// definicja ilo�ci bit�w STOP
#define sb 1
// stop bits 1..2

// definicja op�nienia dla Software UART
// w zale�no�ci od cz�stotliwo�ci taktowania
// zgodnie z warto�ciami podanymi dla r�nych pr�dko�ci
// w nocie AVR-305
// Table 4-2. Baud Rate Table (str.8)


#define b_value 140

/*
; Some b values:     (See also table in Appnote AVR-305 documentation)
;
; 1 MHz crystal:
;   9600 bps - b=14
;  19200 bps - b=5
;  28800 bps - b=2
;
; 2 MHz crystal:
;  19200 bps - b=14
;  28800 bps - b=8
;  57600 bps - b=2

; 4 MHz crystal:
;  19200 bps - b=31
;  28800 bps - b=19
;  57600 bps - b=8
; 115200 bps - b=2

; 8 MHz crystal:
;    9600 bps - b=135  *******
;  14400 bps - b=89
;  19200 bps - b=66
;  28800 bps - b=42

; 11.0592 MHz crystal:
;    9600 bps - b=188  ******
;  14400 bps - b=124
;  19200 bps - b=92
;  28800 bps - b=60
;  57600 bps - b=28
; 115200 bps - b=12

*/

#endif  /* SOF_UART_CFG_ */
