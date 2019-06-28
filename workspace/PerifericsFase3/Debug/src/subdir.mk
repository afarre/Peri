################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mainF3.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/mainF3.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/mainF3.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F429ZITx -DSTM32F429I_DISC1 -DDEBUG -DSTM32F429_439xx -DUSE_STDPERIPH_DRIVER -I"C:/Users/angel/Desktop/3er/Periferics/Practica/workspace/PerifericsFase3/StdPeriph_Driver/inc" -I"C:/Users/angel/Desktop/3er/Periferics/Practica/workspace/PerifericsFase3/inc" -I"C:/Users/angel/Desktop/3er/Periferics/Practica/workspace/PerifericsFase3/CMSIS/device" -I"C:/Users/angel/Desktop/3er/Periferics/Practica/workspace/PerifericsFase3/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


