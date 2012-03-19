################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/com.c \
../source/main.c \
../source/sensors.c \
../source/system.c \
../source/time.c \
../source/uart.c \
../source/util.c 

OBJS += \
./source/com.o \
./source/main.o \
./source/sensors.o \
./source/system.o \
./source/time.o \
./source/uart.o \
./source/util.o 

C_DEPS += \
./source/com.d \
./source/main.d \
./source/sensors.d \
./source/system.d \
./source/time.d \
./source/uart.d \
./source/util.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/media/6ED257FAD257C4CD/Programming/Eclipse/Wetter/include" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=6144000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


