#include "LPC177x_8x.h"
#include "Interrupts.h"

void timer0_init(void){
    LPC_SC->PCONP |= (0x1 << 1);
    //NVIC
    LPC_TIM0->MCR |= (0x1) | (0x1 << 1);
    LPC_TIM0->MR0 |= (0x1); 
    LPC_TIM0->PR = 600000;
    //LPC_TIM0->PC = 50000000;
    LPC_TIM0->TCR = (0x1);
    timer0_interrupt_enable();
    
}