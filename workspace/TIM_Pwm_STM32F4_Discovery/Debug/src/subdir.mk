################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/functions.c \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/functions.o \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/functions.d \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F40XX -DSTM32F40_41xxx -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/inc" -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/CMSIS/core" -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/CMSIS/device" -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/StdPeriph_Driver/inc" -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/Utilities" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


