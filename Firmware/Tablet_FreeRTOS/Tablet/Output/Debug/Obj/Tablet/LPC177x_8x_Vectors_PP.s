# 1 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC177x_8x_Vectors.s"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC177x_8x_Vectors.s"
# 61 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC177x_8x_Vectors.s"
        .syntax unified
# 73 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC177x_8x_Vectors.s"
.macro VECTOR Name=
        .section .vectors, "ax"
        .code 16
        .word \Name
.endm




.macro EXC_HANDLER Name=



        .section .vectors, "ax"
        .word \Name



        .section .init.\Name, "ax"
        .thumb_func
        .weak \Name
        .balign 2
\Name:
        1: b 1b
.endm




.macro ISR_HANDLER Name=



        .section .vectors, "ax"
        .word \Name
# 116 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC177x_8x_Vectors.s"
        .section .init.\Name, "ax"
        .thumb_func
        .weak \Name
        .balign 2
\Name:
        1: b 1b

.endm




.macro ISR_RESERVED
        .section .vectors, "ax"
        .word 0
.endm




.macro ISR_RESERVED_DUMMY
        .section .vectors, "ax"
        .word Dummy_Handler
.endm







        .extern __stack_end__
        .extern Reset_Handler
        .extern HardFault_Handler
# 163 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC177x_8x_Vectors.s"
        .section .vectors, "ax"
        .code 16
        .balign 256
        .global _vectors
_vectors:



        VECTOR __stack_end__
        VECTOR Reset_Handler
        EXC_HANDLER NMI_Handler
        VECTOR HardFault_Handler
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        EXC_HANDLER SVC_Handler
        ISR_RESERVED
        ISR_RESERVED
        EXC_HANDLER PendSV_Handler
        EXC_HANDLER SysTick_Handler




        ISR_HANDLER WDT_IRQHandler
        ISR_HANDLER TIMER0_IRQHandler
        ISR_HANDLER TIMER1_IRQHandler
        ISR_HANDLER TIMER2_IRQHandler
        ISR_HANDLER TIMER3_IRQHandler
        ISR_HANDLER UART0_IRQHandler
        ISR_HANDLER UART1_IRQHandler
        ISR_HANDLER UART2_IRQHandler
        ISR_HANDLER UART3_IRQHandler
        ISR_HANDLER PWM1_IRQHandler
        ISR_HANDLER I2C0_IRQHandler
        ISR_HANDLER I2C1_IRQHandler
        ISR_HANDLER I2C2_IRQHandler
        ISR_HANDLER SPIFI_IRQHandler
        ISR_HANDLER SSP0_IRQHandler
        ISR_HANDLER SSP1_IRQHandler
        ISR_HANDLER PLL0_IRQHandler
        ISR_HANDLER RTC_IRQHandler
        ISR_HANDLER EINT0_IRQHandler
        ISR_HANDLER EINT1_IRQHandler
        ISR_HANDLER EINT2_IRQHandler
        ISR_HANDLER EINT3_IRQHandler
        ISR_HANDLER ADC_IRQHandler
        ISR_HANDLER BOD_IRQHandler
        ISR_HANDLER USB_IRQHandler
        ISR_HANDLER CAN_IRQHandler
        ISR_HANDLER DMA_IRQHandler
        ISR_HANDLER I2S_IRQHandler
        ISR_HANDLER ENET_IRQHandler
        ISR_HANDLER MCI_IRQHandler
        ISR_HANDLER MCPWM_IRQHandler
        ISR_HANDLER QEI_IRQHandler
        ISR_HANDLER PLL1_IRQHandler
        ISR_HANDLER USBActivity_IRQHandler
        ISR_HANDLER CANActivity_IRQHandler
        ISR_HANDLER UART4_IRQHandler
        ISR_HANDLER SSP2_IRQHandler
        ISR_HANDLER LCD_IRQHandler
        ISR_HANDLER GPIO_IRQHandler
        ISR_HANDLER PWM0_IRQHandler
        ISR_HANDLER EEPROM_IRQHandler


        .section .vectors, "ax"
_vectors_end:
# 256 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC177x_8x_Vectors.s"
        .section .init.Dummy_Handler, "ax"
        .thumb_func
        .weak Dummy_Handler
        .balign 2
Dummy_Handler:
        1: b 1b
