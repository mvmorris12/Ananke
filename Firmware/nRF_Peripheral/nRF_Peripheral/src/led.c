#include "led.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"



/*
Configures LED pins as output
*/
void led_init(void){
    nrf_gpio_cfg_output(LED_PIN_RED);
}

/*
Turns red LED on
*/
void led_red_on(void){
    nrf_gpio_pin_set(LED_PIN_RED);
}

/*
Turns red LED off
*/
void led_red_off(void){
    nrf_gpio_pin_clear(LED_PIN_RED);
}


/*
Function for flashing leds:
    led_pin       which LED to flash
    on_time_ms    Duration in ms that LED is on
    flashes       # of times to flash
*/
void led_flash(uint8_t led_pin, uint16_t on_time_ms, uint8_t flashes){
    for (uint8_t i=0; i<flashes; i++){
	led_red_on();
	nrf_delay_ms(on_time_ms);
	led_red_off();
	nrf_delay_ms(125);
    }
}

