/********************************************************************************
* serial.h: Innehåller drivrutiner för seriell överföring med USART.
*           Vi skickar ett tecken i taget asynkront med en överföringhastighet
*           på 9600 kbps eller dylikt.
********************************************************************************/
#ifndef SERIAL_H_
#define SERIAL_H_

#include "misc.h"
#include <stdio.h>

/********************************************************************************
* serial_init: Initierar seriell transmission, där vi skickar en bit i taget
*              med angiven baud rate (bithastighet) mätt i kilobits per sekund
*              (kbps). Ett vagnreturstecken \r skickas så att första utskriften
*              hamnar längst till vänster.
********************************************************************************/
void serial_init(const uint16_t baud_rate_kbps);

/********************************************************************************
* serial_print_char: Skickar angivet tecken till ansluten seriell terminal.
*
*                    - c: Tecknet som ska skickas.
********************************************************************************/
void serial_print_char(const char c);

/********************************************************************************
* serial_print_string: Skickar angivet textstycke till ansluten seriell terminal.
*
*                      - s: Textstycket som ska skickas.
********************************************************************************/
void serial_print_string(const char* s);

/********************************************************************************
* serial_print_integer: Skriver angivet signerat heltal till ansluten seriell
*                       terminal.
*
*                       - number: Talet som ska skrivas.
********************************************************************************/
void serial_print_integer(const int32_t number);

/********************************************************************************
* serial_print_unsigned: Skriver angivet osignerat heltal till ansluten seriell
*                        terminal.
*
*                        - number: Talet som ska skrivas.
********************************************************************************/
void serial_print_unsigned(const uint32_t number);

/********************************************************************************
* serial_print_double: Skriver angivet flyttal till ansluten seriell terminal.
*
*                      - number: Talet som ska skrivas.
********************************************************************************/
void serial_print_double(const double number);

/********************************************************************************
* serial_print_new_line: Ser till att nästa utskrift hamnar på nästa rad.
********************************************************************************/
static inline void serial_print_new_line(void)
{
	serial_print_string("\n");
	return;
}

#endif /* SERIAL_H_ */


