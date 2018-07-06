################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../daemon/rda5807.cpp 

OBJS += \
./daemon/rda5807.o 

CPP_DEPS += \
./daemon/rda5807.d 


# Each subdirectory must supply rules for building sources it contributes
daemon/%.o: ../daemon/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -I../util -I../daemon -I../network -I../driver_wrapper -I../command -I../driver -I../daemon/arg_parser -O3 -g -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


