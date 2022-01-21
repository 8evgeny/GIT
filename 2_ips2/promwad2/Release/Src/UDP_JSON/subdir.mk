################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/UDP_JSON/udp_multicast.cpp \
../Src/UDP_JSON/udp_multicast_firmware.cpp 

OBJS += \
./Src/UDP_JSON/udp_multicast.o \
./Src/UDP_JSON/udp_multicast_firmware.o 

CPP_DEPS += \
./Src/UDP_JSON/udp_multicast.d \
./Src/UDP_JSON/udp_multicast_firmware.d 


# Each subdirectory must supply rules for building sources it contributes
Src/UDP_JSON/%.o: ../Src/UDP_JSON/%.cpp Src/UDP_JSON/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


