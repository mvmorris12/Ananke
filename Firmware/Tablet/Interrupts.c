#include "Interrupts.h"
#include "ESP32.h"
#include "LPC177x_8x.h"
#include "type.h"
#include <stdlib.h>
#include <stdio.h>

extern volatile uint8_t get_touch_coordinates_flag;
volatile uint32_t eint0_counter;
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
void EINT0_IRQHandler(void) 
{
    LPC_SC->EXTINT = EINT0;		/* clear interrupt */
              
    eint0_counter++;
    if ( eint0_counter & 0x01 )	/* alternate the LED display */
    {
        printf("1\n");

    } else {
        printf("2\n");
    }

}


void EINT2_IRQHandler(void) 
{
    LPC_SC->EXTINT = EINT2;		/* clear interrupt */
              
    eint0_counter++;
    if ( eint0_counter & 0x01 )	/* alternate the LED display */
    {
        printf("3\n");

    } else {
        printf("4\n");
    }

}

void GPIO_IRQHandler(void){
    //LPC_GPIOINT->IO0IntClr |= (0x1<<12);
    //NVIC_DisableIRQ(GPIO_IRQn);
    //get_touch_coordinates_flag = 1;
    //NVIC_EnableIRQ(GPIO_IRQn);
    if (LPC_GPIOINT->IO2IntStatF & (0x1<<1)){
        NVIC_DisableIRQ(GPIO_IRQn);
        LPC_GPIOINT->IO2IntClr |= (0x1<<1);
        printf("interrupt P2.1 - BLE Accelerometer data ready\n");
        esp32_get_accel_data();
        //esp32_rcv();
        NVIC_EnableIRQ(GPIO_IRQn);
    }
    if (LPC_GPIOINT->IO0IntStatF & (0x1<<12)){
        NVIC_DisableIRQ(GPIO_IRQn);
        LPC_GPIOINT->IO0IntClr |= (0x1<<12);
        printf("lcd interrupt\n");
        NVIC_EnableIRQ(GPIO_IRQn);
    }

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

extern void lcd_interrupt_enable(void){
    LPC_IOCON->P0_12 = (0x1<<3) | (0x1<<7);
    LPC_GPIO0->DIR &= ~(0x1 << 12);
    LPC_GPIOINT->IO0IntEnF |= (0x1 << 12);	/* Port2.10 is falling edge. */
    //LPC_SC->EXTMODE  = EINT0_EDGE;		/* INT0 edge trigger */
    NVIC_EnableIRQ(GPIO_IRQn);
}

extern void esp32_interrupt_enable(void){
    LPC_IOCON->P2_1 = (0x1<<3);
    LPC_GPIO2->DIR &= ~(0x1 << 1);
    LPC_GPIOINT->IO2IntEnF |= (0x1 << 1);	
    LPC_GPIOINT->IO2IntClr |= (0x1<<1);
    NVIC_EnableIRQ(GPIO_IRQn);
}