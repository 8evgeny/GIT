import qbs
import qbs.FileInfo
import qbs.ModUtils

//openocd -f board/stm32f746g-disco.cfg -c "init" -c "reset init" -c "tpiu config internal /home/user/prj.promwad.Fez-Telecom-Engineering/debug.itm uart off 200000000" -c "itm port 0 on" -c "reset"
//./itmdump -f ../debug.itm -d1
CppApplication  {

    name: "QT-STM32F777"
    type: ["application", "bin", "hex"]

    Depends {
        name: "cpp"
    }

    consoleApplication: true
    cpp.positionIndependentCode: false
    cpp.executableSuffix: ".elf"
    cpp.cxxLanguageVersion: "c++17"

    property int HeapVersion: 4 //version of heap for FreeRTOS
    property bool IsBootloader: false //falsetrue

    property bool IsStm32f1: false
    property bool IsStm32f7: true
    property bool SC2board: true

    property string nameOfBoard: "ST"

    property string Home: path
    property string Config: Home + "/Config"
    property string FreeRTOS: Home + "/Middlewares/Third_Party/FreeRTOS"
    property string CMSIS_RTOS: FreeRTOS + "/Source/CMSIS_RTOS"
    property string STM32F7xx_HAL: Home + "/Drivers/STM32F7xx_HAL_Driver"
    property string STM32F1xx_HAL: Home + "/Drivers/STM32F1xx_HAL_Driver"
    property string CMSIS_STM32F1xx: Home + "/Drivers/CMSIS_STM32F1xx"
    property string CMSIS_STM32F7xx: Home + "/Drivers/CMSIS_STM32F7xx"
    property string Inc: Home + "/Inc"
    property string Src: Home + "/Src"
    property string startup: Home + "/startup"
    property string LD: Home
    property string TraceRecorder: Home + "/Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder"
    property string LwIP: Home + "/Middlewares/Third_Party/LwIP"
    property string ArduinoJson: Home + "/Middlewares/Third_Party/ArduinoJson"
    property string CircularBuffer: Home + "/Middlewares/Third_Party/CircularBuffer"
    property string littlefs: Home + "/Middlewares/Third_Party/littlefs"
    property string UID: Src + "/UID"
    property string RS232: Src + "/RS232"
    property string RTP: Src + "/RTP"
    property string WDT: Src + "/WDT"
    property string Audio: Src + "/Audio"
    property string Debug: Src + "/Debug"
    property string TRNG: Src + "/TRNG"
    property string FatFs: Home + "/Middlewares/Third_Party/FatFs"
    property string Flash: Src + "/Flash"
    property string RTT: Home + "/RTT"
    property string Inc_STM32F1xx: Home + "/Inc/Inc_STM32F1xx"

    property string ringBuffer: Src + "/c-ringbuf"

    property string CAN_STM32F1xx: Src + "/CAN_STM32F1xx"
    property string CAN_STM32F7xx: Src + "/CAN_STM32F7xx"
    property string GPIO_STM32F1xx: Src + "/GPIO_STM32F1xx"
    property string GPIO_STM32F7xx: Src + "/GPIO_STM32F7xx"
    property string GPIO_SC2_BOARD: Src + "/GPIO_SC2_BOARD"
//	property string CAN_Command: Src + "/CAN_Command"
    property string CRC16_CCITT: Src + "/CRC16_CCITT"
    property string SNTP: Src + "/SNTP"
    property string RTC: Src + "/RTC"
    property string SRAM: Src + "/SRAM"
    property string JSON: Src + "/JSON"
    property string UDP_JSON: Src + "/UDP_JSON"
    property string Call_Control: Src + "/Call_Control"
    property string Call_Control_for_SC2_board: Src + "/Call_Control_for_SC2_board"

    property string EEPROM: Src + "/EEPROM"
    property string AES128: Src + "/Bootloader/AES128"
    property string MD5: Src + "/Bootloader/MD5"
    property string Version: Src + "/Bootloader/version"
    property string UpdateMechanism: Src + "/UpdateMechanism"

    Group {
        name: "FatFs"
        condition: IsStm32f7 == true || IsBootloader == true
        prefix: FatFs
        files: ["/src/*.c", "/src/*.h", "/src/option/syscall.c", "/src/option/unicode.c"]
    }

    Group {
        name: "Bootloader"
        condition: IsBootloader == true

        files: [Src + "/Bootloader/*.h", Src + "/Bootloader/*.cpp", AES128 + "/*.h",  AES128 + "/*.c", MD5 + "/*.h", MD5 + "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "Update mechanism for objects"
        files: ["Src/UpdateMechanism/updatemechanism.cpp", "Src/UpdateMechanism/updatemechanism.h"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "EEPROM (M24xx64 - 64KBit) for littlefs"
        prefix: EEPROM
        files: ["/*.c", "/*.h", "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "TraceRecorder (Percepio Tracealyzer)"
        prefix: TraceRecorder
        files: ["/*.c", "/streamports/TCPIP/*.c", "/streamports/TCPIP/include/*.h", "/include/*.h", "/config/*.h"]
    }

    Group {
        name: "docs"
        files: [Home + "/README.md", Home + "/doc/*"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "Testing system"
        files: ["Src/Tests/tests.cpp", "Src/Tests/tests.h"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "UID (unique identifier of mcu)"
        prefix: UID
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "RTP"
        prefix: RTP
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "WDT"
        prefix: WDT
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "Audio"
        prefix: Audio
        files: ["/*.h", "/*.cpp", "/*.c"]
    }

    Group {
        condition: IsStm32f7 == true || IsBootloader == true
        name: "Flash (internal flash of mcu)"
        prefix: Flash
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "TRNG (true random number generator)"
        prefix: TRNG
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "Debug (Instrumentation Trace Macrocell - ITM)"
        prefix: Debug
        files: ["/*.h", "/*.cpp", "/*.c"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "RS232"
        prefix: RS232
        files: ["/*.h", "/*.cpp", "/*.c"]
    }

    Group {
        name: "CAN STM32F7xx"
        prefix: CAN_STM32F7xx
        condition: IsStm32f7 == true && SC2board == false
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "CAN STM32F1xx"
        prefix: CAN_STM32F1xx
        condition: IsStm32f1 == true
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "GPIO STM32F1xx"
        prefix: GPIO_STM32F1xx
        condition: IsStm32f1 == true
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "GPIO STM32F7xx"
        prefix: GPIO_STM32F7xx
        condition: SC2board == false && IsStm32f7 == true
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "GPIO CS2 BOARD"
        prefix: GPIO_SC2_BOARD
        condition: SC2board == true && IsStm32f7 == true
        files: ["/*.h", "/*.cpp"]
    }

//	Group {
//		condition: SC2board == false && IsBootloader == false
//		name: "CAN Command"
//		prefix: CAN_Command
//		files: ["/*.h", "/*.cpp"]
//	}

//	Group {

//        condition: SC2board == false
//        name: "CRC16 CCITT"
//		prefix: CRC16_CCITT
//		files: ["/*.h", "/*.cpp"]
//	}

    Group {
        condition: IsStm32f7 == true
        name: "RTC"
        prefix: RTC
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "SRAM"
        prefix: SRAM
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "JSON"
        prefix: JSON
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "UDP JSON (Multicast)"
        prefix: UDP_JSON
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "LwIP"
        prefix: LwIP
        files: ["/src/include/*/*.h", "/src/include/*/*/*.h", "/src/netif/*.c", "/system/*.h", "/system/OS/*.h", "/src/api/*.c", "/src/apps/sntp/*.c", "/src/core/*.c", "/src/api/*.c", "/src/core/ipv4/*.c", "/system/OS/*.c"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "littlefs (a little fail-safe filesystem designed for microcontrollers)"
        prefix: littlefs
        files: ["/*.c", "/*.h"]
    }

    Group {
        condition: IsStm32f7 == true
        name: "ArduinoJson (C++ JSON library for IoT)"
        prefix: ArduinoJson
        files: ["/*.h"]
    }

    Group {
        //condition: IsBootloader == false
        name: "CircularBuffer"
        prefix: CircularBuffer
        files: ["/*.h", "/*.tpp"]
    }

    Group {
        name: "FreRTOS v10.0.1 (source files without heap_4/5)"
        prefix: FreeRTOS
        files: if (IsStm32f7 == true || IsBootloader == true) {
                   ["/Source/*.c", "/Source/include/*.h", "/Source/portable/GCC/ARM_CM7/r0p1/*.h", "/Source/portable/GCC/ARM_CM7/r0p1/*.c"]
         } else if (IsStm32f1 == true) {
                   ["/Source/*.c", "/Source/include/*.h", "/Source/portable/GCC/ARM_CM3/*.h", "/Source/portable/GCC/ARM_CM3/*.c"]
               }
    }

    Group {
        name: "FreRTOS v10.0.1 (heap 4)"
        prefix: FreeRTOS
        condition: HeapVersion == 4
        files: ["/Source/portable/MemMang/heap_4.c"]
    }

    Group {
        name: "FreRTOS v10.0.1 (heap 5)"
        prefix: FreeRTOS
        condition: HeapVersion == 5 && IsStm32f7 == true
        files: ["/Source/portable/MemMang/heap_5.c"]
    }

    Group {
        //condition: IsStm32f7 == true
        name: "CMSIS-RTOS"
        prefix: CMSIS_RTOS
        files: ["/*.c", "/*.h"]
    }

    Group {
        name: "STM32F7xx HAL Driver (Hardware abstraction layer)"
        prefix: STM32F7xx_HAL
        condition: IsStm32f7 == true || IsBootloader == true
        files: ["/Src/*.c", "/Inc/*.h", "/Inc/Legacy/*.h"]
        excludeFiles: ["/Src/stm32f7xx_hal_timebase_rtc_alarm_template.c", "/Src/stm32f7xx_hal_timebase_rtc_wakeup_template.c", "/Src/stm32f7xx_hal_timebase_tim_template.c", "/Src/stm32f7xx_hal_msp_template.c"]
    }

    Group {
        name: "STM32F1xx HAL Driver (Hardware abstraction layer)"
        prefix: STM32F1xx_HAL
        condition: IsStm32f1 == true
        files: ["/Src/*.c", "/Inc/*.h", "/Inc/Legacy/*.h"]
        excludeFiles: ["/Src/stm32f1xx_hal_timebase_rtc_alarm_template.c", "/Src/stm32f1xx_hal_timebase_rtc_wakeup_template.c", "/Src/stm32f1xx_hal_timebase_tim_template.c", "/Src/stm32f1xx_hal_msp_template.c"]
    }

    Group {
        name: "CMSIS STM32F7xx"
        prefix: CMSIS_STM32F7xx
        condition: IsStm32f7 == true || IsBootloader == true
        files: ["/Include/*.h", "/Device/ST/STM32F7xx/Source/Templates/*", "/Device/ST/STM32F7xx/Include/*.h",
            "/DSP/Include/*.h",
            "/DSP/Source/SupportFunctions/*.c",
            "/DSP/Source/StatisticsFunctions/*.c",
            "/DSP/Source/BasicMathFunctions/*.c"]

        excludeFiles: ["/Device/ST/STM32F7xx/Include/stm32f7xx_hal_wwdg.h",
            "/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c",
            "/Device/ST/STM32F7xx/Include/stm32f7xx_hal_conf_template.h",]
    }

    Group {
        name: "CMSIS STM32F1xx"
        prefix: CMSIS_STM32F1xx
        condition: IsStm32f1 == true
        files: ["/Include/*.h", "/Device/ST/STM32F1xx/Source/Templates/*", "/Device/ST/STM32F1xx/Include/*.h"]
        excludeFiles: ["/Device/ST/STM32F1xx/Include/stm32f1xx_hal_wwdg.h", "/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c", "/Device/ST/STM32F1xx/Include/stm32f1xx_hal_conf_template.h"]
    }

    Group {
        condition: IsStm32f7 == true /*&& SC2board == false*/
        name: "Call control"
        prefix: Call_Control
        files: ["/*.h", "/*.cpp"]
        excludeFiles: ["/call_control.h", "/call_control.cpp"]
    }

//    Group {
//        condition: IsStm32f7 == true && SC2board == true
//        name: "Call control (for SC2 board)"
//        prefix: Call_Control_for_SC2_board
//        files: ["/*.h", "/*.cpp"]

//    }

    Group {
        condition: IsStm32f7 == true
        name: "SNTP"
        prefix: SNTP
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "Inc (include files)"
        prefix: Inc
        files: if (IsStm32f7 || IsBootloader) {
                   ["/*.h"]
               } else if (IsStm32f1) {
                   ["/Inc_STM32F1xx/*.h"]
               }
    }

    Group {
        name: "Src (source files)"
        prefix: Src
        files: if (IsStm32f7) {
                   ["/*.c", "/main.cpp", "/flash_diskio.cpp", "/../syscalls.c"]
//               } else if (IsStm32f7 && SC2board) {
//                   ["/*.c", "/main_sc2_board.cpp", "/flash_diskio.cpp", "/../syscalls.c"]
               } else if (IsStm32f1) {
                   ["/Src_STM32F1xx/*.c", "/Src_STM32F1xx/*.cpp"]
               } else if (IsBootloader) {
                    ["/Src_Bootloader/*.c", "/Src_Bootloader/*.cpp", "/../syscalls.c"]
               }
    }

    Group {
        name: "startup (.s)"
        prefix: startup
        files: if (IsStm32f7 || IsBootloader) {
                   ["/startup_stm32f777xx.s"]
               } else if (IsStm32f1) {
                   ["/startup_stm32f103xb.s"]
               }
    }

    Group {
        name: "LD (linker script)"
        prefix: LD
        files: if (IsStm32f7) {
                   ["/STM32F777IITx_FLASH.ld"]
               } else if (IsStm32f1) {
                   ["/STM32F103RBTx_FLASH.ld"]
               }
    }

    cpp.includePaths: if (IsStm32f7 && !SC2board) {
                          [
                          Home,
                          CMSIS_STM32F7xx,
                          CMSIS_STM32F7xx + "/Include",
                          CMSIS_STM32F7xx + "/DSP/Include/",
                          CMSIS_STM32F7xx + "/Device/ST/STM32F7xx/Include",
                          Inc,
                          FreeRTOS + "/Source/include",
                          FreeRTOS + "/Source/portable/GCC/ARM_CM7/r0p1",
                          CMSIS_RTOS,
                          STM32F7xx_HAL + "/Inc",
                          STM32F7xx_HAL + "/Inc/Legacy",
                          TraceRecorder + "/streamports/TCPIP/include",
                          TraceRecorder + "/include",
                          TraceRecorder + "/config",
                          LwIP + "/src/include",
                          LwIP + "/system",
                          LwIP + "/system/OS",
                          LwIP + "/src",
                          ArduinoJson,
                          ArduinoJson + "/src",
                          ArduinoJson + "/src/ArduinoJson",
                          CircularBuffer,
                          littlefs,
                          FatFs + "/src",
                          RTP,
                          Audio,
                          UID,
                          Flash,
                          EEPROM,
                          CAN_STM32F7xx,
                          GPIO_STM32F7xx,
                          RTT,
                          RTC,
//                          CAN_Command,
//                          CRC16_CCITT,
                          SRAM,
                          RS232,
                          JSON,
                          WDT,
                          UDP_JSON,
                          Call_Control
                          ]
                      } else if (IsStm32f7 && SC2board) {
                          [
                          Home,
                          CMSIS_STM32F7xx,
                          CMSIS_STM32F7xx + "/Include",
                          CMSIS_STM32F7xx + "/Device/ST/STM32F7xx/Include",
                          CMSIS_STM32F7xx + "/DSP/Include/",
                          Inc,
                          FreeRTOS + "/Source/include",
                          FreeRTOS + "/Source/portable/GCC/ARM_CM7/r0p1",
                          CMSIS_RTOS,
                          STM32F7xx_HAL + "/Inc",
                          STM32F7xx_HAL + "/Inc/Legacy",
                          TraceRecorder + "/streamports/TCPIP/include",
                          TraceRecorder + "/include",
                          TraceRecorder + "/config",
                          LwIP + "/src/include",
                          LwIP + "/system",
                          LwIP + "/system/OS",
                          LwIP + "/src",
                          ArduinoJson,
                          ArduinoJson + "/src",
                          ArduinoJson + "/src/ArduinoJson",
                          CircularBuffer,
                          littlefs,
                          FatFs + "/src",
                          RTP,
                          Audio,
                          UID,
                          Flash,
                          EEPROM,
                          GPIO_SC2_BOARD,
                          RTT,
                          RTC,
                          SRAM,
                          RS232,
                          JSON,
                          WDT,
                          UDP_JSON,
                          Call_Control
                          ]
                      } else if (IsStm32f1) {
                          [
                          Home,
                          CMSIS_STM32F1xx,
                          CMSIS_STM32F1xx + "/Include",
                          CMSIS_STM32F1xx + "/Device/ST/STM32F1xx/Include",
                          FreeRTOS + "/Source/include",
                          FreeRTOS + "/Source/portable/GCC/ARM_CM3",
                          CMSIS_RTOS,
                          STM32F1xx_HAL + "/Inc",
                          CircularBuffer,
                          CAN_STM32F1xx,
                          GPIO_STM32F1xx,
//                          CAN_Command,
                          Inc_STM32F1xx,
//                          CRC16_CCITT,
                          WDT
                          ]
                      }
                      else if (IsBootloader) {
                          [
                          Home,
                          CMSIS_STM32F7xx,
                          CMSIS_STM32F7xx + "/Include",
                          CMSIS_STM32F7xx + "/Device/ST/STM32F7xx/Include",
                          Inc,
                          FreeRTOS + "/Source/include",
                          FreeRTOS + "/Source/portable/GCC/ARM_CM7/r0p1",
                          CMSIS_RTOS,
                          STM32F7xx_HAL + "/Inc",
                          STM32F7xx_HAL + "/Inc/Legacy",
                          FatFs + "/src",
                          Flash,
                          WDT,
                          CircularBuffer,
                          AES128
                          ]
                      }


    cpp.defines: if (IsStm32f7 && !SC2board) {
                     [
//                     "LWIP_DEBUG",
                         "STM32F777xx",
                         "USE_HAL_DRIVER",
                         //"USE_STM32746G_DISCOVERY",
                 //        "MBEDTLS_CONFIG_FILE=<mbedtls_config.h>",
                         "__weak=__attribute__((weak))",
                         "__packed=__attribute__((__packed__))",
                         "__FPU_PRESENT=1",
                         "ARM_MATH_CM7",
                         "HAVE_CONFIG_H",
                         "__DCACHE_PRESENT",
                         "__ICACHE_PRESENT"
                     ]
                 } else if (IsStm32f7 && SC2board) {
                     [
                     //"LWIP_DEBUG",
                         "STM32F777xx",
                         "USE_HAL_DRIVER",
                         //"USE_STM32746G_DISCOVERY",
                 //        "MBEDTLS_CONFIG_FILE=<mbedtls_config.h>",
                         "__weak=__attribute__((weak))",
                         "__packed=__attribute__((__packed__))",
                         "__FPU_PRESENT=1",
                         "ARM_MATH_CM7",
                         "HAVE_CONFIG_H",
//                         "__DCACHE_PRESENT",
//                         "__ICACHE_PRESENT",
                         "SC2BOARD"
                            ]


                 }

                else if (IsBootloader) {
                         [
//                     "LWIP_DEBUG",
                             "STM32F777xx",
                             "USE_HAL_DRIVER",
                             "__weak=__attribute__((weak))",
                             "__packed=__attribute__((__packed__))",
                             "__FPU_PRESENT=1",
                             "ARM_MATH_CM7",
                             "HAVE_CONFIG_H",
                             "__DCACHE_PRESENT",
                             "__ICACHE_PRESENT",
                              "BOOTLOADER_FEZ",
                            ]
                     }


                 else if (IsStm32f1) {
                       [ "USE_HAL_DRIVER",
                         "STM32F103xB",
                         "ARM_MATH_CM3"
                     ]
                 }

    cpp.commonCompilerFlags: if (IsStm32f7 || IsBootloader) {
                                 ["-mcpu=cortex-m7", "-mfloat-abi=hard", "-mfpu=fpv5-sp-d16", "-mthumb"]
                             } else if (IsStm32f1) {
                                 ["-mcpu=cortex-m3", "-mthumb"]
                             }


    cpp.driverFlags: if (IsStm32f7|| IsBootloader) {

                         ["-mcpu=cortex-m7",
                         "-mfloat-abi=hard",
                         "-mfpu=fpv5-sp-d16",
                         "-mthumb", "-Xlinker",
                         "--gc-sections",
                         "-specs=nosys.specs",
                         "-specs=nano.specs",
//                         "-fno-exceptions",
//                         "-fno-rtti",
//                         "-u_printf_float",
//                         "-Wl,-section-start=.rtt=0x20000000",
                         "-Wl,-Map=" + path + "/QT-STM32746G-Discovery.map"]
                     } else if (IsStm32f1) {
                         ["-mcpu=cortex-m3",
                         "-mthumb",
                         "-Xlinker",
                         "--gc-sections",
                         "-specs=nosys.specs",
                         "-specs=nano.specs",
//                         "-fno-exceptions",
                         //"-fno-rtti",
                         //"-u_printf_float",
                         //"-Wl,-section-start=.rtt=0x20000000",
                         "-Wl,-Map=" + path + "/QT-STM32746G-Discovery.map"]
                     }

    cpp.linkerFlags: if (IsStm32f7) {["--start-group",
                         "--gc-sections",
                         "-T" + path
                        + "/STM32F777IITx_FLASH.ld",
                         "-lm",
                         "-lnosys",
                         "-lgcc",
                         "-lc",
                         "-lstdc++",
                         "--end-group"]
                     }else if (IsStm32f1){
                         ["--start-group",
                         "--gc-sections",
                         "-T" + path
                                 + "/STM32F103RBTx_FLASH.ld",
                         "-lm",
                         "-lnosys",
                         "-lgcc",
                         "-lc",
                         "-lstdc++",
                         "--end-group"]
                     }
                     else if (IsBootloader){
                         ["--start-group",
                          "--gc-sections",
                          "-T" + path
                         + "/STM32F777IITx_FLASH_BOOTLOADER.ld",
                          "-lm",
                          "-lnosys",
                          "-lgcc",
                          "-lc",
                          "-lstdc++",
                         "--end-group"]
                                          }

    cpp.cxxFlags: "-std=c++17"

    Properties {
        condition: qbs.buildVariant === "debug"
        cpp.debugInformation: true
        cpp.optimization: "none"
    }

    Properties {
        condition: qbs.buildVariant === "release"
        cpp.debugInformation: true
        cpp.optimization: "small" //"none", "fast", "small"
    }

    Properties {
        condition: cpp.debugInformation
        cpp.defines: outer.concat("DEBUG")
    }

    Group {
        // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    Rule {
        id: binDebugFrmw
        condition: qbs.buildVariant === "debug"
        inputs: ["application"]

        Artifact {
            fileTags: ["bin"]
            filePath: input.baseDir + "/" + input.baseName + ".bin"
        }

        //need to add gcc-arm-none-eabi bin dir to add system PATH
        ///home/user/Downloads/gcc-arm-none-eabi-8-2018-q4-major/bin/
        prepare: {

            var objCopyPath = "arm-none-eabi-objcopy"

            var argsConv = ["-O", "binary", input.filePath, output.filePath]
            var cmd = new Command(objCopyPath, argsConv)
            cmd.description = "converting to BIN: " + FileInfo.fileName(
                        input.filePath) + " -> " + input.baseName + ".bin"

            //Запись в nor память по qspi
            var argsFlashingQspi = ["-f", "interface/stlink-v2-1.cfg",  "-f",  "target/stm32f7x.cfg", "-c", "init", "-c", "reset init", "-c", "flash write_bank 1 "
                                    + output.filePath + " 0", "-c", "reset", "-c", "shutdown"]

            var cmdFlashQspi = new Command("openocd", argsFlashingQspi)
            cmdFlashQspi.description = "Wrtie to the NOR QSPI"

            //Запись во внутреннюю память
            var argsFlashingInternalFlash = ["-f", "interface/stlink-v2-1.cfg",  "-f",  "target/stm32f7x.cfg", "-c", "init", "-c", "reset init", "-c", "flash write_image erase "
                                             + input.filePath, "-c", "reset", "-c", "shutdown"]

            var cmdFlashInternalFlash = new Command("openocd",
                                                    argsFlashingInternalFlash)
            cmdFlashInternalFlash.description = "Wrtie to the internal flash"

            //Открытие openocd
            var cmdOpenOcd = new Command("terminator",
                                         "-me terminator -me \'openocd -f target/stm32f7x.cfg -c \"init\" -c \"reset init\"\' &")

            //Открытие openocd
            var cmdOpenocdVersion = new Command("nohup openocd --version &")
            cmdOpenocdVersion.description = "get openocd version"

            cmdOpenOcd.description = "Opening openocd"

            //Открытие Tracealyzer
            var cmdOpenTracealyzer = new Command("terminator",
                                                 "-me /home/user/jedi_way_mcu/Tracealyzer-4.2.11-linux64/launch-tz.sh &")
            cmdOpenTracealyzer.description = "Opening Tracealyzer"

            //			return [cmd, cmdFlashQspi, cmdFlashInternalFlash, cmdOpenOcd, cmdOpenTracealyzer]
            var cmdAbout = new JavaScriptCommand()
            cmdAbout.version = "0.1.0"
            cmdAbout.date = "28/05/2019"
            cmdAbout.authors = "Shulenkov R.A."
            cmdAbout.sourceCode = function () {
                console.info("version : " + version)
                console.info("date : " + date)
                console.info("authors : " + authors)
            }

            //return [cmdAbout, cmdOpenocdVersion]
            return cmd
        }
    }

    Rule {
        id: hexFrmwDgb
        condition: qbs.buildVariant === "debug"
        inputs: ["application"]

        Artifact {
            fileTags: ["hex"]
            filePath: input.baseDir + "/" + input.baseName + ".hex"
        }

        prepare: {
            var objCopyPath = "arm-none-eabi-objcopy"
            var argsConv = ["-O", "ihex", input.filePath, output.filePath]
            var cmd = new Command(objCopyPath, argsConv)
            cmd.description = "converting to HEX: " + FileInfo.fileName(
                        input.filePath) + " -> " + input.baseName + ".hex"

            return [cmd]
        }
    }

    Rule {
        id: binFrmw
        condition: qbs.buildVariant === "release"
        inputs: ["application"]

        Artifact {
            fileTags: ["bin"]
            filePath: input.baseDir + "/" + input.baseName + ".bin"
        }

        //need to add gcc-arm-none-eabi bin dir to add system PATH
        ///home/user/Downloads/gcc-arm-none-eabi-8-2018-q4-major/bin/
        prepare: {
            var objCopyPath = "arm-none-eabi-objcopy"

            var argsConv = ["-O", "binary", input.filePath, output.filePath]
            var cmd = new Command(objCopyPath, argsConv)
            cmd.description = "converting to BIN: " + FileInfo.fileName(
                        input.filePath) + " -> " + input.baseName + ".bin"

            //Запись в nor память по qspi
            var argsFlashingQspi = ["-f", "board/stm32f746g-disco.cfg", "-c", "init", "-c", "reset init", "-c", "flash write_bank 1 "
                                    + output.filePath + " 0", "-c", "reset", "-c", "shutdown"]

            var cmdFlashQspi = new Command("openocd", argsFlashingQspi)
            cmdFlashQspi.description = "Wrtie to the NOR QSPI"

            //Запись во внутреннюю память
            var argsFlashingInternalFlash = ["-f", "board/stm32f746g-disco.cfg", "-c", "init", "-c", "reset init", "-c", "flash write_image erase "
                                             + input.filePath, "-c", "reset", "-c", "shutdown"]

            var cmdFlashInternalFlash = new Command("openocd",
                                                    argsFlashingInternalFlash)
            cmdFlashInternalFlash.description = "Wrtie to the internal flash"

//			return [cmd, cmdFlashQspi, cmdFlashInternalFlash]
            return cmd
        }
    }

    Rule {
        id: hexFrmw
        condition: qbs.buildVariant === "release"
        inputs: ["application"]

        Artifact {
            fileTags: ["hex"]
            filePath: input.baseDir + "/" + input.baseName + ".hex"
        }

        prepare: {
            var objCopyPath = "arm-none-eabi-objcopy"
            var argsConv = ["-O", "ihex", input.filePath, output.filePath]
            var cmd = new Command(objCopyPath, argsConv)
            cmd.description = "converting to HEX: " + FileInfo.fileName(
                        input.filePath) + " -> " + input.baseName + ".hex"

            return [cmd]
        }
    }
}
