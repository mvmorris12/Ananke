/**
 * Copyright (c) 2014 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 * @defgroup rtc_example_main main.c
 * @{
 * @ingroup rtc_example
 * @brief Real Time Counter Example Application main file.
 *
 * This file contains the source code for a sample application using the Real Time Counter (RTC).
 *
 */

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_gpiote.h"
#include "boards.h"
#include "app_error.h"
#include "C:\Users\mimorris\Documents\Sensors\LaggingWear\Firmware\laggingwear-\nRF5_SDK_17\components\libraries\delay\nrf_delay.h"
#include <stdint.h>
#include <stdbool.h>

#define COMPARE_COUNTERTIME  (3UL)  
#define SYNC_INTERRUPT_PIN   (31U)                                      /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */

#ifdef BSP_LED_0
    #define TICK_EVENT_OUTPUT     27                                 /**< Pin number for indicating tick event. */
#endif
#ifndef TICK_EVENT_OUTPUT
    #error "Please indicate output pin"
#endif
#ifdef BSP_LED_1
    #define COMPARE_EVENT_OUTPUT   28                                /**< Pin number for indicating compare event. */
#endif
#ifndef COMPARE_EVENT_OUTPUT
    #error "Please indicate output pin"
#endif

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0); /**< Declaring an instance of nrf_drv_rtc for RTC0. */


volatile bool sync_time_flag = false;
int interrupt_count = 0;
volatile bool ble_disconnect_flag = false;






/** @brief: Function for handling the RTC0 interrupts.
 * Triggered on TICK and COMPARE0 match.
 */


 uint32_t temp = 0;

static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
    nrf_drv_rtc_t const *const 	p_instance;
    //temp = 
    if (NRF_RTC2->COUNTER==0){
        nrf_gpio_pin_set(25);
        nrf_delay_us(5);
        nrf_gpio_pin_clear(25);
    }
    //nrf_drv_rtc_counter_clear(&rtc);
}

/** @brief Function configuring gpio for pin toggling.
 */
static void leds_config(void)
{
    nrf_gpio_cfg_output(25);
}

/** @brief Function starting the internal LFCLK XTAL oscillator.
 */
static void lfclk_config(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}

/** @brief Function initialization and configuration of RTC driver instance.
 */
void rtc_config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    config.prescaler = 4095;
    //config.prescaler = 0;
    err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc,true);

    //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
    err_code = nrf_drv_rtc_cc_set(&rtc,0,COMPARE_COUNTERTIME*8,true);
    APP_ERROR_CHECK(err_code);

    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);
}










int intr = 0;

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    //NRF_LOG_INFO("pin change\t%d", intr++);
    //nrf_drv_gpiote_out_toggle(PIN_OUT);
    //nrf_delay_ms(100);
    //NRF_LOG_INFO("done");
    //if ((sync_time_flag) && (interrupt_count++ > 1)){
        //nrf_drv_rtc_counter_clear(&rtc);
        //ble_disconnect_flag = true;
        nrf_gpio_pin_set(25);
        nrf_gpio_pin_clear(25);
        //nrf_gpio_pin_set(25);
        //nrf_gpio_pin_clear(25);
        //nrf_gpio_pin_set(25);
        //nrf_gpio_pin_clear(25);
        //sync_time_flag = false;
        //gpiote_stop();
        //led_green_on();
        //nrf_delay_ms(20);
        //led_green_off();
        //NRF_LOG_INFO("sync now");
    //}
}








void gpiote_init()
{
    ret_code_t err_code;

    nrf_drv_gpiote_init();
    //APP_ERROR_CHECK(err_code);

    //nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);

    //err_code = nrf_drv_gpiote_out_init(27, &out_config);
    //APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_PULLDOWN;
    in_config.sense = GPIOTE_CONFIG_POLARITY_HiToLo;
    in_config.is_watcher = false;
    in_config.hi_accuracy = false;
    

    err_code = nrf_drv_gpiote_in_init(SYNC_INTERRUPT_PIN, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);

}

void gpiote_start(void){
    nrf_drv_gpiote_in_event_enable(SYNC_INTERRUPT_PIN, true);
    //NRF_LOG_INFO("GPIOTE started");
}

void gpiote_stop(void){
    nrf_drv_gpiote_in_event_disable(SYNC_INTERRUPT_PIN);
    //NRF_LOG_INFO("GPIOTE stopped");
}


void gpiote_uninit(void){
    if(!nrf_drv_gpiote_is_init()){
        nrf_drv_gpiote_uninit();
        //NRF_LOG_INFO("GPIOTE uninitialized");
    } else { 
        //NRF_LOG_INFO("GPIOTE already uninitialized");
        nrf_drv_gpiote_in_event_disable(SYNC_INTERRUPT_PIN);
    }
}








/**
 * @brief Function for application main entry.
 */
int main(void)
{ 
    //nrf_gpio_cfg_output(5);
    //nrf_gpio_pin_set(5);
    leds_config();

    lfclk_config();

    rtc_config();

    
    gpiote_init();
    gpiote_start();

    while (true)
    {
        __SEV();
        __WFE();
        __WFE();
    }
}


/**  @} */
