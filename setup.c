/*
 * setup.c
 *
 * Created: 2022-12-19 09:24:57
 *  Author: willi
 */ 

/********************************************************************************
* setup.c: Innehåller funktionalitet för initiering av det inbyggda systemet.
********************************************************************************/

#include "adc.h"
#include "button.h"
#include "timer.h"
#include "serial.h"
#include "temp_sensor.h"

/********************************************************************************
* setup: initeierar det inbyggda systemet
*
********************************************************************************/



void setup(void)
{
	button_init(&b1,13);
	adc_init(&pin2,2);
	
	timer_init(&timer1_button,TIMER_NR_0,60000);
	timer_init(&timer2_temp_read,TIMER_NR_2,60000);
	
	timer_enable_interrupt(&timer1_button);
	timer_enable_interrupt(&timer2_temp_read);
	
	serial_init(9600);
	
	temp_init();
	

	return;
}
