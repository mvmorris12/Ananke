# 1 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC1700_Startup.s"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC1700_Startup.s"
# 91 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC1700_Startup.s"
        .syntax unified
# 119 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC1700_Startup.s"
        .global reset_handler
        .global Reset_Handler
        .equ reset_handler, Reset_Handler
        .section .init.Reset_Handler, "ax"
        .balign 2
        .thumb_func
Reset_Handler:






        movs R0, #8
        ldr R0, [R0]
        nop
        nop





        bl SystemInit
# 197 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC1700_Startup.s"
        bl _start





        .weak SystemInit





        .section .init_array, "aw"
        .balign 4
        .word SystemCoreClockUpdate
# 234 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC1700_Startup.s"
        .weak HardFault_Handler
        .section .init.HardFault_Handler, "ax"
        .balign 2
        .thumb_func
HardFault_Handler:



        ldr R1, =0xE000ED2C
        ldr R2, [R1]
        cmp R2, #0

.LHardFault_Handler_hfLoop:
        bmi .LHardFault_Handler_hfLoop
# 262 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC1700_Startup.s"
        tst LR, #4
        ite eq
        mrseq R0, MSP
        mrsne R0, PSP
# 276 "C:\\Projects\\Ananke\\Firmware\\Tablet_FreeRTOS\\Tablet\\LPC1700\\Source\\LPC1700_Startup.s"
        orr R2, R2, #0x80000000
        str R2, [R1]




        ldr R1, [R0, #24]
        adds R1, #2
        str R1, [R0, #24]

        bx LR
