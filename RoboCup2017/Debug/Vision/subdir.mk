################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Vision/BallDetector.cpp \
../Vision/VisionThread.cpp 

OBJS += \
./Vision/BallDetector.o \
./Vision/VisionThread.o 

CPP_DEPS += \
./Vision/BallDetector.d \
./Vision/VisionThread.d 


# Each subdirectory must supply rules for building sources it contributes
Vision/%.o: ../Vision/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I/usr/local/include -I/usr/local/include/opencv -I/home/robot/HROS5-Framework/Linux/include -I/home/robot/HROS5-Framework/Framework/include -I/home/robot/src/RobocupBIU/RoboCup2017/Include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


