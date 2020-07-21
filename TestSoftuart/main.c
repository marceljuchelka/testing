#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "LCD/lcd44780.h"
#include "SoftUART/soft_uart.h"

char bufor[100];  // bufor roboczy

int main( void ) {

    // inicjalizacja soft uart'a
    suart_init();

    lcd_init();
    lcd_str( "start..." );

    while ( 1 ) {

        // odbieramy string z terminala (funkcja blokuj�ca)
        suart_gets( bufor, sizeof( bufor ) );

        // odsy�amy z powrotem do terminala ca�y odebrany string
        suart_puts( bufor );
        suart_puts( "\r\n" );

        lcd_cls();
        // wy�wietlamy na ekranie LCD odebrany string z terminala
        lcd_str( bufor );

    }
}