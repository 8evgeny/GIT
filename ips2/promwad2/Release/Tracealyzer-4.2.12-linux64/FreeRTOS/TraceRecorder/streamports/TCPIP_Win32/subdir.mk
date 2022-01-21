################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/TCPIP_Win32/trcStreamingPort.c 

OBJS += \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/TCPIP_Win32/trcStreamingPort.o 

C_DEPS += \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/TCPIP_Win32/trcStreamingPort.d 


# Each subdirectory must supply rules for building sources it contributes
Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/TCPIP_Win32/%.o: ../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/TCPIP_Win32/%.c Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/TCPIP_Win32/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


