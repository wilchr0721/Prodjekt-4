/*
 * prodjekt_3.c
 *
 * Created: 2022-12-06 12:56:01
 * Author : willi
 */ 
/**********************************************************************

	mian_header.h includerar samtliga headers f�r alla includerade funtioner.
	
**********************************************************************/
#include "main_header.h"

/**********************************************************************
*
*	*mian: skriver konturnelrigt ut temperaturen fr�n temperatursensorn tmp36
*		   default efter initiering �r att temperaturen l�ses in och skrivs ut
*		   efter 60 sekunder. temperaturen som skrivs ut �r medelv�rde p� de 
*		   5 senaste m�tningarna. 
*		   
*		   vid en f�rsta knapptryckning s� h�nder inget men vid tv� knapptryckningar
*		   i f�ljd om  inte 60 sekunder har g�tt s� l�ses tiden melan knapptryckningarna in
*		   och anv�nds som nytt v�rde f�r varje g�ng programet skall l�sa in och skriva ut
*		   medeltemperaturen. 
*		   
*		   medelv�rdet melan de 5 senaste knapptryckningarna sparas och anv�nds som
*		   frekvens f�r utskrift av medeltemperaturen.
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

