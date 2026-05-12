################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../osal/os_lyh_core.c 

C_DEPS += \
./osal/os_lyh_core.d 

OBJS += \
./osal/os_lyh_core.o 


# Each subdirectory must supply rules for building sources it contributes
osal/%.o: ../osal/%.c osal/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GD ARM MCU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu11 -DGD32F30X_HD -DGD_ECLIPSE_GCC -DUSE_STDPERIPH_DRIVER -I"../Firmware/CMSIS/" -I"../Firmware/CMSIS/GD/GD32F30x/Include/" -I"../Firmware/GD32F30x_standard_peripheral/Include/" -I"../app/" -I"../osal/" -I"../inc/" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -Wa,-adhlns=$@.lst   -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-osal

clean-osal:
	-$(RM) ./osal/os_lyh_core.d ./osal/os_lyh_core.o

.PHONY: clean-osal

