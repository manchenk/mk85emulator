################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Fonts/font12.c \
../Utilities/Fonts/font16.c \
../Utilities/Fonts/font20.c \
../Utilities/Fonts/font24.c \
../Utilities/Fonts/font8.c 

OBJS += \
./Utilities/Fonts/font12.o \
./Utilities/Fonts/font16.o \
./Utilities/Fonts/font20.o \
./Utilities/Fonts/font24.o \
./Utilities/Fonts/font8.o 

C_DEPS += \
./Utilities/Fonts/font12.d \
./Utilities/Fonts/font16.d \
./Utilities/Fonts/font20.d \
./Utilities/Fonts/font24.d \
./Utilities/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Fonts/%.o Utilities/Fonts/%.su: ../Utilities/Fonts/%.c Utilities/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/BSP/Inc -I../MK85Core/Inc -I../MK85Core/Port/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities-2f-Fonts

clean-Utilities-2f-Fonts:
	-$(RM) ./Utilities/Fonts/font12.d ./Utilities/Fonts/font12.o ./Utilities/Fonts/font12.su ./Utilities/Fonts/font16.d ./Utilities/Fonts/font16.o ./Utilities/Fonts/font16.su ./Utilities/Fonts/font20.d ./Utilities/Fonts/font20.o ./Utilities/Fonts/font20.su ./Utilities/Fonts/font24.d ./Utilities/Fonts/font24.o ./Utilities/Fonts/font24.su ./Utilities/Fonts/font8.d ./Utilities/Fonts/font8.o ./Utilities/Fonts/font8.su

.PHONY: clean-Utilities-2f-Fonts

