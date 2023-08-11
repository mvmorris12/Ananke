#include "led.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "hygrometer.h"
#include "main.h"

extern struct Sensor_Data sensor_data;

void hygrometer_check_connection(void){
    uint8_t rx_buffer[6];
    twi_read(TWI_ADDR_HYGROMETER, HYGROMETER_REG_SERIAL_NUMBER, rx_buffer, sizeof(rx_buffer));
    if ((rx_buffer[0]) || (rx_buffer[1]) || (rx_buffer[2]) || (rx_buffer[3]) || (rx_buffer[4]) || (rx_buffer[5])){
	NRF_LOG_INFO("   Hygrometer \t\t Connected");
    } else {
	NRF_LOG_INFO("   Hygrometer \t\t Not Connected");
    }
}


void hygrometer_get_measurement(void){
    uint8_t rx_buffer[6];
    twi_read(TWI_ADDR_HYGROMETER, HYGROMETER_REG_MEASURE_HI_PRECISION, rx_buffer, sizeof(rx_buffer));
    //NRF_LOG_INFO("   Humidity: %02X %02X %02X %02X %02X %02X", rx_buffer[0], rx_buffer[1], rx_buffer[2], rx_buffer[3], rx_buffer[4], rx_buffer[5]);    
    sensor_data.hygrometer.temperature = ((rx_buffer[0] << 8) + (rx_buffer[1]));
    sensor_data.hygrometer.humidity = ((rx_buffer[3] << 8) + (rx_buffer[4]));;
    NRF_LOG_INFO("   Humidity: %x", sensor_data.hygrometer.humidity);
    NRF_LOG_INFO("   Temperature: %x", sensor_data.hygrometer.temperature);
}
