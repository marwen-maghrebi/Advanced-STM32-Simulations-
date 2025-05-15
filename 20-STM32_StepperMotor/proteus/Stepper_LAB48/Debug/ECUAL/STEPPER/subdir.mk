################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ECUAL/STEPPER/STEPPER.c \
../ECUAL/STEPPER/STEPPER_cfg.c 

OBJS += \
./ECUAL/STEPPER/STEPPER.o \
./ECUAL/STEPPER/STEPPER_cfg.o 

C_DEPS += \
./ECUAL/STEPPER/STEPPER.d \
./ECUAL/STEPPER/STEPPER_cfg.d 


# Each subdirectory must supply rules for building sources it contributes
ECUAL/STEPPER/STEPPER.o: ../ECUAL/STEPPER/STEPPER.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L432xx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ECUAL/STEPPER/STEPPER.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
ECUAL/STEPPER/STEPPER_cfg.o: ../ECUAL/STEPPER/STEPPER_cfg.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L432xx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ECUAL/STEPPER/STEPPER_cfg.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

