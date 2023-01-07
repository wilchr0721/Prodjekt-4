/*
 * adc.h
 *
 * Created: 2022-12-19 09:21:47
 *  Author: willi
 */ 

/********************************************************************************
* adc.h: Innehåller funktionalitet för enkel implementering av AD-omvandling
*        för analoga signaler via strukten adc. AD-omvandling sker via inläsning
*        av signaler från analoga pinnar A0 - A5 på Arduino Uno, vilket
*        motsvarar PORTC0 - PORTC5 på ATmega328P.
*
*		 AD-omvanlingen kan till exemel användas för att läsa av en.
*		 Potensiometer som kan variera spänningen från 0 - 5 V som blir 
*		 en digital signal från 0 - 1023. 
*		
*		 Signalen kan till exemepl användas för att sjukstera ljusstyrka med
*        en duty_cycle som beräknas med: 
*
*			duty_cycle = ADC_Value/Max_Value
*		
********************************************************************************/

#ifndef ADC_H_
#define ADC_H_

/* Inkluderingsderektiv:*/
#include "misc.h"
#include "setup.h"

#define PO1 0 /* Potensiometer kopplad till ingång A0 på aruduinot.*/
#define PO2 1 /* Potensiometer kopplad till ingång A1 på aruduinot.*/

/********************************************************************************
* adc: Strukt för implementering av AD-omvandlare, som möjliggör avläsning
*      av insignaler från analoga pinnar samt beräkning av on- och off-tid för
*      PWM-generering 
********************************************************************************/
struct adc_pin{
	uint8_t pin;			/* pin: anger vilken pin på arduinot som läses av.*/
	uint16_t pwm_on_us;		/* pwm_on_us: Anger hur länge en signal skall vara hög vid PWM styrning.*/
	uint16_t pwm_off_us;	/* pwm_off_us: Anger hur länge en signal skall vara låg vid PWM styrning.*/
};

/********************************************************************************
* adc_init: Initierar analog pin för avläsning och AD-omvandling av insignaler,
*			Och nollställer pwm värderna.
*
*           - self: Pekare till analog pin som ska användas för AD-omvandling.
*           - pin : Analog pin som ska läsas för AD-omvandling.
********************************************************************************/
void adc_init(struct adc_pin* self, uint8_t pin);

/********************************************************************************
* adc_read: Läser av en analog insignal och returnerar motsvarande digitala
*           motsvarighet mellan 0 - 1023.
*
*           - self: Pekare till analog pin vars insignal ska AD-omvandlas.
********************************************************************************/
uint16_t adc_read(struct adc_pin* self);

/********************************************************************************
* adc_get_pwm_values: Läser av en analog insignal och beräknar on- och off-tid
*                     för PWM-generering, avrundat till närmaste heltal.
*
*                     - self         : Pekare till analog pin som ska läsas av.
*                     - pwm_period_us: PWM-perioden (on-tid + off-tid) mätt i
*                                      mikrosekunder.
********************************************************************************/
void adc_get_pwm_values(struct adc_pin* self, uint16_t pwm_period_us);

/********************************************************************************
* adc_delay_on: Aktiverar en fördröjning på perioden av hög signal vid pwm styrning
				enligt tidigare avläst värde. 
				Fördröjningen blir värdet på pwm_on_us i microsekunder
				
				- self         : Pekare till analog pin som har lästs av.
********************************************************************************/
void adc_delay_on(struct adc_pin* self);

/********************************************************************************
* adc_delay_off: Aktiverar en fördröjning på perioden av låg signal vid pwm styrning
				enligt tidigare avläst värde. 
				Fördröjningen blir värdet på pwm_off_us i microsekunder
				
				- self         : Pekare till analog pin som har lästs av.
********************************************************************************/
void adc_delay_off(struct adc_pin* self);

#endif /* ADC_H_ */