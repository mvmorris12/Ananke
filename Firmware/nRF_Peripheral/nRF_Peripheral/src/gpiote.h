#define PIN_IN_RTC_IRQ                  7

void in_pin_handler_rtc(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
void gpiote_init();
void gpiote_uninit(void);
void gpiote_rtc_start(void);
void gpiote_rtc_stop(void);