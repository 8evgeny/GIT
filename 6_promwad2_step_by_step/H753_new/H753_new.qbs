import qbs
import qbs.FileInfo
import qbs.ModUtils

//openocd -f board/stm32f746g-disco.cfg -c "init" -c "reset init" -c "tpiu config internal /home/user/prj.promwad.Fez-Telecom-Engineering/debug.itm uart off 200000000" -c "itm port 0 on" -c "reset"
//./itmdump -f ../debug.itm -d1
CppApplication  {

    name: "H753_new"
    type: ["application", "bin", "hex"]

    Depends {
        name: "cpp"
    }

    consoleApplication: true
    cpp.positionIndependentCode: false
    cpp.executableSuffix: ".elf"
    cpp.cxxLanguageVersion: "c++17"

    property int HeapVersion: 4 //version of heap for FreeRTOS

    property string Home: path
    property string Src: Home + "/Core/Src"
    property string RS232: Src + "/RS232"
    property string Debug: Src + "/Debug"
    property string littlefs: Home + "/Middlewares/Third_Party/littlefs"
    property string EEPROM: Src + "/EEPROM"
    property string CircularBuffer: Home + "/Middlewares/Third_Party/CircularBuffer"
    property string SRAM: Src + "/SRAM"
    property string JSON: Src + "/JSON"
    property string ArduinoJson: Home + "/Middlewares/Third_Party/ArduinoJson"
    property string RTC: Src + "/RTC"
    property string SNTP: Src + "/SNTP"
    property string TRNG: Src + "/TRNG"
    property string GPIO: Src + "/GPIO"
    property string TraceRecorder: Home + "/Tracealyzer-4.2.12-linux64/FreeRTOS/TraceRecorder"
    property string RTP: Src + "/RTP"
    property string Audio: Src + "/AUDIO"
    property string UDP_JSON: Src + "/UDP_JSON"
    property string Call_Control: Src + "/Call_Control"
    property string Flash: Src + "/Flash"
    property string FatFs: Home + "/Middlewares/Third_Party/FatFs"
    property string UID: Src + "/UID"
    property string WDT: Src + "/WDT"
    property string startup: Home + "/Core/Startup"

    property string FreeRTOS: Home + "/Middlewares/Third_Party/FreeRTOS"
    property string CMSIS_RTOS: FreeRTOS + "/Source/CMSIS_RTOS"
    property string STM32H7xx_HAL: Home + "/Drivers/STM32H7xx_HAL_Driver"
    property string CMSIS_STM32H7xx: Home + "/Drivers/CMSIS"
    property string Inc: Home + "/Core/Inc"
    property string LD: Home
    property string LwIP: Home + "/Middlewares/Third_Party/LwIP"
    property string LwIP1: Src + "/LWIP"
    property string RTT: Home + "/RTT"
    property string ringBuffer: Src + "/c-ringbuf"
    property string CRC16_CCITT: Src + "/CRC16_CCITT"
    property string Call_Control_for_SC2_board: Src + "/Call_Control_for_SC2_board"
    property string AES128: Src + "/Bootloader/AES128"
    property string MD5: Src + "/Bootloader/MD5"
    property string Version: Src + "/Bootloader/version"
    property string UpdateMechanism: Src + "/UpdateMechanism"

    Group {
        name:"lostFiles"
        files:  [
        Home + "/Drivers/CMSIS/DSP/Source/SupportFunctions/arm_copy_q15.c",
        Home + "/Drivers/CMSIS/DSP/Source/SupportFunctions/arm_copy_q7.c",
        Home + "/Drivers/CMSIS/DSP/Source/SupportFunctions/arm_fill_q15.c",
        Home + "/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.c",
        Home + "/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c",
        Home + "/Core/Src/flash/flash.cpp"
    ]
    }

    Group {
        name: "FatFs"
        prefix: FatFs
        files: ["/src/*.c", "/src/*.h", "/src/option/syscall.c", "/src/option/unicode.c"]
    }

    Group {
        name: "Update mechanism for objects"
        files: ["Src/UpdateMechanism/updatemechanism.cpp", "Src/UpdateMechanism/updatemechanism.h"]
    }

    Group {
        name: "TraceRecorder (Percepio Tracealyzer)"
        prefix: TraceRecorder
        files: ["/*.c", "/streamports/TCPIP/*.c", "/streamports/TCPIP/include/*.h", "/include/*.h", "/config/*.h"]
    }

    Group {
        name: "docs"
        files: [Home + "/README.md", Home + "/doc/*"]
    }

    Group {
        name: "Testing system"
        files: ["Src/Tests/tests.cpp", "Src/Tests/tests.h"]
    }

    Group {
        name: "UID (unique identifier of mcu)"
        prefix: UID
        files: ["/*.h", "/*.cpp"]
    }

    Group {
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
        name: "Audio"
        prefix: Audio
        files: ["/*.h", "/*.cpp", "/*.c"]
    }

    Group {
        name: "Flash (internal flash of mcu)"
        prefix: Flash
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "TRNG (true random number generator)"
        prefix: TRNG
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "GPIO"
        prefix: GPIO_STM32H7xx
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "GPIO CS2 BOARD"
        prefix: GPIO_SC2_BOARD
        files: ["/*.h", "/*.cpp"]
    }


    Group {
        name: "RTC"
        prefix: RTC
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "SRAM"
        prefix: SRAM
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "JSON"
        prefix: JSON
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "UDP JSON (Multicast)"
        prefix: UDP_JSON
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "ArduinoJson (C++ JSON library for IoT)"
        prefix: ArduinoJson
        files: ["/*.h"]
    }

    Group {
        name: "CMSIS STM32H7xx"
        prefix: CMSIS_STM32H7xx
        files: ["/Include/*.h", "/Device/ST/STM32H7xx/Source/Templates/*", "/Device/ST/STM32H7xx/Include/*.h",
            "/DSP/Include/*.h",
            "/DSP/Source/SupportFunctions/*.c",
            "/DSP/Source/StatisticsFunctions/*.c",
            "/DSP/Source/BasicMathFunctions/*.c"]

        excludeFiles: ["/Device/ST/STM32H7xx/Include/stm32h7xx_hal_wwdg.h",
            "/Device/ST/STM32H7xx/Source/Templates/system_stm32h7xx.c",
            "/Device/ST/STM32H7xx/Include/stm32h7xx_hal_conf_template.h",]
    }

    Group {
        name: "Call control"
        prefix: Call_Control
        files: ["/*.h", "/*.cpp"]
        excludeFiles: ["/call_control.h", "/call_control.cpp"]
    }

    Group {
        name: "SNTP"
        prefix: SNTP
        files: ["/*.h", "/*.cpp"]
    }

    Group {
        name: "LwIP"
        prefix: LwIP
        files: ["/src/include/*/*.h", "/src/include/*/*/*.h", "/src/netif/*.c", "/system/*.h", "/system/OS/*.h", "/src/api/*.c", "/src/apps/sntp/*.c", "/src/core/*.c", "/src/api/*.c", "/src/core/ipv4/*.c", "/system/OS/*.c"]
    }

    Group {
        name: "LwIP1"
        prefix: LwIP1
        files: ["/App/*.c", "/App/*.h", "/Target/*.c", "/Target/*.h"]
    }

    Group {
        name: "DP83848"
        prefix: Home
        files: ["/Drivers/BSP/Components/dp83848/*.c", "/Drivers/BSP/Components/dp83848/*.h"]
    }

    Group {
        name: "littlefs (a little fail-safe filesystem designed for microcontrollers)"
        prefix: littlefs
        files: ["/*.c", "/*.h"]
    }

    Group {
        name: "EEPROM (M24xx64 - 64KBit) for littlefs"
        prefix: EEPROM
        files: ["/*.c", "/*.h", "/*.cpp"]
    }

    Group {
        name: "Debug (Instrumentation Trace Macrocell - ITM)"
        prefix: Debug
        files: ["/*.h", "/*.cpp", "/*.c"]
    }

    Group {
        name: "RS232"
        prefix: RS232
        files: ["/*.h", "/*.cpp", "/*.c"]
    }

    Group {
        name: "CircularBuffer"
        prefix: CircularBuffer
        files: ["/*.h", "/*.tpp"]
    }

    Group {
        name: "FreRTOS v10.0.1 (source files without heap_4/5)"
        prefix: FreeRTOS
        files: ["/Source/*.c", "/Source/include/*.h",
            "/Source/portable/GCC/ARM_CM7/r0p1/*.h",
            "/Source/portable/GCC/ARM_CM7/r0p1/*.c",
            "/Source/portable/GCC/ARM_CM4F/*.h",
            "/Source/portable/GCC/ARM_CM4F/*.c"
        ]
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
        condition: HeapVersion == 5
        files: ["/Source/portable/MemMang/heap_5.c"]
    }

    Group {
        name: "CMSIS-RTOS"
        prefix: CMSIS_RTOS
        files: ["/*.c", "/*.h"]
    }

    Group {
        name: "STM32H7xx HAL Driver (Hardware abstraction layer)"
        prefix: STM32H7xx_HAL
        files: ["/Src/*.c", "/Inc/*.h", "/Inc/Legacy/*.h"]
        excludeFiles: ["/Src/stm32h7xx_hal_timebase_rtc_alarm_template.c", "/Src/stm32h7xx_hal_timebase_rtc_wakeup_template.c", "/Src/stm32h7xx_hal_timebase_tim_template.c", "/Src/stm32h7xx_hal_msp_template.c"]
    }

    Group {
        name: "Inc (include files)"
        prefix: Inc
        files:     ["/*.h"]
        }

    Group {
        name: "Src (source files)"
        prefix: Src
        files: ["/*.c", "/*.cpp"
//            "/flash_diskio.cpp"
        ]
//            ["/Src_Bootloader/*.c", "/Src_Bootloader/*.cpp", "/../syscalsyscalls.cls.c"]

        excludeFiles: ["/net_sockets.c", "/syscalls.c" ]
         }

    Group {
        name: "startup (.s)"
        prefix: startup
        files:  ["/startup_stm32h753iitx.s"]
        }

    Group {
        name: "LD (linker script)"
        prefix: LD
        files: ["/STM32H753IITX_FLASH.ld"]
    }

    cpp.includePaths:  {
        [
        Inc,
        Home,
        CMSIS_STM32H7xx + "/Include",
        CMSIS_STM32H7xx + "/DSP/Include/",
        CMSIS_STM32H7xx + "/Device/ST/STM32H7xx/Include",
        Home + "/Drivers/BSP/Components/dp83848",
        STM32H7xx_HAL + "/Inc",
        STM32H7xx_HAL + "/Inc/Legacy",
        FreeRTOS + "/Source/include",
        CMSIS_RTOS,
        FreeRTOS + "/Source/portable/MemMang",
        FreeRTOS + "/Source/portable/GCC/ARM_CM4F",
        ArduinoJson + "/src",
        LwIP + "/system/arch",
        LwIP + "/system",
        LwIP + "/src/include/netif",
        LwIP + "/src/include/netif/ppp",
        LwIP + "/src/include",
        LwIP + "/src/include/lwip",
        LwIP + "/src/include/lwip/apps",
        LwIP + "/src/include/lwip/priv",
        LwIP + "/src/include/lwip/prot",
        LwIP + "/src/include/compat/posix",
        LwIP + "/src/include/compat/arpa",
        LwIP + "/src/include/compat/net",
        LwIP + "/src/include/compat/sys",
        RS232,
        littlefs,
        EEPROM,
        CircularBuffer,
        SRAM,
        JSON,
        ArduinoJson,
        RTC,
        SNTP,
        TRNG,
        GPIO,
        TraceRecorder + "/streamports/TCPIP/include",
        TraceRecorder + "/include",
        TraceRecorder + "/config",
        RTP,
        Audio,
        UDP_JSON,
        Call_Control,
        Flash,
        FatFs,
        UID,
        WDT,

        RTT,
                          ]
                      }



    cpp.defines:  {
                     [
                     //"LWIP_DEBUG",
                         "STM32H753xx",
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
                            ]
                 }


    cpp.commonCompilerFlags:  ["-mcpu=cortex-m7", "-mfloat-abi=hard", "-mfpu=fpv5-sp-d16", "-mthumb"]


    cpp.driverFlags:     ["-mcpu=cortex-m7",
                         "-mfloat-abi=hard",
                         "-mfpu=fpv5-sp-d16",
                         "-mthumb", "-Xlinker",
                         "--gc-sections",
        "-Os",
        "-ffunction-sections",
        "-fdata-sections",
                         "-specs=nosys.specs",
                         "-specs=nano.specs",
//                         "-fno-exceptions",
//                         "-fno-rtti",
//                         "-u_printf_float",
//                         "-Wl,-section-start=.rtt=0x20000000",
                         "-Wl,-Map=" + path + "/QT-STM32H753.map"]

    cpp.linkerFlags: ["--start-group",
                         "--gc-sections",
                         "-T" + path
                        + "/STM32H753IITX_FLASH.ld",
                         "-lm",
                         "-lnosys",
                         "-lgcc",
                         "-lc",
                         "-lstdc++",
                         "--end-group"]

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
            cmd.description = "converting to BIN: " + FileInfo.fileName(input.filePath) + " -> " + input.baseName + ".bin"

            //Запись в nor память по qspi
            var argsFlashingQspi =
                    [
                        "-f", "interface/stlink-v2-1.cfg",  "-f",  "target/stm32h7x.cfg", "-c", "init", "-c", "reset init",
                        "-c", "flash write_bank 1 " + output.filePath + " 0", "-c", "reset", "-c", "shutdown"
                    ]
            var cmdFlashQspi = new Command("openocd", argsFlashingQspi)
            cmdFlashQspi.description = "Wrtie to the NOR QSPI"

            //Запись во внутреннюю память
            var argsFlashingInternalFlash =
                    [
                        "-f", "interface/stlink-v2-1.cfg",  "-f",  "target/stm32h7x.cfg", "-c", "init", "-c", "reset init",
                        "-c", "flash write_image erase " + input.filePath, "-c", "reset", "-c", "shutdown"
                    ]
            var cmdFlashInternalFlash = new Command("openocd", argsFlashingInternalFlash)
            cmdFlashInternalFlash.description = "Wrtie to the internal flash"

            //Открытие openocd
            var cmdOpenOcd = new Command("terminator",
                                         "-me terminator -me \'openocd -f target/stm32h7x.cfg -c \"init\" -c \"reset init\"\' &")

            //Открытие openocd
            var cmdOpenocdVersion = new Command("nohup openocd --version &")
            cmdOpenocdVersion.description = "get openocd version"
            cmdOpenOcd.description = "Opening openocd"

            //Открытие Tracealyzer
            var cmdOpenTracealyzer = new Command("terminator",
                                                 "-me /home/user/jedi_way_mcu/Tracealyzer-4.2.11-linux64/launch-tz.sh &")
            cmdOpenTracealyzer.description = "Opening Tracealyzer"

//            return [cmd, cmdFlashQspi, cmdFlashInternalFlash, cmdOpenOcd, cmdOpenTracealyzer]

            var cmdAbout = new JavaScriptCommand()
            cmdAbout.version = "0.1.0"
            cmdAbout.date = "27/06/2022"
            cmdAbout.authors = "Parubets E.V."
            cmdAbout.sourceCode = function () {
                console.info("version : " + version)
                console.info("date : " + date)
                console.info("authors : " + authors)
            }

            return [cmdAbout, cmdOpenocdVersion]
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
            var argsFlashingQspi = [
                        "-f", "board/stm32h746g-disco.cfg", "-c", "init", "-c", "reset init", "-c", "flash write_bank 1 "
                        + output.filePath + " 0", "-c", "reset", "-c", "shutdown"
                    ]

            var cmdFlashQspi = new Command("openocd", argsFlashingQspi)
            cmdFlashQspi.description = "Wrtie to the NOR QSPI"

            //Запись во внутреннюю память
            var argsFlashingInternalFlash = ["-f", "board/stm32h7x3i_eval.cfg", "-c", "init", "-c", "reset init", "-c", "flash write_image erase "
                                             + input.filePath, "-c", "reset", "-c", "shutdown"]
            var cmdFlashInternalFlash = new Command("openocd", argsFlashingInternalFlash)
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
