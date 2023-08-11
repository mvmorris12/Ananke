#include <stdint.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_timer.h"
#include "gpiote.h"
#include "rtc.h"

extern bool take_measurement_flag;

/*
GPIOTE interrupt handler for rtc
*/
void in_pin_handler_rtc(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    //NRF_LOG_INFO("RTC interrupt triggered");
    take_measurement_flag = true;
}


/*
Initializes GPIOTE for comparator and accelerometer interrupts. 
*/
void gpiote_init()
{
    ret_code_t err_code;

    nrf_drv_gpiote_init();

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_NOPULL;
    //in_config.pull = NRF_GPIO_PIN_PULLUP;
    in_config.sense = GPIOTE_CONFIG_POLARITY_HiToLo;
    in_config.is_watcher = false;
    in_config.hi_accuracy = false;
   
    err_code = nrf_drv_gpiote_in_init(PIN_IN_RTC_IRQ, &in_config, in_pin_handler_rtc);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("GPIOTE initialized");
    gpiote_rtc_start();
}



/*
Uninitializes GPIOTE
*/ //TODO check if this is right
void gpiote_uninit(void){
    if(!nrf_drv_gpiote_is_init()){
        nrf_drv_gpiote_uninit();
        NRF_LOG_INFO("GPIOTE uninitialized");
    }
}

/*
Enables GPIOTE event for accelerometer interrupt 1
*/
void gpiote_rtc_start(void){
    nrf_drv_gpiote_in_event_enable(PIN_IN_RTC_IRQ, true);
    //NRF_LOG_INFO("RTC interrupt armed")
}


/*
Disables GPIOTE event for accelerometer interrupt 1
*/
void gpiote_rtc_stop(void){
    nrf_drv_gpiote_in_event_disable(PIN_IN_RTC_IRQ);
}

