/*
 * temp_sensor_reader.c
 *
 * Created: 2023-01-01 18:50:58
 *  Author: willi
 */ 

/* inkluderingsdirektiv */
#include "timer.h"
#include "temp_sensor.h"
#include "misc.h"
#include "serial.h"

/* deklaration av statiska funtuoner. */
static inline double temp_calc_avrg_array_value(double data_array[],uint8_t array_size);
static void temp_get_avrage_temp(double new_temp);
static void temp_get_avrage_time(uint32_t new_avrage_ms);
static inline double temp_read_temprature(void);

/* deklaration av variabeler */
uint32_t mesure_frequensy; /* frenkvens som anv�nds f�r att ange hur ofta temperatur l�ses in och skrivs ut. */
double avrage_temprature; /* snitt temperatur fr�n de 5 senaste m�tningarna.*/
bool button_was_pressed; /* variabel som anv�nds f�r att att aktivera temperaturl�sning vid knappnedtryckning.*/

/********************************************************************************
*
*	temp_init: intierar variabeln mersure_frequensy och button_was_pressed med
*			   startv�rden s� att systemet �r redo att k�ras.
*
********************************************************************************/
void temp_init(void)
{
	mesure_frequensy = 60000;
	button_was_pressed = false;
}

/********************************************************************************
* 
*	serial_print_temp: skriver ut v�rdet f�r temperatur och frekvens till ansluten seriel terminal.
*
********************************************************************************/

void serial_print_temp(void)
{
	serial_print_string("temperature:");
	serial_print_double(avrage_temprature);
	serial_print_string(" C");
	serial_print_new_line();
	serial_print_string("m�tfrekvens:");
	serial_print_integer(mesure_frequensy);
	serial_print_string(" ms");
	serial_print_new_line();
	return;
}

/********************************************************************************
*
*	temp_get_arave_time: tar emot en variabel som anger en vis tid i milesekunder. placerar in variabel 
*						 i en array och r�knar sedan utt medelv�rdet p� de fem senaste v�rdena placerade i arrayen.
*						 
*						 De fem f�rsta v�rdena placeras in i arrayen. N�r arrayen �r full  flytas v�rderna
*						 i arrayen ett steg upp�t och det nya v�rdet placeras sedan in l�ngts ner i arrayen.
*						 
*						 Medelv�rdet f�r tiden placeras sedan i mesure_frequensy. och anv�nds f�r att 
*						 avg�ra hur ofta temperaturen skall m�tas och skrivas utt.
*
*		- new_arage_ms: Det nya v�rdet p� tiden i milesekunder.
*
*		- time_betwen_presses: statisk array d�r de 5 senaste tiderna melan knapttryck i milesekunder laggras.
*
********************************************************************************/
static void temp_get_avrage_time(uint32_t new_avrage_ms)
{
	static double time_betwen_presses[5];
	for (uint8_t i = 0; i < 5 ; i++)
	{
		if (!time_betwen_presses[i])
		{
			time_betwen_presses[i] = new_avrage_ms;
			mesure_frequensy = (uint32_t)(temp_calc_avrg_array_value(time_betwen_presses,i+1)+0.5);
			return;
		}
	}
	for (uint8_t i = 0; i < 4 ; i++) time_betwen_presses[i]=time_betwen_presses[i+1];
	time_betwen_presses[4] = new_avrage_ms;
	mesure_frequensy = (uint32_t)(temp_calc_avrg_array_value(time_betwen_presses,5)+0.5);
	return;
}

/********************************************************************************
*
*	temp_get_arave_temp: tar emot en variabel som anger en temperatur. placerar in variabel i en array
*						 och r�knar sedan utt medelv�rdet p� de fem senaste v�rdena placerade i arrayen.
*
*						 De fem f�rsta v�rdena placeras in i arrayen. N�r arrayen �r full flytas sedan v�rderna
*						 i arrayen ett steg upp�t och det nya v�rdet placeras sedan in l�ngst ner i arrayen.
*
*						 Medelv�rdet f�r tiden placeras sedan i avrage_temprature och �r det v�rdet som skrivs utt
*						 till en seriel terminal.
*						
*
*		- new_temp:	Det nya v�rdet p� temperaturen.
*
*		- avrage_temprature: statisk array d�r de 5 senaste temperaturena lagras.
*
********************************************************************************/
static void temp_get_avrage_temp(double new_temp)
{
	static double avrage_temprature_array[5];
	for (uint8_t i = 0; i < 5 ; i++)
	{
		if (!avrage_temprature_array[i])
		{
			avrage_temprature_array[i] = new_temp;
			avrage_temprature = temp_calc_avrg_array_value(avrage_temprature_array,i+1);
			return;
		}
	}
	for (uint8_t i = 0; i < 4 ; i++) avrage_temprature_array[i]=avrage_temprature_array[i+1];
	avrage_temprature_array[4] = new_temp;
	avrage_temprature = temp_calc_avrg_array_value(avrage_temprature_array,5);
}

/********************************************************************************
*
*	temp_calc_avrg_array_value: tar emot en array pekare och r�knar utt medlev�rdet p�
*						        p� elementen och returnerar v�rdet. antal emlement som 
*							    skall aderas anges av array_size. 
*
*		- data_array[]: array vars medelv�rde skall skall r�knas ut.
*		- array_size: storlek p� arrayen som skikas.
*		- avrg_valu: anv�nds f�r att lagra medelv�rdet som skall returneras.
*
********************************************************************************/
static inline double temp_calc_avrg_array_value(double data_array[],uint8_t array_size)
{
	double avrg_value = 0;
	for (uint8_t i = 0; i < array_size; i++) avrg_value =+ (avrg_value+data_array[i]);
	avrg_value = avrg_value / array_size;
	return  avrg_value;
}

/********************************************************************************
*
*	temp_read_temprature: l�ser av v�rdet p� en analog pin med hj�lp av adc_read 
*						  och konverterar sedan det v�rdet till en temperatur.
*						  konvertering avser ut v�rdet f�r en temperatur sensor 
*						  TMP 36. den utr�cknade teperaturen returneras.
*					
*	- pin_voltage: sparar v�rdet f�r sp�ningen som ligger p� avl�st pin.
*
********************************************************************************/
static inline double temp_read_temprature(void)
{
	double pin_voltage = (adc_read(&pin2) / 1023.0) * 5;
	return 100 * pin_voltage - 50;
}

/********************************************************************************
*
*	ISR (TIMER0_OVF_vect): Tidsbaserat avbrot som sker varje 128e mirco sekund.
*
*						   Vid tv� knapptryckningar i f�ljd inom 60 sekunder s� sparas tiden
*						   melan knapptryckningarna och anv�nds f�r att r�kna utt m�tfrekvensen
*						   f�r m�tning av temperatur.
*
*						   Medeltemperaturen fr�n de 5 senaste knapptryckningarna anv�nds f�r att
*						   ange frekvensen f�r temperaturm�tning och uttskrift.
*
*						   Om Knappen trycks in endast en g�ng och 60 sekunder 
*						   passerar s� �terst�ls systemet och ingen m�t frekvens
*						   l�ses in.
*
*						   F�r att undvika kontaktst�tar s� l�sses inte en knapptryckning in 
*						   100 milesekunder efter att knappen trycks in.
*
*		- button_paused: Variabel som anv�nds f�r att pausa knapptryck 100 ms efter 
*						 att knappen trycks ner. 
*
*		- button_pause_counter:	Variabel som anv�nds f�r att r�kna till antalet avbrot f�r
*								att pausa knappen i 100 ms.
*
*		_ button_has_never_ben_presed: variabel som anger om knappen har tryckts
*									   in dom senaste 60 sekunderna.
*									
********************************************************************************/
ISR (TIMER0_OVF_vect)	
{
    static bool button_paused = false;
	static uint16_t button_pause_counter = 0;
	static bool button_has_never_ben_presed = true;
	
	if (button_pause_counter >= timer_get_max_count(100)) 
	{
		button_paused = false;
		button_pause_counter = 0;
	}
	if (button_paused) button_pause_counter++; 
	
	if (button_is_pressed(&b1) && !button_paused)
	{
		button_paused = true;
		button_was_pressed = true;
		
		if(button_has_never_ben_presed) 
		{
			button_has_never_ben_presed = false;
			timer1_button.counter = 0;
		}
		else 
		{
			temp_get_avrage_time(timer_get_time_elapsed_ms(timer1_button.counter));
			timer1_button.counter = 0;
		}
	}
	else if (timer1_button.counter >= (timer1_button.max_count))
	{
		timer1_button.counter = 0;
		button_has_never_ben_presed = true;
	}
	timer1_button.counter++;
	return;
}


/********************************************************************************
*
*	ISR (TIMER2_0VF_vect): Tidsbaserat avbrot som sker varje 128e mirco sekund.
*						   
*						   Efter initiering s� l�ses l�ses temperaturen in och skriver utt 
*						   medeltemperaturen till Seriel terminal varje 60 sekund.
*
*						   Vid en knapp trycknings s� skrivs medel temperaturen ut. tiden melan 
*						   m�tningar anges av medelv�rdet melan knapptryckningar.
*
*		- mesure counter: R�knare som anv�nds f�r att r�kna upp till 5 temperaturm�tningar
*						  f�r att sedan f� en medeltemperatur.
*
********************************************************************************/
ISR (TIMER2_OVF_vect)
{
	static uint8_t mesure_counter = 5;
	if (button_was_pressed)
	{
		mesure_counter = 0;
		button_was_pressed = false;
	}
	
	if (mesure_counter <= 4 && (uint16_t)(timer_get_max_count(mesure_frequensy)/5+0.5) <= timer2_temp_read.counter)
	{
		mesure_counter++;
		temp_get_avrage_temp(temp_read_temprature());
		timer2_temp_read.counter = 0;
		if (mesure_counter == 4 ) serial_print_temp();
	}
	else if (mesure_counter > 4 && timer_get_max_count(mesure_frequensy) <= timer2_temp_read.counter)
	{
		temp_get_avrage_temp(temp_read_temprature());
		serial_print_temp();
		timer2_temp_read.counter = 0;
	}
	
	else if (timer2_temp_read.counter >= timer2_temp_read.max_count)
	{
		temp_get_avrage_temp(temp_read_temprature());
		serial_print_temp();
		timer2_temp_read.counter = 0;
	}
	timer2_temp_read.counter++;
	return;
}









