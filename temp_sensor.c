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
uint32_t mesure_frequensy; /* frenkvens som används för att ange hur ofta temperatur läses in och skrivs ut. */
double avrage_temprature; /* snitt temperatur från de 5 senaste mätningarna.*/
bool button_was_pressed; /* variabel som används för att att aktivera temperaturläsning vid knappnedtryckning.*/

/********************************************************************************
*
*	temp_init: intierar variabeln mersure_frequensy och button_was_pressed med
*			   startvärden så att systemet är redo att köras.
*
********************************************************************************/
void temp_init(void)
{
	mesure_frequensy = 60000;
	button_was_pressed = false;
}

/********************************************************************************
* 
*	serial_print_temp: skriver ut värdet för temperatur och frekvens till ansluten seriel terminal.
*
********************************************************************************/

void serial_print_temp(void)
{
	serial_print_string("temperature:");
	serial_print_double(avrage_temprature);
	serial_print_string(" C");
	serial_print_new_line();
	serial_print_string("mätfrekvens:");
	serial_print_integer(mesure_frequensy);
	serial_print_string(" ms");
	serial_print_new_line();
	return;
}

/********************************************************************************
*
*	temp_get_arave_time: tar emot en variabel som anger en vis tid i milesekunder. placerar in variabel 
*						 i en array och räknar sedan utt medelvärdet på de fem senaste värdena placerade i arrayen.
*						 
*						 De fem första värdena placeras in i arrayen. När arrayen är full  flytas värderna
*						 i arrayen ett steg uppåt och det nya värdet placeras sedan in längts ner i arrayen.
*						 
*						 Medelvärdet för tiden placeras sedan i mesure_frequensy. och används för att 
*						 avgöra hur ofta temperaturen skall mätas och skrivas utt.
*
*		- new_arage_ms: Det nya värdet på tiden i milesekunder.
*
*		- time_betwen_presses: statisk array där de 5 senaste tiderna melan knapttryck i milesekunder laggras.
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
*						 och räknar sedan utt medelvärdet på de fem senaste värdena placerade i arrayen.
*
*						 De fem första värdena placeras in i arrayen. När arrayen är full flytas sedan värderna
*						 i arrayen ett steg uppåt och det nya värdet placeras sedan in längst ner i arrayen.
*
*						 Medelvärdet för tiden placeras sedan i avrage_temprature och är det värdet som skrivs utt
*						 till en seriel terminal.
*						
*
*		- new_temp:	Det nya värdet på temperaturen.
*
*		- avrage_temprature: statisk array där de 5 senaste temperaturena lagras.
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
*	temp_calc_avrg_array_value: tar emot en array pekare och räknar utt medlevärdet på
*						        på elementen och returnerar värdet. antal emlement som 
*							    skall aderas anges av array_size. 
*
*		- data_array[]: array vars medelvärde skall skall räknas ut.
*		- array_size: storlek på arrayen som skikas.
*		- avrg_valu: används för att lagra medelvärdet som skall returneras.
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
*	temp_read_temprature: läser av värdet på en analog pin med hjälp av adc_read 
*						  och konverterar sedan det värdet till en temperatur.
*						  konvertering avser ut värdet för en temperatur sensor 
*						  TMP 36. den uträcknade teperaturen returneras.
*					
*	- pin_voltage: sparar värdet för späningen som ligger på avläst pin.
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
*						   Vid två knapptryckningar i följd inom 60 sekunder så sparas tiden
*						   melan knapptryckningarna och används för att räkna utt mätfrekvensen
*						   för mätning av temperatur.
*
*						   Medeltemperaturen från de 5 senaste knapptryckningarna används för att
*						   ange frekvensen för temperaturmätning och uttskrift.
*
*						   Om Knappen trycks in endast en gång och 60 sekunder 
*						   passerar så återstäls systemet och ingen mät frekvens
*						   läses in.
*
*						   För att undvika kontaktstötar så lässes inte en knapptryckning in 
*						   100 milesekunder efter att knappen trycks in.
*
*		- button_paused: Variabel som används för att pausa knapptryck 100 ms efter 
*						 att knappen trycks ner. 
*
*		- button_pause_counter:	Variabel som används för att räkna till antalet avbrot för
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
*						   Efter initiering så läses läses temperaturen in och skriver utt 
*						   medeltemperaturen till Seriel terminal varje 60 sekund.
*
*						   Vid en knapp trycknings så skrivs medel temperaturen ut. tiden melan 
*						   mätningar anges av medelvärdet melan knapptryckningar.
*
*		- mesure counter: Räknare som används för att räkna upp till 5 temperaturmätningar
*						  för att sedan få en medeltemperatur.
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









