################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f4xx.S 

OBJS += \
./startup/startup_stm32f4xx.o 

S_UPPER_DEPS += \
./startup/startup_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F40XX -DSTM32F40_41xxx -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/inc" -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/CMSIS/core" -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/CMSIS/device" -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/StdPeriph_Driver/inc" -I"C:/Users/Admin/SW4STM32/workspace/TIM_Pwm_STM32F4_Discovery/Utilities" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


