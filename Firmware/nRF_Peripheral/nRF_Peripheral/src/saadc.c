#include "saadc.h"
#include "nrf_drv_saadc.h"  
#include "nrf_drv_timer.h"
#include "nrf_drv_ppi.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_delay.h"
#include "main.h"

#define SAMPLES_IN_BUFFER 100
#define SAADC_CHANNEL 0

extern struct Sensor_Data sensor_data;

/**
 * @brief Not used in this example, but driver API requiers a callback function to be proivded.
 */
void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
}


/**
 * @brief Function for confguring SAADC channel 0 for sampling AIN3.
 */
void saadc_init(void)
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN3);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(SAADC_CHANNEL, &channel_config);
    APP_ERROR_CHECK(err_code);
}


/**
 * @brief Function for main application entry.
 */
void battery_get_percent(void){
    //NRF_LOG_INFO("SAADC HAL simple example started.");
    nrf_saadc_value_t saadc_sample;
    uint8_t saadc_sample_count = 0;
    uint32_t saadc_avg = 0;
    while (saadc_sample_count < SAMPLES_IN_BUFFER){
        ret_code_t err_code = nrfx_saadc_sample_convert(SAADC_CHANNEL, &saadc_sample);
        APP_ERROR_CHECK(err_code);
        //NRF_LOG_INFO("Sample %d: %i", saadc_sample_count++, saadc_sample);
        //NRF_LOG_FLUSH();
	saadc_sample_count++;
        nrf_delay_ms(5);
	saadc_avg += saadc_sample;
    }
    //NRF_LOG_INFO("saadc done");
    //for (uint8_t i=0; i<SAMPLES_IN_BUFFER; i++){
    //}
    saadc_avg /= SAMPLES_IN_BUFFER;
    //saadc_avg -= 55;
    sensor_data.battery.voltage = saadc_avg;
    NRF_LOG_INFO("   Bat_V: %03X", sensor_data.battery.voltage);
}
