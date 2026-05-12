################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/acmotor.c \
../app/adc.c \
../app/app.c \
../app/bus.c \
../app/dcmotor.c \
../app/display.c \
../app/extint.c \
../app/gpio.c \
../app/keyboard.c \
../app/timer.c \
../app/usart.c 

C_DEPS += \
./app/acmotor.d \
./app/adc.d \
./app/app.d \
./app/bus.d \
./app/dcmotor.d \
./app/display.d \
./app/extint.d \
./app/gpio.d \
./app/keyboard.d \
./app/timer.d \
./app/usart.d 

OBJS += \
./app/acmotor.o \
./app/adc.o \
./app/app.o \
./app/bus.o \
./app/dcmotor.o \
./app/display.o \
./app/extint.o \
./app/gpio.o \
./app/keyboard.o \
./app/timer.o \
./app/usart.o 


# Each subdirectory must supply rules for building sources it contributes
app/%.o: ../app/%.c app/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GD ARM MCU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu11 -DGD32F30X_HD -DGD_ECLIPSE_GCC -DUSE_STDPERIPH_DRIVER -I"../Firmware/CMSIS/" -I"../Firmware/CMSIS/GD/GD32F30x/Include/" -I"../Firmware/GD32F30x_standard_peripheral/Include/" -I"../app/" -I"../osal/" -I"../inc/" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -Wa,-adhlns=$@.lst   -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-app

clean-app:
	-$(RM) ./app/acmotor.d ./app/acmotor.o ./app/adc.d ./app/adc.o ./app/app.d ./app/app.o ./app/bus.d ./app/bus.o ./app/dcmotor.d ./app/dcmotor.o ./app/display.d ./app/display.o ./app/extint.d ./app/extint.o ./app/gpio.d ./app/gpio.o ./app/keyboard.d ./app/keyboard.o ./app/timer.d ./app/timer.o ./app/usart.d ./app/usart.o

.PHONY: clean-app

