/*
 * led_vect.c
 *
 * Created: 2022-12-06 18:31:29
 *  Author: willi
 */ 

/********************************************************************************
* led_vect.c: Innehåller funktionsdefinitioner för implementering av dynamiska
*               vektorer för lagring och styrning över multipla lysdioder eller
*               andra utportar via strukten led_vect.
********************************************************************************/

#include "led_vect.h"

/********************************************************************************
* led_vect_init: Initierar angiven vektor till tom vid start.
*
*                - self: Pekare till vektorn som ska initieras.
********************************************************************************/
void led_vect_init(struct led_vect* self){
	
	self->leds = 0;
	self->size = 0;
	return;
}

/********************************************************************************
* led_vect_push: Lägger till ett nytt led-objekt längst bak i angiven vektor.
*                Vid misslyckad minnesallokering returneras felkod 1.
*                Annars om push-operationen lyckas returneras 0.
*
*                - self*   : Pekare till vektorn som ska tilldelas.
*                - led_ptr*: Det nya led-objekt som ska läggas till.
********************************************************************************/
void led_vect_push(struct led_vect* self, struct led* new_led){
	
	struct led* copy = (struct led*)realloc(self->leds,sizeof(struct led*)*(self->size + 1));
	if(!copy) return;
	copy[self->size++]= *new_led;
	self->leds= copy;
	return;
}

/********************************************************************************
* led_vect_pop: Tar bort eventuellt sista led-objekt i angiven vektor genom
*               att minska dess storlek med ett. Vid misslyckad omallokering
*               returneras felkod 1, annars 0.
*
*               - self: Pekare till vektorn vars sista element ska tas bort.
********************************************************************************/
void led_vect_pop(struct led_vect* self){
	if(self->size <= 1){
		led_vect_clear(self);
		return;
	}
	struct led* copy = (struct led*)realloc(self->leds,sizeof(struct led*)*(self->size - 1));
	if(!copy) return;
	self->leds= copy;
	return;
}

/********************************************************************************
* led_vect_clear:Tömmer och nollställer angiven vektor.
*
*                - self: Pekare till vektorn som ska tömmas.
********************************************************************************/
void led_vect_clear(struct led_vect* self){
	free(self->leds);
	self->size = 0;
	self->leds = 0;
}

/********************************************************************************
* led_vect_on: Tänder samtliga lysdioder lagrade i angiven vektor.
*
*              - self: Pekare till vektorn vars lysdioder ska tändas.
********************************************************************************/
void led_vect_on(struct led_vect* self){
	
	for(int i = 0 ; i < self->size ; i++){
		led_on(&self->leds[i]);	
	}
	return;
}


/********************************************************************************
* led_vect_off: släcker samtliga lysdioder lagrade i angiven vektor.
*
*              - self: Pekare till vektorn vars lysdioder ska släckas.
********************************************************************************/
void led_vect_off(struct led_vect* self){
	
	for(uint8_t i = 0 ; i < self->size ; i++){
		led_off(&self->leds[i]);
	}
	return;
}

/********************************************************************************
* led_vect_blink_sequentially: Genomför sekventiell blinkning av samtliga
*                              lysdioder lagrade i angiven vektor. Därmed
*                              blinkar lysdioderna i en sekvens en efter en.
*
*                                - self      : Pekare till vektorn vars
*                                              lysdioder ska blinkas.
*                                - time_delay: Lysdiodernas blinkhastighet
*                                              mätt i millisekunder.
********************************************************************************/
void led_vect_blink_sequentally(struct led_vect* self, uint16_t time_delay){
	
	for (uint8_t i = 0; i < self->size; i++){
		led_on(&self->leds[i]);
		delay_ms(time_delay);
		led_off(&self->leds[i]);
	}
	return;
}

/********************************************************************************
* led_vect_blink_collectively: Genomför kollektiv (synkroniserad) blinkning
*                              av samtliga lysdioder lagrade i angiven vektor.
*
*                              - self          : Pekare till vektorn vars
*                                                lysdioder ska blinkas.
*                              - time_delay: Lysdiodernas blinkhastighet
*                                            mätt i millisekunder.
********************************************************************************/
void led_vect_toggle(struct led_vect* self){
	
	for (uint8_t i = 0; i < self->size; i++){
		led_toggle(&self->leds[i]);
	}
	return;
}

/********************************************************************************
* led_vect_toggle: Togglar samtliga lysdioder lagrade i angiven vektor.
*
*                  - self: Pekare till vektorn vars lysdioder ska togglas.
********************************************************************************/
void led_vect_blink_collectively(struct led_vect* self, uint16_t time_delay){
	
	led_vect_toggle(self);
	delay_ms(time_delay);
	return;
}

