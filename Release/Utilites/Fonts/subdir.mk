################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilites/Fonts/font12.c \
../Utilites/Fonts/font16.c \
../Utilites/Fonts/font20.c \
../Utilites/Fonts/font24.c \
../Utilites/Fonts/font8.c 

OBJS += \
./Utilites/Fonts/font12.o \
./Utilites/Fonts/font16.o \
./Utilites/Fonts/font20.o \
./Utilites/Fonts/font24.o \
./Utilites/Fonts/font8.o 

C_DEPS += \
./Utilites/Fonts/font12.d \
./Utilites/Fonts/font16.d \
./Utilites/Fonts/font20.d \
./Utilites/Fonts/font24.d \
./Utilites/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Utilites/Fonts/%.o Utilites/Fonts/%.su: ../Utilites/Fonts/%.c Utilites/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/BSP/Inc -I../MK85Core/Inc -I../MK85Core/Port/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilites-2f-Fonts

clean-Utilites-2f-Fonts:
	-$(RM) ./Utilites/Fonts/font12.d ./Utilites/Fonts/font12.o ./Utilites/Fonts/font12.su ./Utilites/Fonts/font16.d ./Utilites/Fonts/font16.o ./Utilites/Fonts/font16.su ./Utilites/Fonts/font20.d ./Utilites/Fonts/font20.o ./Utilites/Fonts/font20.su ./Utilites/Fonts/font24.d ./Utilites/Fonts/font24.o ./Utilites/Fonts/font24.su ./Utilites/Fonts/font8.d ./Utilites/Fonts/font8.o ./Utilites/Fonts/font8.su

.PHONY: clean-Utilites-2f-Fonts

