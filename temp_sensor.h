/*
 * temp_sensor.h
 *
 * Created: 2023-01-01 22:23:55
 *  Author: willi
 */ 
#include "timer.h"
#include "misc.h"
#include "serial.h"


#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

void temp_init(void);

void serial_print_temp();

#endif /* TEMP_SENSOR_H_ */