################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MK85Core/Src/app.c \
../MK85Core/Src/app_lcd.c \
../MK85Core/Src/bitmap.c \
../MK85Core/Src/colors.c \
../MK85Core/Src/display.c \
../MK85Core/Src/geometry.c \
../MK85Core/Src/mk85_ctrl.c \
../MK85Core/Src/mk85_kbd.c \
../MK85Core/Src/mk85_lcd.c \
../MK85Core/Src/mk85_mb.c \
../MK85Core/Src/pdp11_adr.c \
../MK85Core/Src/pdp11_bus.c \
../MK85Core/Src/pdp11_cmds.c \
../MK85Core/Src/pdp11_cpu.c \
../MK85Core/Src/touch_kbd.c \
../MK85Core/Src/video.c 

OBJS += \
./MK85Core/Src/app.o \
./MK85Core/Src/app_lcd.o \
./MK85Core/Src/bitmap.o \
./MK85Core/Src/colors.o \
./MK85Core/Src/display.o \
./MK85Core/Src/geometry.o \
./MK85Core/Src/mk85_ctrl.o \
./MK85Core/Src/mk85_kbd.o \
./MK85Core/Src/mk85_lcd.o \
./MK85Core/Src/mk85_mb.o \
./MK85Core/Src/pdp11_adr.o \
./MK85Core/Src/pdp11_bus.o \
./MK85Core/Src/pdp11_cmds.o \
./MK85Core/Src/pdp11_cpu.o \
./MK85Core/Src/touch_kbd.o \
./MK85Core/Src/video.o 

C_DEPS += \
./MK85Core/Src/app.d \
./MK85Core/Src/app_lcd.d \
./MK85Core/Src/bitmap.d \
./MK85Core/Src/colors.d \
./MK85Core/Src/display.d \
./MK85Core/Src/geometry.d \
./MK85Core/Src/mk85_ctrl.d \
./MK85Core/Src/mk85_kbd.d \
./MK85Core/Src/mk85_lcd.d \
./MK85Core/Src/mk85_mb.d \
./MK85Core/Src/pdp11_adr.d \
./MK85Core/Src/pdp11_bus.d \
./MK85Core/Src/pdp11_cmds.d \
./MK85Core/Src/pdp11_cpu.d \
./MK85Core/Src/touch_kbd.d \
./MK85Core/Src/video.d 


# Each subdirectory must supply rules for building sources it contributes
MK85Core/Src/%.o MK85Core/Src/%.su: ../MK85Core/Src/%.c MK85Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/BSP/Inc -I../MK85Core/Inc -I../MK85Core/Port/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MK85Core-2f-Src

clean-MK85Core-2f-Src:
	-$(RM) ./MK85Core/Src/app.d ./MK85Core/Src/app.o ./MK85Core/Src/app.su ./MK85Core/Src/app_lcd.d ./MK85Core/Src/app_lcd.o ./MK85Core/Src/app_lcd.su ./MK85Core/Src/bitmap.d ./MK85Core/Src/bitmap.o ./MK85Core/Src/bitmap.su ./MK85Core/Src/colors.d ./MK85Core/Src/colors.o ./MK85Core/Src/colors.su ./MK85Core/Src/display.d ./MK85Core/Src/display.o ./MK85Core/Src/display.su ./MK85Core/Src/geometry.d ./MK85Core/Src/geometry.o ./MK85Core/Src/geometry.su ./MK85Core/Src/mk85_ctrl.d ./MK85Core/Src/mk85_ctrl.o ./MK85Core/Src/mk85_ctrl.su ./MK85Core/Src/mk85_kbd.d ./MK85Core/Src/mk85_kbd.o ./MK85Core/Src/mk85_kbd.su ./MK85Core/Src/mk85_lcd.d ./MK85Core/Src/mk85_lcd.o ./MK85Core/Src/mk85_lcd.su ./MK85Core/Src/mk85_mb.d ./MK85Core/Src/mk85_mb.o ./MK85Core/Src/mk85_mb.su ./MK85Core/Src/pdp11_adr.d ./MK85Core/Src/pdp11_adr.o ./MK85Core/Src/pdp11_adr.su ./MK85Core/Src/pdp11_bus.d ./MK85Core/Src/pdp11_bus.o ./MK85Core/Src/pdp11_bus.su ./MK85Core/Src/pdp11_cmds.d ./MK85Core/Src/pdp11_cmds.o ./MK85Core/Src/pdp11_cmds.su ./MK85Core/Src/pdp11_cpu.d ./MK85Core/Src/pdp11_cpu.o ./MK85Core/Src/pdp11_cpu.su ./MK85Core/Src/touch_kbd.d ./MK85Core/Src/touch_kbd.o ./MK85Core/Src/touch_kbd.su ./MK85Core/Src/video.d ./MK85Core/Src/video.o ./MK85Core/Src/video.su

.PHONY: clean-MK85Core-2f-Src

