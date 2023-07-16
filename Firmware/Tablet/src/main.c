#include <stdio.h>
#include <stdlib.h>
#include <LPC177x_8x.h>
//#include <LPC17xx.h>
#include "system_LPC177x_8x.h"

#include "Accelerometer.h"
#include "ESP32.h"
//#include "Flash.h"
#include "FFT.h"
#include "GPIO.h"
#include "I2C.h"
#include "Interrupts.h"
#include "LCD.h"
#include "LEDs.h"
#include "Microphone.h"
#include "Power.h"
#include "RAM.h"
#include "RTC.h"
#include "SPI.h"
#include "getdatetime.h"
#include "getdatetime_var.h"

volatile unsigned int Continue;
extern volatile uint32_t alarm_on;
extern volatile uint8_t get_touch_coordinates_flag;
//extern volatile uint32_t I2SRXBuffer[MEASUREMENTS_TO_TAKE];

RTCTime local_time, alarm_time, current_time;

struct Sensor_Data sensor_data;


 //kiss_fft_cpx cx_in[512];
 //kiss_fft_cpx cx_out[258];
 //kiss_fft_cpx *p_in;
 //kiss_fft_cpx *p_out;

extern void delay(uint32_t i);
extern void delay_ms(uint32_t ms);
extern void delay_short(void);
extern void delay_long(void);


void delay(uint32_t i){
	while (i--){}
}


void delay_ms(uint32_t ms)
{
	delay(ms * 20500);
}

void delay_long(void){
    for (uint32_t i=0; i<10000000; i++){}
}


void delay_short(void){
    for (uint32_t i=0; i<1000000; i++){}
}


//void read_pin(void){
//    uint32_t pin_value1, pin_value2;
//    pin_value1 = (LPC_GPIO1->PIN);
//    pin_value2 = (pin_value1>>18) & 0x01;
//    printf("Pin state: %d\t%d\n", pin_value1, pin_value2);
//}


//void pin_toggle(void){

//}


//void pin_test_on(void){
//    LPC_GPIO0->SET |= (0x1<<28);
//}


//void pin_test_off(void){
//    LPC_GPIO0->CLR |= (0x1<<28);
//}


void SystemClockUpdate(void){ 
    //LPC_SC->PLL0CFG = 0x04;
    LPC_SC->CLKSRCSEL = 0x1;
    LPC_SC->PCLKSEL   = 0x1;
    //LPC_SC->CCLKSEL   &= ~(0x1<<8);
    LPC_SC->PLL0CFG   = 0x0B;
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;
    LPC_SC->PLL0CON   = 0x1;
}


void HardFault_Handler(void) {
    Continue = 0u;
    //
    // When stuck here, change the variable value to != 0 in order to step out
    //
    printf("hardfault..\n");
    while (Continue == 0u);
}

int main(void) {
    SystemClockUpdate();
    led_init();
    led_flash(2);

    //I2C0_Init(1); // lcd
    I2C1_Init(1); // rtc
    SSP0_Init();
    //SSP1_Init();

    esp32_init();
    esp32_start_ble();



    //fft_test(); 

    //RTC_Init();  // need to check
    //RTCStart(); 
    //RTCStop();
    
    ram_init();  // generally working, not sure why ram_addr needs to increment by 1 on read
    //ram_test();

    //flash_test();

    
    /* TODO clean up mic fx's - generally working for short msmnts, need to incorporate fft*/  
    //mic_init();
    //mic_take_measurements();
    

    //acc_init();
    //acc_read_xyz();


//delay(50000000);

    printf("ENTERING MAIN LOOP\n");

    lcd_init();
    //i2c_scan_devices();

    //delay_short();  

    //interrupt_test();




    printf("start lcd test\n");
    lcd_test();
    printf("end lcd test\n");

    uint32_t iter = 0;

    while(1){
        printf("%d\n", iter++);
        delay_ms(1000);

    }

    printf("Fin");
}

