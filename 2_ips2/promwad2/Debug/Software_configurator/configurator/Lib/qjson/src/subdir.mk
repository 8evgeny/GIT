################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Software_configurator/configurator/Lib/qjson/src/json_parser.cc \
../Software_configurator/configurator/Lib/qjson/src/json_scanner.cc 

CPP_SRCS += \
../Software_configurator/configurator/Lib/qjson/src/cmdline_tester.cpp \
../Software_configurator/configurator/Lib/qjson/src/cmdlineparser.cpp \
../Software_configurator/configurator/Lib/qjson/src/json_scanner.cpp \
../Software_configurator/configurator/Lib/qjson/src/parser.cpp \
../Software_configurator/configurator/Lib/qjson/src/parserrunnable.cpp \
../Software_configurator/configurator/Lib/qjson/src/qobjecthelper.cpp \
../Software_configurator/configurator/Lib/qjson/src/serializer.cpp \
../Software_configurator/configurator/Lib/qjson/src/serializerrunnable.cpp 

CC_DEPS += \
./Software_configurator/configurator/Lib/qjson/src/json_parser.d \
./Software_configurator/configurator/Lib/qjson/src/json_scanner.d 

OBJS += \
./Software_configurator/configurator/Lib/qjson/src/cmdline_tester.o \
./Software_configurator/configurator/Lib/qjson/src/cmdlineparser.o \
./Software_configurator/configurator/Lib/qjson/src/json_parser.o \
./Software_configurator/configurator/Lib/qjson/src/json_scanner.o \
./Software_configurator/configurator/Lib/qjson/src/parser.o \
./Software_configurator/configurator/Lib/qjson/src/parserrunnable.o \
./Software_configurator/configurator/Lib/qjson/src/qobjecthelper.o \
./Software_configurator/configurator/Lib/qjson/src/serializer.o \
./Software_configurator/configurator/Lib/qjson/src/serializerrunnable.o 

CPP_DEPS += \
./Software_configurator/configurator/Lib/qjson/src/cmdline_tester.d \
./Software_configurator/configurator/Lib/qjson/src/cmdlineparser.d \
./Software_configurator/configurator/Lib/qjson/src/json_scanner.d \
./Software_configurator/configurator/Lib/qjson/src/parser.d \
./Software_configurator/configurator/Lib/qjson/src/parserrunnable.d \
./Software_configurator/configurator/Lib/qjson/src/qobjecthelper.d \
./Software_configurator/configurator/Lib/qjson/src/serializer.d \
./Software_configurator/configurator/Lib/qjson/src/serializerrunnable.d 


# Each subdirectory must supply rules for building sources it contributes
Software_configurator/configurator/Lib/qjson/src/%.o: ../Software_configurator/configurator/Lib/qjson/src/%.cpp Software_configurator/configurator/Lib/qjson/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Software_configurator/configurator/Lib/qjson/src/%.o: ../Software_configurator/configurator/Lib/qjson/src/%.cc Software_configurator/configurator/Lib/qjson/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


