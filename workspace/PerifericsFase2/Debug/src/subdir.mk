################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/clock.c \
../src/delay.c \
../src/mainF2.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/tm_stm32f4_disco.c \
../src/tm_stm32f4_gpio.c 

OBJS += \
./src/clock.o \
./src/delay.o \
./src/mainF2.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/tm_stm32f4_disco.o \
./src/tm_stm32f4_gpio.o 

C_DEPS += \
./src/clock.d \
./src/delay.d \
./src/mainF2.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/tm_stm32f4_disco.d \
./src/tm_stm32f4_gpio.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F429ZITx -DSTM32F429I_DISC1 -DDEBUG -DSTM32F429_439xx -DUSE_STDPERIPH_DRIVER -I"C:/Users/angel/Desktop/3er/Periferics/Practica/workspace/PerifericsFase2/StdPeriph_Driver/inc" -I"C:/Users/angel/Desktop/3er/Periferics/Practica/workspace/PerifericsFase2/inc" -I"C:/Users/angel/Desktop/3er/Periferics/Practica/workspace/PerifericsFase2/CMSIS/device" -I"C:/Users/angel/Desktop/3er/Periferics/Practica/workspace/PerifericsFase2/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


