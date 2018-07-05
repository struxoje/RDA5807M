################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../driver_wrapper/RDA5807MWrapper.cpp 

OBJS += \
./driver_wrapper/RDA5807MWrapper.o 

CPP_DEPS += \
./driver_wrapper/RDA5807MWrapper.d 


# Each subdirectory must supply rules for building sources it contributes
driver_wrapper/%.o: ../driver_wrapper/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -I../util -I../driver_wrapper -I../command -I../driver -O3 -g -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


