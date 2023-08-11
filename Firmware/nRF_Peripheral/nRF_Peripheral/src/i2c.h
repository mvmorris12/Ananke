#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_delay.h"

#define TWI_PIN_SCL			  26
#define TWI_PIN_SDA			  25
#define TWI_ADDR_THERMOMETER_BAROMETER	  0x76
#define TWI_ADDR_MAGNETOMETER		  0x30	// R/W on 0x7E = pwr cycle to switch I3C -> I2C
#define TWI_ADDR_HYGROMETER		  0x44
#define TWI_ADDR_RTC			  0x51

void twi_init(void);
void twi_check_sensors(void);
//bool twi_write(uint8_t device, uint8_t regNumber, uint8_t value);
bool twi_write(uint8_t device, uint8_t regNumber, uint8_t *tx_buffer,  uint8_t length);
bool twi_read(uint8_t device, uint8_t regNumber, uint8_t *rx_buffer, uint8_t length);