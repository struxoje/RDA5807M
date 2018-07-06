################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../network/SocketWrapper.cpp \
../network/TcpServer.cpp 

OBJS += \
./network/SocketWrapper.o \
./network/TcpServer.o 

CPP_DEPS += \
./network/SocketWrapper.d \
./network/TcpServer.d 


# Each subdirectory must supply rules for building sources it contributes
network/%.o: ../network/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++14 -I../util -I../daemon -I../network -I../driver_wrapper -I../command -I../driver -I../daemon/arg_parser -O3 -g -Wall -Wextra -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


