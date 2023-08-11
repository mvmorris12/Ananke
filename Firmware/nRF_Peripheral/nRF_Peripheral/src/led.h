#include "stdint.h"

#define LED_PIN_RED                     8
#define LED_FLASH_DELAY                 125 // milliseconds

void led_init(void);
void led_green_on(void);
void led_green_off(void);
void led_red_on(void);
void led_red_off(void);
void led_flash(uint8_t led_pin, uint16_t on_time_ms, uint8_t flashes);