/*
 * led.c
 *
 * Created: 2022-12-06 18:28:23
 *  Author: willi
 */ 

/********************************************************************************
* led.c: Innehåller funtioner för tändning och släckning av leds.
********************************************************************************/
#include "led.h"

/********************************************************************************
* led_init: initierar en led strukt för andvädning på port B eller D
			 och sätter porten som en utgång. 
*
*           - self: led pekare för en led koplad till arduinon.
*           - pin: anger vilken pin som skall tilldelas led strukten.
********************************************************************************/
void led_init(struct led* self,const uint8_t pin){
	
	if (pin >= 0 && pin <= 7){

		self->io_port_led = IO_PORTD;
		self->pin = pin;
		DDRD |= ( 1 << pin);
		self->enabled = false;
	}
	
	else if(pin >= 8 && pin <= 13 ){
		self->pin = pin-8;
		self->io_port_led = IO_PORTB;
		DDRB |= ( 1 << (pin-8));
		self->enabled = false;
	}
	return;
}

/********************************************************************************
* led_on: tänder ledden på uttgången som skikas.
*
*           - struct led* self: led strukt pekare för en led koplad till arduinon.
********************************************************************************/
void led_on(struct led* self){
	if (self->io_port_led == IO_PORTB){
		PORTB |= ( 1 << (self->pin));
		self->enabled=true;
	}
	else if (self->io_port_led == IO_PORTD){
		PORTD  |= ( 1 << (self->pin));
		self->enabled=true;
	}
	return;
}

/********************************************************************************
* led_toggle: tänder eller släcker en led beroende på des tidiggare tillstånd.
*
*           - struct led* self: led strukt pekare för en led koplad till arduinon.
********************************************************************************/
void led_toggle(struct led* self){
	if (self->enabled == true)
	{
		led_off(self);
		self->enabled = false;
		return;
	}
	else if(self->enabled == false)
	{
		led_on(self);
		self->enabled = true;
		return;
	}
	return;
}

/********************************************************************************
* led_off: släcker ledden på uttgången som skikas.
*
*           - struct led* self: led strukt pekare för en led koplad till arduinon.
********************************************************************************/
void led_off(struct led* self){
	if (self->io_port_led == IO_PORTB){
		PORTB &= ~( 1 << (self->pin));
		self->enabled = false;
	}
	else if (self->io_port_led == IO_PORTD){
		PORTD  &= ~( 1 << (self->pin));
		self->enabled = false;
	}
}