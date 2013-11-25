################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/com.c \
../source/commands.c \
../source/data.c \
../source/main.c \
../source/net.c \
../source/sensors.c \
../source/system.c \
../source/time.c \
../source/uart.c \
../source/util.c 

OBJS += \
./source/com.o \
./source/commands.o \
./source/data.o \
./source/main.o \
./source/net.o \
./source/sensors.o \
./source/system.o \
./source/time.o \
./source/uart.o \
./source/util.o 

C_DEPS += \
./source/com.d \
./source/commands.d \
./source/data.d \
./source/main.d \
./source/net.d \
./source/sensors.d \
./source/system.d \
./source/time.d \
./source/uart.d \
./source/util.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/media/gabm/6ED257FAD257C4CD/Programming/Eclipse/Wetter/include" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=6144000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


