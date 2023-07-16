#include "LEDs.h"


void led_init(void){
    LPC_SC->PCONP |= (0x1<<15); // Enable GPIO module
    LPC_IOCON->P1_18 = 0x0;
    LPC_IOCON->P0_10 = 0x0;
    LPC_IOCON->P2_15 = 0x0;
    LPC_IOCON->P0_11 = 0x0;
    LPC_GPIO1->DIR |= (0x1<<18); 
    LPC_GPIO0->DIR |= (0x1<<10); 
    LPC_GPIO2->DIR |= (0x1<<15); 
    LPC_GPIO0->DIR |= (0x1<<11); 
    led_status(0);
}


void led_green_on(void){
    LPC_GPIO1->CLR |= (0x1<<18);
}


void led_green_off(void){
    LPC_GPIO1->SET |= (0x1<<18);
}

void led_status(uint8_t mode){
    switch(mode){
        case 0:    // all off
            LPC_GPIO0->SET |= (0x1<<10); 
            LPC_GPIO2->SET |= (0x1<<15);
            LPC_GPIO0->SET |= (0x1<<11);
            break;
        case 1:     // red on
            LPC_GPIO0->CLR |= (0x1<<10);            
            break;
        case 2:     // green on
            LPC_GPIO2->CLR |= (0x1<<15);
            break;
        case 3:     // blue on
            LPC_GPIO0->CLR |= (0x1<<11);
            break;
    }
}

void led_flash(uint8_t times_to_flash){
    for (uint8_t i=0; i<times_to_flash; i++){
        led_green_on();
        delay_short();
        led_green_off();
        delay_short();
    }
}
