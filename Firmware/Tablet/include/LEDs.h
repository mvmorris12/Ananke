#include <stdio.h>
#include <stdlib.h>
#include <LPC177x_8x.h>
//#include <LPC17xx.h>
#include "system_LPC177x_8x.h"

#define LED_PIN_GREEN                   8
#define LED_PIN_STATUS_RED              98
#define LED_PIN_STATUS_GREEN            99
#define LED_PIN_STATUS_BLUE             100
#define LED_FLASH_DELAY                 125 // milliseconds

extern void delay_short(void);
extern void delay_long(void);
void led_init(void);
void led_green_on(void);
void led_green_off(void);
void led_flash(uint8_t times_to_flash);
void led_status(uint8_t mode);