################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../daemon/arg_parser/ArgParser.cpp \
../daemon/arg_parser/ArgParserFunction.cpp 

OBJS += \
./daemon/arg_parser/ArgParser.o \
./daemon/arg_parser/ArgParserFunction.o 

CPP_DEPS += \
./daemon/arg_parser/ArgParser.d \
./daemon/arg_parser/ArgParserFunction.d 


# Each subdirectory must supply rules for building sources it contributes
daemon/arg_parser/%.o: ../daemon/arg_parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -I../util -I../daemon -I../network -I../driver_wrapper -I../command -I../driver -I../daemon/arg_parser -O3 -g -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


