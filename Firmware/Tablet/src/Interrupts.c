#include "Interrupts.h"
#include "ESP32.h"
#include "LPC177x_8x.h"
#include "type.h"
#include "RTC.h"
#include "LCD.h"
#include "FFT.h"
#include "Colors.h"
#include <stdlib.h>
#include <stdio.h>

extern volatile uint8_t get_touch_coordinates_flag;
extern volatile uint8_t fft_ready;
extern volatile uint8_t tr_ready;
extern volatile RTCTime local_time;
volatile uint32_t eint0_counter;
extern float32_t maxValue;
extern uint16_t touch_x[5], touch_y[5];
uint32_t cnt1 = 0;

/*****************************************************************************
** Function name:		EINT0_Handler
**
** Descriptions:		external INT handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void EINT0_IRQHandler(void){
    LPC_SC->EXTINT = EINT0;		/* clear interrupt */
              
    eint0_counter++;
    if ( eint0_counter & 0x01 )	/* alternate the LED display */
    {
        printf("1\n");

    } else {
        printf("2\n");
    }

}


void EINT2_IRQHandler(void){
    LPC_SC->EXTINT = EINT2;		/* clear interrupt */
              
    eint0_counter++;
    if ( eint0_counter & 0x01 )	/* alternate the LED display */
    {
        printf("3\n");

    } else {
        printf("4\n");
    }

}

void TIMER0_IRQHandler(void){
    NVIC_DisableIRQ(TIMER0_IRQn);
    //printf("timer0 interrupt\n");
    //physics_interrupt_operations();
    LPC_TIM0->IR = 0x1;
    timer0_interrupt_enable();
}


void TIMER1_IRQHandler(void){
    NVIC_DisableIRQ(TIMER1_IRQn);
    printf("timer1 interrupt\n");
    //physics_interrupt_operations();
    LPC_TIM1->IR = 0x1;
    timer1_interrupt_enable();
}


void RTC_IRQHandler(void){
    NVIC_DisableIRQ(RTC_IRQn);
    lcd_draw_time();
    LPC_RTC->ILR |= (0x1);
    //printf("RTC Interrupt\n");
    rtc_interrupt_enable();
}


void LCD_IRQHandler(void){
    NVIC_DisableIRQ(LCD_IRQn);
    //printf("LCD interrupt\n");

    if (fft_ready){
        fft_ready = 0;
        lcd_draw_audio_signal();
        lcd_draw_fft_bins(maxValue);
    }

    if (tr_ready){
        tr_ready = 0;
    }

    LPC_LCD->INTCLR |= (0x1 << 3);
    LPC_LCD->CTRL |= (0x3 << 12);
    LPC_LCD->INTMSK |= (0x1 << 3);

    NVIC_EnableIRQ(LCD_IRQn);
}


void GPIO_IRQHandler(void){
    NVIC_DisableIRQ(GPIO_IRQn);
    //LPC_GPIOINT->IO0IntClr |= (0x1<<12);
    //NVIC_DisableIRQ(GPIO_IRQn);
    get_touch_coordinates_flag = 1;
    //NVIC_EnableIRQ(GPIO_IRQn);
    if (LPC_GPIOINT->IO2IntStatR & (0x1<<1)){
        LPC_GPIOINT->IO2IntClr |= (0x1<<1);
        //printf("esp32 interrupt\n"); // BLE Accelerometer data ready
        esp32_get_accel_data();
        //esp32_rcv();
    }
    if (LPC_GPIOINT->IO0IntStatF & (0x1<<12)){
        LPC_GPIOINT->IO0IntClr |= (0x1<<12);
        printf("lcd interrupt\n");
    }
    NVIC_EnableIRQ(GPIO_IRQn);
}


/*****************************************************************************
** Function name:		EINTInit
**
** Descriptions:		Initialize external interrupt pin and
**						install interrupt handler
**
** parameters:			None
** Returned value:		true or false, return false if the interrupt
**						handler can't be installed to the VIC table.
** 
*****************************************************************************/
uint32_t EINTInit( void )
{
    //LPC_PINCON->PINSEL4 = 0x00100000;	/* set P2.10 as EINT0 and P2.0~7 GPIO output */
    //LPC_GPIO2->FIODIR = 0x000000FF;	/* port 2, bit 0~7 only */
    //LPC_GPIO2->FIOCLR = 0x000000FF;	/* turn off LEDs */

    LPC_IOCON->P0_29 = 0x02;
    LPC_IOCON->P2_19 = 0x00;
    LPC_GPIO0->DIR |= (0x0 << 29);
    LPC_GPIO2->DIR |= (0x0 << 19);

    //p0.29
    //p2.19
  
    LPC_GPIOINT->IO0IntEnF |= (0x1 << 29);	/* Port2.10 is falling edge. */
    LPC_GPIOINT->IO2IntEnF |= (0x1 << 19);	/* Port2.10 is falling edge. */
    LPC_SC->EXTMODE |= EINT2_EDGE;		/* INT0 edge trigger */
    LPC_SC->EXTPOLAR = 0x0;			/* INT0 is falling edge by default */

    NVIC_EnableIRQ(EINT2_IRQn);
    return( TRUE );
  }

  void interrupt_init(void){
      if (EINTInit()){
          printf("interrupts on\n");
      }
}

extern void lcd_touch_interrupt_enable(void){
    LPC_IOCON->P0_12 = (0x1<<3) | (0x1<<7);
    LPC_GPIO0->DIR &= ~(0x1 << 12);
    LPC_GPIOINT->IO0IntEnF |= (0x1 << 12);	/* Port2.10 is falling edge. */
    //LPC_SC->EXTMODE  = EINT0_EDGE;		/* INT0 edge trigger */
    LPC_GPIOINT->IO0IntClr |= (0x1<<12);
    touch_x[0] = 0;
    touch_y[0] = 0;
    NVIC_EnableIRQ(GPIO_IRQn);
}

extern void lcd_touch_interrupt_disable(void){
    NVIC_DisableIRQ(GPIO_IRQn);
}



extern void esp32_interrupt_enable(void){
    LPC_IOCON->P2_1 = (0x1<<3);
    LPC_GPIO2->DIR &= ~(0x1 << 1);
    LPC_GPIOINT->IO2IntEnR |= (0x1 << 1);	
    LPC_GPIOINT->IO2IntClr |= (0x1<<1);
    NVIC_EnableIRQ(GPIO_IRQn);
}

void rtc_interrupt_enable(void){
    //LPC_IOCON->P2_25 = 0x10;
    //LPC_GPIO2->DIR &= ~(0x1 << 25);
    //LPC_GPIOINT->IO2IntEnF |= (0x1 << 25);	
    //LPC_GPIOINT->IO2IntClr |= (0x1<<25);
    //NVIC_EnableIRQ(GPIO_IRQn);
    LPC_RTC->CIIR |= (0x1 << 1);
    NVIC_EnableIRQ(RTC_IRQn);
}

void lcd_vfp_interrupt_enable(void){
    LPC_LCD->CTRL |= (0x3 << 12);
    LPC_LCD->INTMSK |= (0x1 << 3);

    NVIC_EnableIRQ(LCD_IRQn);
}


void timer0_interrupt_enable(void){

    NVIC_EnableIRQ(TIMER0_IRQn);
}


void timer1_interrupt_enable(void){

    NVIC_EnableIRQ(TIMER1_IRQn);
}