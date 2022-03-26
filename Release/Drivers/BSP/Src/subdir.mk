################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Src/stm32f429i_discovery.c \
../Drivers/BSP/Src/stm32f429i_discovery_io.c \
../Drivers/BSP/Src/stm32f429i_discovery_lcd.c \
../Drivers/BSP/Src/stm32f429i_discovery_sdram.c \
../Drivers/BSP/Src/stm32f429i_discovery_ts.c 

OBJS += \
./Drivers/BSP/Src/stm32f429i_discovery.o \
./Drivers/BSP/Src/stm32f429i_discovery_io.o \
./Drivers/BSP/Src/stm32f429i_discovery_lcd.o \
./Drivers/BSP/Src/stm32f429i_discovery_sdram.o \
./Drivers/BSP/Src/stm32f429i_discovery_ts.o 

C_DEPS += \
./Drivers/BSP/Src/stm32f429i_discovery.d \
./Drivers/BSP/Src/stm32f429i_discovery_io.d \
./Drivers/BSP/Src/stm32f429i_discovery_lcd.d \
./Drivers/BSP/Src/stm32f429i_discovery_sdram.d \
./Drivers/BSP/Src/stm32f429i_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Src/%.o Drivers/BSP/Src/%.su: ../Drivers/BSP/Src/%.c Drivers/BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/BSP/Inc -I../MK85Core/Inc -I../MK85Core/Port/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Src

clean-Drivers-2f-BSP-2f-Src:
	-$(RM) ./Drivers/BSP/Src/stm32f429i_discovery.d ./Drivers/BSP/Src/stm32f429i_discovery.o ./Drivers/BSP/Src/stm32f429i_discovery.su ./Drivers/BSP/Src/stm32f429i_discovery_io.d ./Drivers/BSP/Src/stm32f429i_discovery_io.o ./Drivers/BSP/Src/stm32f429i_discovery_io.su ./Drivers/BSP/Src/stm32f429i_discovery_lcd.d ./Drivers/BSP/Src/stm32f429i_discovery_lcd.o ./Drivers/BSP/Src/stm32f429i_discovery_lcd.su ./Drivers/BSP/Src/stm32f429i_discovery_sdram.d ./Drivers/BSP/Src/stm32f429i_discovery_sdram.o ./Drivers/BSP/Src/stm32f429i_discovery_sdram.su ./Drivers/BSP/Src/stm32f429i_discovery_ts.d ./Drivers/BSP/Src/stm32f429i_discovery_ts.o ./Drivers/BSP/Src/stm32f429i_discovery_ts.su

.PHONY: clean-Drivers-2f-BSP-2f-Src

