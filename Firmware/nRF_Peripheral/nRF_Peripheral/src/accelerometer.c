#include "accelerometer.h"
#include "spi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

extern volatile bool spi_xfer_done;

void set_pin(uint32_t pin, uint8_t onoff){
    nrf_gpio_cfg_output(pin);
    if (onoff == 1){ 
        nrf_gpio_pin_set(pin);
    } else { 
        nrf_gpio_pin_clear(pin);
    }
}
void ism_init(void){
    set_pin(ACC_ENABLE_PIN, 1);
    set_pin(ACC_CS_PIN, 1);
    nrf_delay_ms(500);
    ism_write(ACC_CTRL1_XL, 0x80);
    ism_write(ACC_CTRL2_G, 0x48);  
    ism_write(ACC_CTRL4_C, 0x04);  

    //ism_write(ACC_ACCEL_CONFIG0, 0x63);  //Old Accelerometer
    //ism_write(ACC_PWR_MGMT0, 0x13);	   //Old Accelerometer
}


void ism_write(uint8_t reg, uint8_t value){
    spi_xfer_done = false;
	
    uint8_t spiCommandData[] = {0x00, 0x00};
    spiCommandData[0] = reg;
    spiCommandData[0] = spiCommandData[0] & 0x7f;
    spiCommandData[1] = value;
    uint8_t spiReadData[] = {0x00, 0x00};
    set_pin(ACC_CS_PIN, 0);
    set_pin(LCD_DC, 0);
    //APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spiCommandData, 2, spiReadData, 2));
    spi_master_tx_fast(ACC_CS_PIN, SPI_INSTANCE, 2, spiCommandData);
    
 //   while (!spi_xfer_done)
 //   {
	//__WFE();
 //   }
    set_pin(ACC_CS_PIN, 1);
    set_pin(LCD_DC, 1);
    NRF_LOG_INFO("wrote %x to ism %x", spiCommandData[1], reg);

    NRF_LOG_FLUSH();

}


int ism_read(uint8_t reg){
    spi_xfer_done = false;
	
    uint8_t spiCommandData[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    spiCommandData[0] = reg;
    spiCommandData[0] = spiCommandData[0] | 0x80;
    uint8_t spiReadData[] = {0x00, 0x00};
    //APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spiCommandData, 2, spiReadData, 2));
    nrf_gpio_pin_clear(ACC_CS_PIN);
    //set_pin(LCD_DC, 0);
    //spi_master_tx_fast(ACC_CS_PIN, ST7789_SPI_INSTANCE, 1, spiCommandData);
    nrf_drv_spi_transfer( SPI_INSTANCE, 2, spiCommandData, 2, spiReadData);
    nrf_gpio_pin_set(ACC_CS_PIN);
    //set_pin(LCD_DC, 1);

    if (reg == ACC_WHOAMI && spiReadData[1] == 0x6A){
	NRF_LOG_INFO("read %x: %x", reg, spiReadData[1]);
	NRF_LOG_INFO("Accelerometer: ON");
    } else if (reg == ACC_WHOAMI) { 
	NRF_LOG_INFO("ism ERROR.....   read %x: %x %x %x %x", reg, spiReadData[0], spiReadData[1], spiReadData[2], spiReadData[3]);
	NRF_LOG_INFO("ism ERROR.....   read %x: %x %x %x %x", reg, spiReadData[4], spiReadData[5], spiReadData[6], spiReadData[7]);
	//NRF_LOG_INFO("ism ERROR.....");
	//NRF_LOG_FLUSH();
	//nrf_delay_ms(10000);
    }
    //NRF_LOG_INFO("read %x: %x", reg, spiReadData[1]);
    NRF_LOG_FLUSH();
}