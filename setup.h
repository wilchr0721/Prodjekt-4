/*
 * setup.h
 *
 * Created: 2022-12-19 09:25:15
 *  Author: willi
 */ 

#include "adc.h"
#include "button.h"
#include "timer.h"
/********************************************************************************
*  setup.h:	Innehåller funktionalitet för initiering av det inbyggda systemet.
*			Innehåler eventuella globala variabeldefinitioner eller
*			Definitioner av globala struktar.
*
********************************************************************************/

struct button b1;
struct adc_pin pin2;
struct timer timer1_button,timer2_temp_read;

#ifndef SETUP_H_
#define SETUP_H_

void setup(void);

#endif /* SETUP_H_ */