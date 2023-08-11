#define LOGGER_ENABLED 1

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

struct Sensor_Data{
    struct Thermometer_Barometer{
	uint32_t temperature;
	float ftemperature;
	uint32_t pressure;
	float fpressure;
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
    struct RTC{
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        //uint32_t epoch_s;
    } rtc;
};

extern struct Sensor_Data sensor_data;

#endif