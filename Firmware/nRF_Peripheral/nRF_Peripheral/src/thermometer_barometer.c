#include "led.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "thermometer_barometer.h"
#include "main.h"

extern struct Sensor_Data sensor_data;
extern struct Calib_Data calib_data;

void thermometer_barometer_init(void){
    uint8_t tx_buffer = 0b00000000;
    twi_write(TWI_ADDR_THERMOMETER_BAROMETER, TB_REG_FIFO_CONFIG_1, &tx_buffer, sizeof(tx_buffer));	
    tx_buffer = 0b00001101;
    twi_write(TWI_ADDR_THERMOMETER_BAROMETER, TB_REG_OSR, &tx_buffer, sizeof(tx_buffer));	  
}



void thermometer_barometer_take_measurement(void){
    uint8_t tx_buffer = 0b00100011;
    twi_write(TWI_ADDR_THERMOMETER_BAROMETER, TB_REG_PWR_CTRL, &tx_buffer, sizeof(tx_buffer));
    //NRF_LOG_INFO("Temperature & pressure sensors enabled");
}


//void thermometer_barometer_disable_sensors(void){
//    uint8_t tx_buffer = 0b00000000;
//    twi_write(TWI_ADDR_THERMOMETER_BAROMETER, TB_REG_PWR_CTRL, &tx_buffer, sizeof(tx_buffer));
//    //NRF_LOG_INFO("Temperature & pressure sensors disabled");
//}


void thermometer_barometer_read_measurement(void){
    uint8_t measurement_data[6];
    //char date_str[20];
    twi_read(TWI_ADDR_THERMOMETER_BAROMETER, TB_REG_PRESSURE_DATA_0, measurement_data, sizeof(measurement_data));
    sensor_data.thermometer_barometer.pressure = ((measurement_data[2]) << 16) | (measurement_data[1] << 8) | (measurement_data[0]);
    sensor_data.thermometer_barometer.temperature = (measurement_data[5] << 16) | (measurement_data[4] << 8) | (measurement_data[3]);
    sensor_data.thermometer_barometer.ftemperature = thermometer_barometer_compensate_temperature(sensor_data.thermometer_barometer.temperature);
    sensor_data.thermometer_barometer.fpressure = thermometer_barometer_compensate_pressure(sensor_data.thermometer_barometer.pressure);
    sensor_data.thermometer_barometer.temperature = *(int*)&sensor_data.thermometer_barometer.ftemperature;
    sensor_data.thermometer_barometer.pressure = *(int*)&sensor_data.thermometer_barometer.fpressure;
    NRF_LOG_INFO("   Temperature: %x", sensor_data.thermometer_barometer.temperature);
    NRF_LOG_INFO("   Pressure: %x", sensor_data.thermometer_barometer.pressure);
}


void thermometer_barometer_get_measurement(void){
    thermometer_barometer_take_measurement();
    nrf_delay_ms(80);
    thermometer_barometer_read_measurement();
}


float thermometer_barometer_compensate_temperature(uint32_t uncomp_temp){
    float partial_data1;
    float partial_data2;
    partial_data1 = (float)(uncomp_temp - calib_data.fpar_t1);
    partial_data2 = (float)(partial_data1 * calib_data.fpar_t2);
    /* Update the compensated temperature in calib structure since this is
    * needed for pressure calculation */
    calib_data.t_lin = partial_data2 + (partial_data1 * partial_data1) * calib_data.fpar_t3;
    /* Returns compensated temperature */
    return calib_data.t_lin;
}


float thermometer_barometer_compensate_pressure(uint32_t uncomp_press){
    /* Variable to store the compensated pressure */
    float comp_press;
    /* Temporary variables used for compensation */
    float partial_data1;
    float partial_data2;
    float partial_data3;
    float partial_data4;
    float partial_out1;
    float partial_out2;
    /* Calibration data */
    partial_data1 = (float)calib_data.fpar_p5 + (calib_data.fpar_p6 * calib_data.t_lin);
    partial_data2 = calib_data.fpar_p7 * (calib_data.t_lin * calib_data.t_lin);
    partial_data3 = calib_data.fpar_p8 * (calib_data.t_lin * calib_data.t_lin * calib_data.t_lin);
    partial_out1  = partial_data1 + partial_data2 + partial_data3;
    partial_data1 = calib_data.fpar_p2 * calib_data.t_lin;
    partial_data2 = calib_data.fpar_p3 * (calib_data.t_lin * calib_data.t_lin);
    partial_data3 = calib_data.fpar_p4 * (calib_data.t_lin * calib_data.t_lin * calib_data.t_lin);
    partial_out2  = (float)uncomp_press * (calib_data.fpar_p1 + partial_data1 + partial_data2 + partial_data3);
    partial_data1 = (float)uncomp_press * (float)uncomp_press;
    partial_data2 = calib_data.fpar_p9 + calib_data.fpar_p10 * calib_data.t_lin;
    partial_data3 = partial_data1 * partial_data2;
    partial_data4 = partial_data3 + ((float)uncomp_press * (float)uncomp_press * (float)uncomp_press) * calib_data.fpar_p11;
    comp_press = partial_out1 + partial_out2 + partial_data4;

    return comp_press;
}

void thermometer_barometer_read_calibration(void){
    uint8_t calib_reg_data[21];
    twi_read(TWI_ADDR_THERMOMETER_BAROMETER, TB_REG_NVM_PAR_T1_0, calib_reg_data, sizeof(calib_reg_data));
    //NRF_LOG_INFO("%02X %02X %02X %02X %02X", calib_reg_data[0], calib_reg_data[1], calib_reg_data[2], calib_reg_data[3], calib_reg_data[4]);
    // temperature registers
    calib_data.par_t1 = (calib_reg_data[0]) | (calib_reg_data[1]<<8);
    calib_data.par_t2 = (calib_reg_data[2]) | (calib_reg_data[3]<<8);
    calib_data.par_t3 = (calib_reg_data[4]); 
    calib_data.fpar_t1 = calib_data.par_t1 * (0x2<<7);
    calib_data.fpar_t2 = ((float)calib_data.par_t2) / (0x2<<29);
    calib_data.fpar_t3 = ((float)(calib_data.par_t3) / (0x2<<26));
    calib_data.fpar_t3 = (calib_data.fpar_t3) / (0x2<<20);
    // pressure registers
    calib_data.par_p1 = (calib_reg_data[5]) | (calib_reg_data[6]<<8);
    calib_data.par_p2 = (calib_reg_data[7]) | (calib_reg_data[8]<<8);
    calib_data.par_p3 = (calib_reg_data[9]);
    calib_data.par_p4 = (calib_reg_data[10]);
    calib_data.par_p5 = (calib_reg_data[11]) | (calib_reg_data[12]<<8);
    calib_data.par_p6 = (calib_reg_data[13]) | (calib_reg_data[14]<<8);
    calib_data.par_p7 = (calib_reg_data[15]);
    calib_data.par_p8 = (calib_reg_data[16]);
    calib_data.par_p9 = (calib_reg_data[17]) | (calib_reg_data[18]<<8);
    calib_data.par_p10 = (calib_reg_data[19]);
    calib_data.par_p11 = (calib_reg_data[20]);
    calib_data.fpar_p1 = ((float)calib_data.par_p1 - (0x2<<13)) / (0x2<<19);
    calib_data.fpar_p2 = ((float)calib_data.par_p2 - (0x2<<13)) / (0x2<<28);
    calib_data.fpar_p3 = ((float)calib_data.par_p3) / (0x2<<10);
    calib_data.fpar_p3 = (calib_data.fpar_p3) / (0x2<<20);
    calib_data.fpar_p4 = ((float)calib_data.par_p4) / (0x2<<15);
    calib_data.fpar_p4 = (calib_data.fpar_p4) / (0x2<<20);
    calib_data.fpar_p5 = (calib_data.par_p5) * 8.0;
    calib_data.fpar_p6 = ((float)calib_data.par_p6) / (0x2<<5);
    calib_data.fpar_p7 = ((float)calib_data.par_p7) / (0x2<<7);
    calib_data.fpar_p8 = ((float)calib_data.par_p8) / (0x2<<14);
    calib_data.fpar_p9 = ((float)calib_data.par_p9) / (0x2<<26);
    calib_data.fpar_p9 = (calib_data.fpar_p9) / (0x2<<20);
    calib_data.fpar_p10 = ((float)calib_data.par_p10) / (0x2<<26);
    calib_data.fpar_p10 = (calib_data.fpar_p10) / (0x2<<20);
    calib_data.fpar_p11 = ((float)calib_data.par_p11) / (0x2<<22);
    calib_data.fpar_p11 = (calib_data.fpar_p11) / (0x2<<20);
    calib_data.fpar_p11 = (calib_data.fpar_p11) / (0x2<<20);
}