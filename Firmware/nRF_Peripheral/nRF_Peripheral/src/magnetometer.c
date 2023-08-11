#include "led.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "magnetometer.h"
#include "main.h"

extern struct Sensor_Data sensor_data;

void magnetometer_take_measurement(void){
    uint8_t tx_buffer = 0b00100001;
    twi_write(TWI_ADDR_MAGNETOMETER, MAGNETOMETER_REG_INTERNAL_CONTROL_0, &tx_buffer, sizeof(tx_buffer));  
    nrf_delay_us(16700);
}


void magnetometer_read_measurement(void){
    uint8_t measurement_data[9];
    char date_str[20];
    twi_read(TWI_ADDR_MAGNETOMETER, MAGNETOMETER_REG_XOUT0, measurement_data, sizeof(measurement_data));
    sensor_data.magnetometer.x = (measurement_data[0] << 12) + (measurement_data[1] << 4) + (measurement_data[6] >> 4);
    sensor_data.magnetometer.y = (measurement_data[2] << 12) + (measurement_data[3] << 4) + (measurement_data[7] >> 4);
    sensor_data.magnetometer.z = (measurement_data[4] << 12) + (measurement_data[5] << 4) + (measurement_data[8] >> 4);
    NRF_LOG_INFO("   Mag_x: %x", sensor_data.magnetometer.x);
    NRF_LOG_INFO("   Mag_y: %x", sensor_data.magnetometer.y);
    NRF_LOG_INFO("   Mag_z: %x", sensor_data.magnetometer.z);
}


void magnetometer_get_measurement(void){
    magnetometer_take_measurement();
    magnetometer_read_measurement();
}
