################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/cr_startup_lpc176x.c \
../Source/main.c \
../Source/printf-stdarg.c 

OBJS += \
./Source/cr_startup_lpc176x.o \
./Source/main.o \
./Source/printf-stdarg.o 

C_DEPS += \
./Source/cr_startup_lpc176x.d \
./Source/main.d \
./Source/printf-stdarg.d 


# Each subdirectory must supply rules for building sources it contributes
Source/cr_startup_lpc176x.o: ../Source/cr_startup_lpc176x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS\include" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\CMSISv2p00_LPC17xx\inc" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Plus-Demo-1\Source\Examples\Include" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Plus-Demo-1\Source" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\lpc17xx.cmsis.driver.library\Include" -O0 -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"Source/cr_startup_lpc176x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/%.o: ../Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Products\FreeRTOS\include" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\CMSISv2p00_LPC17xx\inc" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Plus-Demo-1\Source\Examples\Include" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\FreeRTOS-Plus-Demo-1\Source" -I"C:\Projects\Ananke\Firmware\Tablet_FreeRTOS\lpc17xx.cmsis.driver.library\Include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


