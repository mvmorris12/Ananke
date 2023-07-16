#include <stdio.h>
#include <stdlib.h>
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"



//extern void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
//{
//	( void ) pcTaskName;
//	( void ) pxTask;

//	/* Run time stack overflow checking is performed if
//	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
//	function is called if a stack overflow is detected. */
//	taskDISABLE_INTERRUPTS();
//	for( ;; );
//}

void vConfigureTimerForRunTimeStats( void ) {
    //CMU->HFRCOCTRL = 0x8;                           // Set High Freq. RC Osc. to 1 MHz
    //CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_TIMER3;     // Enable clock for Timer3
    //TIMER3->CNT = 0x0;
    //TIMER3->CTRL = ( TIMER3->CTRL & ~_TIMER_CTRL_PRESC_MASK) | TIMER_CTRL_PRESC_DIV1024; // Prescaler needed to reduce resolution, 1024
    //TIMER3->CMD = 0x1;                              // Start Timer3
}

unsigned long vGetTimerForRunTimeStats( void ) {
    //return ( TIMER3->CNT );
}


extern void vApplicationTickHook();
void vApplicationTickHook()
{
    vApplicationTickHook();
}


void hello_world_task(void* p)
{
    while(1) {
        puts("Hello World!");
        vTaskDelay(1000);
    }
}

int main()
{
    xTaskCreate(hello_world_task, (signed char*)"task_name", 400, 0, 1, 0);
    vTaskStartScheduler();

    return -1;
}

/*************************** End of file ****************************/
