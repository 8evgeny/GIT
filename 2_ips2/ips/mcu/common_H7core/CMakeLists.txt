
cmake_minimum_required(VERSION 3.6)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

SET(TARGET_TRIPLET "arm-none-eabi")
SET(TOOLCHAIN_BIN_DIR "/usr/bin")


SET(CMAKE_C_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}")
SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOL_EXECUTABLE_SUFFIX}")

SET(CMAKE_ASM_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}")

SET(CMAKE_OBJCOPY "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objcopy${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "objcopy tool")
SET(CMAKE_OBJDUMP "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objdump${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "objdump tool")
SET(CMAKE_SIZE "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-size${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "size tool")
SET(CMAKE_DEBUGER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gdb${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "debuger")
SET(CMAKE_CPPFILT "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-c++filt${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "C++filt")


project(gdmx_display)

SET(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/STM32H753IITX_FLASH.ld")

#Add your own GCC compile flags (set CPU core type (Cortex M3), set CPU instruction set mode (Thumb2)
SET(GCC_COVERAGE_COMPILE_FLAGS "-mcpu=cortex-m4 -mthumb -Wl,-Map=${PROJECT_SOURCE_DIR}/objmapfile.map")

#Add your own linker flags
SET(GCC_COVERAGE_LINK_FLAGS    "-Wl,--gc-sections -T${LINKER_SCRIPT} -lm -lnosys -lgcc -lc -lstdc++")

#Override system C/C++ compiler flags
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

#Override system linker flags
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")


#add_compile_definitions(STM32F103xB)

include_directories ("${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include/")
include_directories ("${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include/")
include_directories ("${PROJECT_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc/")
include_directories ("${PROJECT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy/")
include_directories ("${PROJECT_SOURCE_DIR}/Drivers/BSP/Components/lan8742/")
include_directories ("${PROJECT_SOURCE_DIR}/Core/Inc/")
include_directories ("${PROJECT_SOURCE_DIR}/FATFS/App/")
include_directories ("${PROJECT_SOURCE_DIR}/FATFS/Target/")
include_directories ("${PROJECT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/")
include_directories ("${PROJECT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/")
include_directories ("${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/")
include_directories ("${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/")
include_directories ("${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include/")
include_directories ("${PROJECT_SOURCE_DIR}/Inc")



aux_source_directory(${PROJECT_SOURCE_DIR}/Startup/ startup)

aux_source_directory(${PROJECT_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/ hal_src)

aux_source_directory(${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/ rtos_source)
list(APPEND rtos_source "${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c"
                        "${PROJECT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c")

aux_source_directory(${PROJECT_SOURCE_DIR}/Src m_src_e)




add_executable(${PROJECT_NAME}.elf ${startup} ${m_src_e} ${rtos_source} ${hal_src} ${usbd_src})
SET(EXECUTABLE_NAME "${PROJECT_NAME}.elf")

add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex ${EXECUTABLE_NAME} ${PROJECT_NAME}.hex
)



