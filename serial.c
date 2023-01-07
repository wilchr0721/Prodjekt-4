
/********************************************************************************
* serial.c: Inneh�ller drivrutiner f�r seriell �verf�ring med USART.
********************************************************************************/
#include "serial.h"

/********************************************************************************
* serial_init: Initierar seriell transmission, d�r vi skickar en bit i taget
*              med angiven baud rate (bithastighet) m�tt i kilobits per sekund
*              (kbps). Ett vagnreturstecken \r skickas s� att f�rsta utskriften
*              hamnar l�ngst till v�nster.
*
*              1. Vi aktiverar seriell transmission (s�ndning) genom att
*                 ettst�lla biten TXEN0 (Transmitter Enable 0) i kontroll- och
*                 statusregistret UCSR0B (USART Control and Status Register 0 B).
*
*              2. Vi st�ller in att �tta bitar ska skickas i taget (ett tecken
*                 �r �tta bitar) via ettst�llning av bitar UCSZ00 - UCSZ01
*                 (USART Character Size 00 - 01) i kontroll- och status-
*                 registret UCSR0C (USART Control and Status Register 0 C).
*
*              3. Vi st�ller in baud rate (�verf�ringshastighet) genom att
*                 skriva till det 16-bitars registret UBRR0 (USART Baud Rate
*                 Register 0), tilldelat enligt formeln:
*
*                 UBRR0 = F_CPU / (16 * baud_rate_kbps) - 1,
*
*                 d�r F_CPU �r mikrodatorns klockfrekvens och baud_rate_kbps
*                 �r �verf�ringshastigheten i kilobits per sekund. Vi anv�nder
*                 flyttalsdivision genom att s�tta 16 till 16.0. Vi avrundar
*                 till n�rmaste heltal genom att addera 0.5. Vi typomvandlar
*                 resultatet till uint16_t, d� UBRR0 �r 16 bitar bred.
*
*              4. Vi l�gger ett vagnreturstecken 'r' i dataregistret UDR0
*                 (USART Data Register 0, v�rt postfack) s� att f�rsta
*                 utskriften hamnar l�ngst till v�nster.
*
*              5. Vi indikerar att seriell �verf�ring �r aktiverat, s� att
*                 vi inte kan �terinitiera USART av misstag.
********************************************************************************/
void serial_init(const uint16_t baud_rate_kbps)
{
	static bool serial_initialized = false;
	if (serial_initialized) return;

	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
	UBRR0 = (uint16_t)(F_CPU / (16.0 * baud_rate_kbps) - 1 + 0.5);
	UDR0 = '\r';

	serial_initialized = true;
	return;
}

/********************************************************************************
* serial_print_char: Skickar angivet tecken till ansluten seriell terminal.
*
*                    1. Vi v�ntar p� att postfacket UDR0 (USART Data Register 0)
*                       blir tomt, vilket indikeras via biten UDRE0 (USART
*                       Data Register Empty 0) i i kontroll- och status-
*                       registret UCSR0A (USART Control and Status Register 0 A),
*                       som d� blir ettst�lld. S� l�nge UDRE0 �r noll s� v�ntar
*                       vi via en loop.
*
*                   2. S� fort postfacket UDR0 �r tomt s� l�gger vi i det nya
*                      tecknet s� att det skickas sedan.
*
*                    - c: Tecknet som ska skickas.
********************************************************************************/
void serial_print_char(const char c)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
	return;
}

/********************************************************************************
* serial_print_string: Skickar angivet textstycke till ansluten seriell terminal.
*
*                      1. Vi itererar genom str�ngen fr�n b�rjan till slut
*                         (tills vi n�r ett nolltecken) och skickar varje tecken
*                         en efter en via anrop av funktionen serial_print_char.
*
*                      2. Om ett nyradstecken '\n' skickas s� skickar vi ett
*                         vagnreturstecken f�r att n�sta utskrift hamnar l�ngst
*                         till v�nster p� n�sta rad.
*
*                      - s: Textstycket som ska skickas.
********************************************************************************/
void serial_print_string(const char* s)
{
	for (uint16_t i = 0; s[i]; ++i)
	{
		serial_print_char(s[i]);

		if (s[i] == '\n')
		{
			serial_print_char('\r');
		}
	}
	return;
}

/********************************************************************************
* serial_print_integer: Skriver angivet signerat heltal till ansluten seriell
*                       terminal. I praktiken omvandlas heltalet till text
*                       via funktionen sprintf fr�n stdio.h och skickas.
*
*                       1. Deklarerar och initierar en tom str�ng som rymmer
*                          20 tecken. I denna str�ng ska heltalet l�ggas,
*                          fast omvandlat till text.
*
*                       2. Vi omvandlar heltalet till text och lagrar i
*                          str�ngen via anrop av funktionen sprintf. Eftersom
*                          talet �r 32-bitar l�ngt anv�nds formatspecificerare
*                          %ld (long decimal).
*
*                       3. Vi skickar talet som text via str�ngen s.
*
*                       - number: Talet som ska skrivas.
********************************************************************************/
void serial_print_integer(const int32_t number)
{
	char s[20] = { '\0' };
	sprintf(s, "%ld", number);
	serial_print_string(s);
	return;
}

/********************************************************************************
* serial_print_unsigned: Skriver angivet osignerat heltal till ansluten seriell
*                        terminal. I praktiken omvandlas heltalet till text
*                        via funktionen sprintf fr�n stdio.h och skickas.
*
*                        1. Deklarerar och initierar en tom str�ng som rymmer
*                           20 tecken. I denna str�ng ska heltalet l�ggas,
*                           fast omvandlat till text.
*
*                        2. Vi omvandlar heltalet till text och lagrar i
*                           str�ngen via anrop av funktionen sprintf. Eftersom
*                           talet �r 32-bitar l�ngt anv�nds formatspecificerare
*                           %lu (long unsigned).
*
*                        3. Vi skickar talet som text via str�ngen s.
*
*                        - number: Talet som ska skrivas.
********************************************************************************/
void serial_print_unsigned(const uint32_t number)
{
	char s[20] = { '\0' };
	sprintf(s, "%lu", number);
	serial_print_string(s);
	return;
}

/********************************************************************************
* serial_print_double: Skriver angivet flyttal till ansluten seriell terminal.
*
*                      1. Vi deklarerar och initierar en tom str�ng som rymmer
*                         20 tecken.
*
*                      2. Vi erh�ller heltalsdelen av talet via typomvandling
*                         till uint32_t och lagrar i en konstant d�pt integer.
*
*                      3. Om heltalet �r �ver eller lika med noll, s� extraheras
*                         decimaltalet genom att ta flyttalsdelen - heltalsdelen,
*                         exempelvis 3.79 - 3 = 0.79. Vi multiplicerar med 100
*                         f�r att f� tv� decimaler, exempelvis 79, avrundat
*                         till n�rmaste heltal.
*
*                      4. Annars om heltalet �r negativt extraheras decimaldelen
*                         genom att subtrahera heltalsdelen med decimaldelen,
*                         exempelvis -3 - (-3.79) = 0.79. Vi multiplicerar med
*                         100 f�r att f� tv� decimaler, exempelvis 79, avrundat
*                         till n�rmaste heltal.
*
*                      5. Vi s�tter ihop flyttalen i str�ngen, typomvandlat
*                         till text, separerade med en punkt.
*
*                      6. Vi skriver ut flyttalet genom att anropa funktionen
*                         serial_print_string med str�ngen som argument.
*
*                      - number: Talet som ska skrivas.
********************************************************************************/
void serial_print_double(const double number)
{
	char s[20] = { '\0' };
	const int32_t integer = (int32_t)number;
	int32_t decimal;

	if (integer >= 0)
	{
		decimal = (int32_t)((number - integer) * 100 + 0.5);
	}
	else
	{
		decimal = (int32_t)((integer - number) * 100 + 0.5);
	}

	sprintf(s, "%ld.%ld", integer, decimal);
	serial_print_string(s);
	return;
}

