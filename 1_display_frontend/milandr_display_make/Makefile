CXX		= arm-none-eabi-g++
CC		= arm-none-eabi-gcc
LD		= arm-none-eabi-g++
OBJCOPY		= arm-none-eabi-objcopy
SIZE		= arm-none-eabi-size
STFLASH		= st-flash
OPENOCD		= openocd
STUTIL		= st-flash

OUTPUT_DIR	= bin
OBJ_DIR         = obj

TARGET_NAME     = display

TARGET_BINARY	= $(TARGET_NAME).elf
TARGET_HEX	= $(TARGET_NAME).hex
TARGET_BIN	= $(TARGET_NAME).bin

vpath %.c ./src
vpath %.cpp ./src

SOURCES_C	= \
gpio.c \
logic.c \
mt20s4.c \
rcc_delay.c \
usb.c \
var.c \
#main.c \

SOURCES_CPP	= \
main.cpp \

LDFLAGS_DEBUG =  
CDFLAGS_DEBUG = -g3 -O0

LDFLAGS_RELEASE = 
CDFLAGS_RELEASE = -O2

CFLAGS = \
    -Wall \
    -mcpu=cortex-m3 \
    -mthumb \
    -D__HEAP_SIZE=0x0000 \
    -D__STACK_SIZE=0x0100 \
    -mfloat-abi=soft \
    -fno-strict-aliasing \
    -fdata-sections \
    -ffunction-sections \
#    -DSTM32F103C8 \
#    -DSTM32F10X_MD \
#    -DUSE_STDPERIPH_DRIVER \
#-xc \
#-std=c99 \
#--target=arm-arm-none-eabi \
#-mcpu=cortex-m3 \
#-c \
#-fno-rtti \
#-funsigned-char \
#-fshort-enums \
#-fshort-wchar \

TARGET	= $(OUTPUT_DIR)/$(TARGET_BINARY)

LIBS = -lm

CXXFLAGS = -O3 -o prog -rdynamic -D_GNU_SOURCE -L./libmine
#LIBS += -lpthread -ldl

LDFLAGS	= \
--specs=nosys.specs \
--specs=nano.specs \
-mcpu=cortex-m3 \
-mthumb \
-Wl,--defsym=__HEAP_SIZE=0x0000 \
-Wl,--defsym=__STACK_SIZE=0x0100 \
-mfloat-abi=soft \
-fno-strict-aliasing \
-T ../Milandr_MCU_1986x_Standard_Peripherals_Library/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/MDR32F9Qx.ld \
-fdata-sections \
-ffunction-sections  \
-Wl,--gc-sections \
-Wl,-Map=$(TARGET).map \
# -Wl,-script="../Milandr_MCU_1986x_Standard_Peripherals_Library/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/MDR32F9Qx.ld" \
#-u _printf_float 


INCLUDES = \
-I../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/inc \
-I../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/startup/arm \
-I../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/CoreSupport/CM3 \
-I../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx \
-I../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/inc \
-I../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/inc/USB_Library \
-I../MDR1986BExx/2.0.3/Libraries/MDR32Fx/inc \
-I../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src \
-I. \
-I./inc

STARUPFILE = ../Milandr_MCU_1986x_Standard_Peripherals_Library/CMSIS/CM3/DeviceSupport/MDR32F9Qx/startup/gcc/startup_MDR32F9Qx.S
DEV_SUPPORT_FILE =  ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/startup/arm/system_MDR32F9Qx.c

SOURCES_SPL = \
MDR32F9Qx_iwdg.c \
MDR32F9Qx_port.c \
MDR32F9Qx_rst_clk.c \
MDR32F9Qx_timer.c \
MDR32F9Qx_usb.c

SOURCES_USB = \
MDR32F9Qx_usb_CDC.c \
MDR32F9Qx_usb_device.c

SOURCES_DEVICE_SUPPORT = \
system_MDR32F9Qx.c

################################################################

SOURCES_C += $(addprefix ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/, $(SOURCES_SPL))
SOURCES_C += $(addprefix ../MDR1986BExx/2.0.3/Libraries/CMSIS/MDR32Fx/DeviceSupport/MDR1986VE9x/startup/arm/, $(SOURCES_DEVICE_SUPPORT))
SOURCES_C += $(addprefix ../MDR1986BExx/2.0.3/Libraries/SPL/MDR32Fx/src/USB_Library/, $(SOURCES_USB))

HEX = $(OUTPUT_DIR)/$(TARGET_HEX)

STARTUPOBJ = $(notdir $(STARUPFILE))
STARTUPOBJ := $(patsubst %.S, $(OBJ_DIR)/%.S.o, $(STARTUPOBJ))

OBJECTS	+= $(patsubst %, $(OBJ_DIR)/%.o, $(SOURCES_C))
OBJECTS	+= $(patsubst %, $(OBJ_DIR)/%.o, $(SOURCES_CPP))
OBJECTS += $(STARTUPOBJ)

DEPS = $(OBJECTS:.o=.d)

all: release

release: LDFLAGS+=$(LDFLAGS_RELEASE)
release: CFLAGS+=$(CDFLAGS_RELEASE)
release: _firmware

debug: LDFLAGS+=$(LDFLAGS_DEBUG)
debug: CFLAGS+=$(CDFLAGS_DEBUG)
debug: _firmware

_firmware: $(TARGET) $(HEX)

$(TARGET): $(OBJECTS) | $(OUTPUT_DIR) $(OBJ_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)
	$(SIZE) --format=berkeley $(TARGET) 

obj/%.cpp.o: %.cpp
#ifeq ($(OS), Windows_NT)
#	$(eval dirname=$(subst /,\\,$(dir $@)))
#	@ if NOT EXIST $(dirname) mkdir $(dirname)
#else
	@mkdir -p $(dir $@) $(TO_NULL) 
#endif
	$(CXX) -c -MMD -MP $(CFLAGS) $(INCLUDES) $< -o $@

obj/%.c.o: %.c
#ifeq ($(OS), Windows_NT)
#	$(eval dirname=$(subst /,\\,$(dir $@)))
#	@ if NOT EXIST $(dirname) mkdir $(dirname)
#else
	@mkdir -p $(dir $@) $(TO_NULL) 
#endif
	$(CC) -c -MMD -MP $(CFLAGS) $(INCLUDES) $< -o $@

$(STARTUPOBJ):
	$(CC) -c -MMD -MP $(CFLAGS) $(INCLUDES) $(STARUPFILE) -o $(STARTUPOBJ)

$(HEX): $(TARGET)
	$(OBJCOPY) -O ihex $(TARGET) $(HEX)
	
$(OUTPUT_DIR):
	@mkdir $(OUTPUT_DIR)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

_size: $(TARGET)
	$(SIZE) --format=berkeley $(TARGET) 

flash: flash-ocd

flash-stutil:
	$(STFLASH) --format ihex write $(HEX)

flash-ocd:
	$(OPENOCD) -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "program $(TARGET) verify reset exit"

srclist:
	@echo $(SOURCES)

objlist:
	@echo $(OBJECTS)

clean:

ifeq ($(OS), Windows_NT)
	-del $(subst /,\\,$(OBJECTS))
	-del $(subst /,\\,$(DEPS))
	-rmdir /S /Q $(OBJ_DIR)
	-del /Q /F $(OUTPUT_DIR)\\$(TARGET_BINARY)
	-del /Q /F $(OUTPUT_DIR)\\$(TARGET_BINARY).map
	-del /Q /F $(OUTPUT_DIR)\\$(TARGET_HEX)
else
	-rm -f $(OBJECTS)
	-rm -f $(DEPS)
	-rm -rf $(OBJ_DIR)
	-rm -f $(OUTPUT_DIR)/$(TARGET_BINARY)
	-rm -f $(OUTPUT_DIR)/$(TARGET_BINARY).map
	-rm -f $(OUTPUT_DIR)/$(TARGET_HEX)
endif	

-include $(DEPS)
