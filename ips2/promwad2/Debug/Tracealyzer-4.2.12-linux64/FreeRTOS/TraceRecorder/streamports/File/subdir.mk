################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/File/trcStreamingPort.c 

OBJS += \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/File/trcStreamingPort.o 

C_DEPS += \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/File/trcStreamingPort.d 


# Each subdirectory must supply rules for building sources it contributes
Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/File/%.o: ../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/File/%.c Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/streamports/File/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


