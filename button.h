/*
 * button.h
 *
 * Created: 2022-12-09 11:54:08
 *  Author: willi
 */ 

/********************************************************************************
* button.h: Innehåller funktionalitet för enkel implementering av tryckknappar
*           via strukten button samt associerade funktioner. Denna strukt
*           fungerar också utmärkt för andra digitala inportar där insignalen
*           ska kunna läsas av samt avbrott ska kunna genereras vid ett
*           godtyckligt event.
********************************************************************************/

#ifndef BUTTON_H_
#define BUTTON_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* button: Strukt för implementering av tryckknappar och andra digitala inportar.
*    
********************************************************************************/

struct button
{
	uint8_t pin;					/* Tryckknappens pin-nummer på aktuell I/O-port. */
	enum io_port io_port_button;	/* I/O-port som lysdioden är ansluten till. */
};


/********************************************************************************
* button_init: Initierar ny tryckknapp på angiven pin.
*
*              - self: Pekare till tryckknappen som ska initieras.
*              - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 13.
*                      Alternativt kan motsvarande port-nummer på ATmega328P
*                      anges, exempelvis B5 för pin 13 eller D3 för pin 3.
********************************************************************************/
void button_init(struct button* self, uint8_t pin);

/********************************************************************************
* button_is_pressed: Läser av tryckknappens pin och indikerar ifall denna är
*                    nedtryckt. I så fall returneras true, annars false.
*
*                    - self: Pekare till tryckknappen som ska läsas av.
********************************************************************************/
bool button_is_pressed(struct button* self);

/********************************************************************************
* button_clear: Nollställer tryckknapp samt motsvarande pin.
*
*               - self: Pekare till tryckknappen som ska nollställas.
********************************************************************************/
void button_clear(struct button* self);

/********************************************************************************
* button_aktivate_interrupt: Aktiverar PCI-avbrott på angiven tryckknapp så att
*							event på tryckknappens pin medför avbrott, både på
*							stigande och fallande flank. Kontroll av flank måste
*							skötas av användaren, förslagsvis via avläsning i
*							motsvarande avbrottsrutin.
*
*							Nedan visas sambandet mellan använd I/O-port samt
*							avbrottsvektorn för motsvarande avbrottsrutin:
*
*							I/O-port     pin (Arduino Uno)     Avbrottsvektor
*                             B              8 - 13             PCINT0_vect
*                             C             A0 - A5             PCINT1_vect
*                             D              0 - 7              PCINT2_vect
*
*                          - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska aktiveras på.
********************************************************************************/
void button_aktivate_interupts(struct button* self);


#endif /* BUTTON_H_ */