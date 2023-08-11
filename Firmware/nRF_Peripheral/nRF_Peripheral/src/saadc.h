#include "stdint.h"
#include "nrf_drv_saadc.h"

void saadc_callback(nrf_drv_saadc_evt_t const * p_event);
void saadc_init(void);
void battery_get_percent(void);