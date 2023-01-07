/*
 * adc.c
 *
 * Created: 2022-12-19 09:22:15
 *  Author: willi
 */ 

/********************************************************************************
* adc.c: Innehåller funktionsdefinitioner för implementering av AD-omvandling
*        för analoga signaler via strukten adc.
********************************************************************************/

#include "adc.h"

/********************************************************************************
* adc_read: Läser av en analog insignal och returnerar motsvarande digitala
*           motsvarighet mellan 0 - 1023.
*
*           - self: Pekare till analog pin vars insignal ska AD-omvandlas.
********************************************************************************/
uint16_t adc_read(struct adc_pin* self){
	
	ADMUX = (1 << REFS0) | self->pin;
	ADCSRA = (1 << ADEN) | (1<< ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	while ((ADCSRA & (1 << ADIF)) == 0);
	ADCSRA = (1 << ADIF);
	return ADC;
}

/********************************************************************************
* adc_get_pwm_values: Läser av en analog insignal och beräknar on- och off-tid
*                     för PWM-generering, avrundat till närmaste heltal.
*
*                     - self         : Pekare till analog pin som ska läsas av.
*                     - pwm_period_us: PWM-perioden (on-tid + off-tid) mätt i
*                                      mikrosekunder.
********************************************************************************/
void adc_get_pwm_values(struct adc_pin* self, uint16_t pwm_period_us)
{
	const double duty_cycle = adc_read(self) / 1023.0;
	self->pwm_on_us =(uint16_t) (duty_cycle * pwm_period_us+0.5);
	self->pwm_off_us = pwm_period_us - self->pwm_on_us;
	return;
}

/********************************************************************************
* adc_init: Initierar analog pin för avläsning och AD-omvandling av insignaler,
*			Och nollställer pwm värderna.
*
*           - self: Pekare till analog pin som ska användas för AD-omvandling.
*           - pin : Analog pin som ska läsas för AD-omvandling.
********************************************************************************/
void adc_init(struct adc_pin* self, uint8_t pin)
{
	self->pin = pin;
	self->pwm_off_us = 0;
	self->pwm_on_us = 0;
	(void)adc_read(self);
	return;
}

/********************************************************************************
* adc_delay_on: Aktiverar en fördröjning på perioden av hög signal vid pwm styrning
				enligt tidigare avläst värde. 
				Fördröjningen blir värdet på pwm_on_us i microsekunder
				
				- self         : Pekare till analog pin som har lästs av.
********************************************************************************/
void adc_delay_on(struct adc_pin* self){
	delay_us(self->pwm_on_us);
	return;
}

/********************************************************************************
* adc_delay_off: Aktiverar en fördröjning på perioden av låg signal vid pwm styrning
				enligt tidigare avläst värde. 
				Fördröjningen blir värdet på pwm_off_us i microsekunder
				
				- self         : Pekare till analog pin som har lästs av.
********************************************************************************/
void adc_delay_off(struct adc_pin* self){
	delay_us(self->pwm_off_us);
	return;
}

