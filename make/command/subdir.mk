################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../command/CommandParser.cpp 

OBJS += \
./command/CommandParser.o 

CPP_DEPS += \
./command/CommandParser.d 


# Each subdirectory must supply rules for building sources it contributes
command/%.o: ../command/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -I../util -I../driver_wrapper -I../command -I../driver -O3 -g -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


