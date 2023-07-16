#define LOGGER_ENABLED 1

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

struct Sensor_Data{
    struct Thermometer_Barometer{
	uint32_t temperature;
	uint32_t pressure;
    } thermometer_barometer;
    struct Magnetometer{
	uint32_t x;
	uint32_t y;
	uint32_t z;
    } magnetometer;
    struct Hygrometer{
	uint16_t temperature;
	uint16_t humidity;
    } hygrometer;
    struct Battery{
	uint16_t voltage;
    } battery;
    //struct RTC {
	
    //}
};

extern struct Sensor_Data sensor_data;

#endif