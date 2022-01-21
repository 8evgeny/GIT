################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcKernelPort.c \
../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcSnapshotRecorder.c \
../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcStreamingRecorder.c 

OBJS += \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcKernelPort.o \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcSnapshotRecorder.o \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcStreamingRecorder.o 

C_DEPS += \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcKernelPort.d \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcSnapshotRecorder.d \
./Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/%.o: ../Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/%.c Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


