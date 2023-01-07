
/********************************************************************************
* serial.c: Innehåller drivrutiner för seriell överföring med USART.
********************************************************************************/
#include "serial.h"

/********************************************************************************
* serial_init: Initierar seriell transmission, där vi skickar en bit i taget
*              med angiven baud rate (bithastighet) mätt i kilobits per sekund
*              (kbps). Ett vagnreturstecken \r skickas så att första utskriften
*              hamnar längst till vänster.
*
*              1. Vi aktiverar seriell transmission (sändning) genom att
*                 ettställa biten TXEN0 (Transmitter Enable 0) i kontroll- och
*                 statusregistret UCSR0B (USART Control and Status Register 0 B).
*
*              2. Vi ställer in att åtta bitar ska skickas i taget (ett tecken
*                 är åtta bitar) via ettställning av bitar UCSZ00 - UCSZ01
*                 (USART Character Size 00 - 01) i kontroll- och status-
*                 registret UCSR0C (USART Control and Status Register 0 C).
*
*              3. Vi ställer in baud rate (överföringshastighet) genom att
*                 skriva till det 16-bitars registret UBRR0 (USART Baud Rate
*                 Register 0), tilldelat enligt formeln:
*
*                 UBRR0 = F_CPU / (16 * baud_rate_kbps) - 1,
*
*                 där F_CPU är mikrodatorns klockfrekvens och baud_rate_kbps
*                 är överföringshastigheten i kilobits per sekund. Vi använder
*                 flyttalsdivision genom att sätta 16 till 16.0. Vi avrundar
*                 till närmaste heltal genom att addera 0.5. Vi typomvandlar
*                 resultatet till uint16_t, då UBRR0 är 16 bitar bred.
*
*              4. Vi lägger ett vagnreturstecken 'r' i dataregistret UDR0
*                 (USART Data Register 0, vårt postfack) så att första
*                 utskriften hamnar längst till vänster.
*
*              5. Vi indikerar att seriell överföring är aktiverat, så att
*                 vi inte kan återinitiera USART av misstag.
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
*                    1. Vi väntar på att postfacket UDR0 (USART Data Register 0)
*                       blir tomt, vilket indikeras via biten UDRE0 (USART
*                       Data Register Empty 0) i i kontroll- och status-
*                       registret UCSR0A (USART Control and Status Register 0 A),
*                       som då blir ettställd. Så länge UDRE0 är noll så väntar
*                       vi via en loop.
*
*                   2. Så fort postfacket UDR0 är tomt så lägger vi i det nya
*                      tecknet så att det skickas sedan.
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
*                      1. Vi itererar genom strängen från början till slut
*                         (tills vi når ett nolltecken) och skickar varje tecken
*                         en efter en via anrop av funktionen serial_print_char.
*
*                      2. Om ett nyradstecken '\n' skickas så skickar vi ett
*                         vagnreturstecken för att nästa utskrift hamnar längst
*                         till vänster på nästa rad.
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
*                       via funktionen sprintf från stdio.h och skickas.
*
*                       1. Deklarerar och initierar en tom sträng som rymmer
*                          20 tecken. I denna sträng ska heltalet läggas,
*                          fast omvandlat till text.
*
*                       2. Vi omvandlar heltalet till text och lagrar i
*                          strängen via anrop av funktionen sprintf. Eftersom
*                          talet är 32-bitar långt används formatspecificerare
*                          %ld (long decimal).
*
*                       3. Vi skickar talet som text via strängen s.
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
*                        via funktionen sprintf från stdio.h och skickas.
*
*                        1. Deklarerar och initierar en tom sträng som rymmer
*                           20 tecken. I denna sträng ska heltalet läggas,
*                           fast omvandlat till text.
*
*                        2. Vi omvandlar heltalet till text och lagrar i
*                           strängen via anrop av funktionen sprintf. Eftersom
*                           talet är 32-bitar långt används formatspecificerare
*                           %lu (long unsigned).
*
*                        3. Vi skickar talet som text via strängen s.
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
*                      1. Vi deklarerar och initierar en tom sträng som rymmer
*                         20 tecken.
*
*                      2. Vi erhåller heltalsdelen av talet via typomvandling
*                         till uint32_t och lagrar i en konstant döpt integer.
*
*                      3. Om heltalet är över eller lika med noll, så extraheras
*                         decimaltalet genom att ta flyttalsdelen - heltalsdelen,
*                         exempelvis 3.79 - 3 = 0.79. Vi multiplicerar med 100
*                         för att få två decimaler, exempelvis 79, avrundat
*                         till närmaste heltal.
*
*                      4. Annars om heltalet är negativt extraheras decimaldelen
*                         genom att subtrahera heltalsdelen med decimaldelen,
*                         exempelvis -3 - (-3.79) = 0.79. Vi multiplicerar med
*                         100 för att få två decimaler, exempelvis 79, avrundat
*                         till närmaste heltal.
*
*                      5. Vi sätter ihop flyttalen i strängen, typomvandlat
*                         till text, separerade med en punkt.
*
*                      6. Vi skriver ut flyttalet genom att anropa funktionen
*                         serial_print_string med strängen som argument.
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

