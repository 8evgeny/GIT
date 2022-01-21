################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.c \
../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.c 

OBJS += \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.o \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.o 

C_DEPS += \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/SEGGER_RTT.d \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/trcStreamingPort.d 


# Each subdirectory must supply rules for building sources it contributes
Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/%.o: ../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/%.c Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/Jlink_RTT/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


