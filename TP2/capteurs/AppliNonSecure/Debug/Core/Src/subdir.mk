################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/hts221_read_data_polling.c \
../Core/Src/hts221_reg.c \
../Core/Src/lis2dw12_read_data_polling.c \
../Core/Src/lis2dw12_reg.c \
../Core/Src/lis2mdl_read_data_polling.c \
../Core/Src/lis2mdl_reg.c \
../Core/Src/lps22hh_read_data_polling.c \
../Core/Src/lps22hh_reg.c \
../Core/Src/lsm6dso_read_data_polling.c \
../Core/Src/lsm6dso_reg.c \
../Core/Src/main.c \
../Core/Src/stm32n6xx_hal_msp.c \
../Core/Src/stm32n6xx_it.c \
../Core/Src/stts751_read_data_polling.c \
../Core/Src/stts751_reg.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32n6xx_ns.c 

OBJS += \
./Core/Src/hts221_read_data_polling.o \
./Core/Src/hts221_reg.o \
./Core/Src/lis2dw12_read_data_polling.o \
./Core/Src/lis2dw12_reg.o \
./Core/Src/lis2mdl_read_data_polling.o \
./Core/Src/lis2mdl_reg.o \
./Core/Src/lps22hh_read_data_polling.o \
./Core/Src/lps22hh_reg.o \
./Core/Src/lsm6dso_read_data_polling.o \
./Core/Src/lsm6dso_reg.o \
./Core/Src/main.o \
./Core/Src/stm32n6xx_hal_msp.o \
./Core/Src/stm32n6xx_it.o \
./Core/Src/stts751_read_data_polling.o \
./Core/Src/stts751_reg.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32n6xx_ns.o 

C_DEPS += \
./Core/Src/hts221_read_data_polling.d \
./Core/Src/hts221_reg.d \
./Core/Src/lis2dw12_read_data_polling.d \
./Core/Src/lis2dw12_reg.d \
./Core/Src/lis2mdl_read_data_polling.d \
./Core/Src/lis2mdl_reg.d \
./Core/Src/lps22hh_read_data_polling.d \
./Core/Src/lps22hh_reg.d \
./Core/Src/lsm6dso_read_data_polling.d \
./Core/Src/lsm6dso_reg.d \
./Core/Src/main.d \
./Core/Src/stm32n6xx_hal_msp.d \
./Core/Src/stm32n6xx_it.d \
./Core/Src/stts751_read_data_polling.d \
./Core/Src/stts751_reg.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32n6xx_ns.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m55 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32N657xx -c -I../Core/Inc -I../../Secure_nsclib -I"/home/benjaminavocat-maulaz/Documents/Programs_STM32/N657/capteurs/AppliNonSecure/Drivers/STM32N6xx_HAL_Driver" -I../../Drivers/STM32N6xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32N6xx/Include -I../../Drivers/STM32N6xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/hts221_read_data_polling.cyclo ./Core/Src/hts221_read_data_polling.d ./Core/Src/hts221_read_data_polling.o ./Core/Src/hts221_read_data_polling.su ./Core/Src/hts221_reg.cyclo ./Core/Src/hts221_reg.d ./Core/Src/hts221_reg.o ./Core/Src/hts221_reg.su ./Core/Src/lis2dw12_read_data_polling.cyclo ./Core/Src/lis2dw12_read_data_polling.d ./Core/Src/lis2dw12_read_data_polling.o ./Core/Src/lis2dw12_read_data_polling.su ./Core/Src/lis2dw12_reg.cyclo ./Core/Src/lis2dw12_reg.d ./Core/Src/lis2dw12_reg.o ./Core/Src/lis2dw12_reg.su ./Core/Src/lis2mdl_read_data_polling.cyclo ./Core/Src/lis2mdl_read_data_polling.d ./Core/Src/lis2mdl_read_data_polling.o ./Core/Src/lis2mdl_read_data_polling.su ./Core/Src/lis2mdl_reg.cyclo ./Core/Src/lis2mdl_reg.d ./Core/Src/lis2mdl_reg.o ./Core/Src/lis2mdl_reg.su ./Core/Src/lps22hh_read_data_polling.cyclo ./Core/Src/lps22hh_read_data_polling.d ./Core/Src/lps22hh_read_data_polling.o ./Core/Src/lps22hh_read_data_polling.su ./Core/Src/lps22hh_reg.cyclo ./Core/Src/lps22hh_reg.d ./Core/Src/lps22hh_reg.o ./Core/Src/lps22hh_reg.su ./Core/Src/lsm6dso_read_data_polling.cyclo ./Core/Src/lsm6dso_read_data_polling.d ./Core/Src/lsm6dso_read_data_polling.o ./Core/Src/lsm6dso_read_data_polling.su ./Core/Src/lsm6dso_reg.cyclo ./Core/Src/lsm6dso_reg.d ./Core/Src/lsm6dso_reg.o ./Core/Src/lsm6dso_reg.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32n6xx_hal_msp.cyclo ./Core/Src/stm32n6xx_hal_msp.d ./Core/Src/stm32n6xx_hal_msp.o ./Core/Src/stm32n6xx_hal_msp.su ./Core/Src/stm32n6xx_it.cyclo ./Core/Src/stm32n6xx_it.d ./Core/Src/stm32n6xx_it.o ./Core/Src/stm32n6xx_it.su ./Core/Src/stts751_read_data_polling.cyclo ./Core/Src/stts751_read_data_polling.d ./Core/Src/stts751_read_data_polling.o ./Core/Src/stts751_read_data_polling.su ./Core/Src/stts751_reg.cyclo ./Core/Src/stts751_reg.d ./Core/Src/stts751_reg.o ./Core/Src/stts751_reg.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32n6xx_ns.cyclo ./Core/Src/system_stm32n6xx_ns.d ./Core/Src/system_stm32n6xx_ns.o ./Core/Src/system_stm32n6xx_ns.su

.PHONY: clean-Core-2f-Src

