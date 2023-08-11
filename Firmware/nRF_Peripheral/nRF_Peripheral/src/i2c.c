#include "i2c.h"
#include "hygrometer.h"

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_delay.h"

#define TWI_INSTANCE_ID     0

#ifndef TWI_INST
#define TWI_INST
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
#endif



void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
       .scl                = TWI_PIN_SCL,
       .sda                = TWI_PIN_SDA,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}


void twi_check_sensors(void){
    uint8_t sample_data;
    ret_code_t err_code;
    char sensor_names[] = "Magnetometer";
    char tab_chars[] = "\t\t";
    uint8_t sensor_addr = TWI_ADDR_MAGNETOMETER;
    NRF_LOG_INFO("Device Status:");
    err_code = nrf_drv_twi_rx(&m_twi, sensor_addr, &sample_data, sizeof(sample_data));
    if (err_code == NRF_SUCCESS){
        NRF_LOG_INFO("   %s %s Connected", sensor_names, tab_chars);
    } else {
        NRF_LOG_INFO("   %s %s Not Connected", sensor_names, tab_chars);
    }
}


bool twi_write(uint8_t device, uint8_t regNumber, uint8_t *tx_buffer,  uint8_t length){
  ret_code_t err_code;
  uint8_t valueBuffer[length+1];
  valueBuffer[0] = regNumber;
  for (uint8_t i=0; i<length; i++){
      valueBuffer[i+1] = tx_buffer[i];
  }
  err_code = nrf_drv_twi_tx(&m_twi, device, valueBuffer, sizeof(valueBuffer), false);
  APP_ERROR_CHECK(err_code);
  for (uint8_t i=0; i<length; i++){
      tx_buffer[i] = valueBuffer[i+1];
  }
  return 1;
}


bool twi_read(uint8_t device, uint8_t regNumber, uint8_t *rx_buffer,  uint8_t length){
  ret_code_t err_code;
  //uint8_t results[length];
  uint8_t reg = regNumber;
  err_code = nrf_drv_twi_tx(&m_twi, device, &reg, 1, false);
  if (device == TWI_ADDR_HYGROMETER){
      nrf_delay_ms(9);
  }
  err_code = nrf_drv_twi_rx(&m_twi, device, rx_buffer, length);
  APP_ERROR_CHECK(err_code);
  return 1;
}