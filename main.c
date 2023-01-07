/*
 * prodjekt_3.c
 *
 * Created: 2022-12-06 12:56:01
 * Author : willi
 */ 
/**********************************************************************

	mian_header.h includerar samtliga headers för alla includerade funtioner.
	
**********************************************************************/
#include "main_header.h"

/**********************************************************************
*
*	*mian: skriver konturnelrigt ut temperaturen från temperatursensorn tmp36
*		   default efter initiering är att temperaturen läses in och skrivs ut
*		   efter 60 sekunder. temperaturen som skrivs ut är medelvärde på de 
*		   5 senaste mätningarna. 
*		   
*		   vid en första knapptryckning så händer inget men vid två knapptryckningar
*		   i följd om  inte 60 sekunder har gått så läses tiden melan knapptryckningarna in
*		   och används som nytt värde för varje gång programet skall läsa in och skriva ut
*		   medeltemperaturen. 
*		   
*		   medelvärdet melan de 5 senaste knapptryckningarna sparas och används som
*		   frekvens för utskrift av medeltemperaturen.
*		   
*		   
**********************************************************************/

int main(void)
{
    setup();
	
    while (1) 
	
    {
		
    }
}

