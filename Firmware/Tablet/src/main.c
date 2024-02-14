#include <stdio.h>
#include <stdlib.h>
#include <LPC177x_8x.h>
#include "system_LPC177x_8x.h"

#include "Accelerometer.h"
#include "ESP32.h"
#include "Flash.h"
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

#include "Cube.h"
#include "Physics.h"
#include "Pong.h"
#include "Launcher.h"

#include "Colors.h"


volatile unsigned int Continue;
extern volatile uint32_t alarm_on;
extern volatile uint8_t get_touch_coordinates_flag;
//extern volatile uint32_t I2SRXBuffer[MEASUREMENTS_TO_TAKE];

volatile RTCTime local_time, alarm_time, current_time;

struct Sensor_Data sensor_data;



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


void SystemClockUpdate(void){ 
    LPC_SC->CLKSRCSEL = 0x1;
    LPC_SC->PCLKSEL   = 0x1;
    LPC_SC->PLL0CFG   = 0x0A;
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;
    LPC_SC->PLL0CON   = 0x1;
    //LPC_SC->PLL1CON   = 0x1;
    //LPC_SC->PLL1CFG   = 0x02;
    //LPC_SC->PLL1FEED  = 0xAA;
    //LPC_SC->PLL1FEED  = 0x55;
    //LPC_SC->PLL1CON   = 0x1;
}


void HardFault_Handler(void) {
    Continue = 1u;
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
    I2C1_Init(1);   // rtc
    SSP0_Init();


    esp32_init(); 

    RTC_Init();  // just for on-board RTC
    RTCStart();  // just for on-board RTC
    //RTCStop();   // just for on-board RTC
    
    ram_init();

    //flash_test();


    

    //acc_init();
    //acc_read_xyz();

    lcd_init();
    delay_long();
    //i2c_scan_devices();

    //delay_short();  

    //interrupt_test();


    lcd_test();

    lcd_draw_time();

    //lcd_block_test(150,25,600,400);

    delay_short();
    lcd_vfp_interrupt_enable();

    //uint32_t iter = 0;
    //char txtarr[10], txtarr2[10];
    //rtc_read_time();
    //delay_short();
    //rtc_read_time();
    //delay_short();

    //rtc_interrupt_enable();
    //delay_short();

    //rtc_set_time();
    //delay_short();
    //rtc_set_minute_timer();
    //delay_short();
    


    printf("ENTERING MAIN LOOP\n");

    //cube_run_app();
    //physics_run_app();
<<<<<<< Updated upstream
    mic_start_fft();
=======



    //mic_start_fft(); ////////// live audio analysis
    //speech_detection_app(); ////////// speech recognition
    pathfinder_run_app(); ////////// pathfinding




    //for (int i=0; i<16000; i++){
    //    printf("%d\n",i);
    //    delay(1000);
    //}
    //mic_start_speech_analyzer();


    //flash_test();
    //flash_write_audio(NULL, 0);


    int i=0;
>>>>>>> Stashed changes
    while(1){

    }

    printf("Fin\n");
}

