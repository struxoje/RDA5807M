################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/ben/Development/RDA5807M/main.cpp 

OBJS += \
./RDA5807M/main.o 

CPP_DEPS += \
./RDA5807M/main.d 


# Each subdirectory must supply rules for building sources it contributes
RDA5807M/main.o: /home/ben/Development/RDA5807M/main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	clang++ -std=c++0x -I/usr/local/include/ -I/usr/local/include/mraa -I"/home/ben/Development/RDA5807M" -I"/home/ben/Development/RDA5807M/driver_wrapper" -I"/home/ben/Development/RDA5807M/driver" -I"/home/ben/Development/RDA5807M/util" -I"/home/ben/Development/RDA5807M/command" -mno-sse -mno-mmx -march=i586 -O0 -g3 -Wall -Wextra -Wconversion -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


