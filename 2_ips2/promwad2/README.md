# Настройка окружения для проекта

Необходимо скачать и установить Qt Creator с сайта https://www.qt.io/download, скачать необходимо open source версию.

После установки Qt Creator необходимо перейти меню Help -> About Plugins… и в Device Support выбрать BareMetal, после перезапустить Qt Creator.


Установите следующие пакеты:
* libtool
* eclipse-cdt-autotools
* libusb-1.0-0-dev

Установка openocd:
* git clone http://openocd.zylin.com/openocd 
* cd openocd
* ./bootstrap 
* ./configure --enable-ft2232_ftd2xx 
* make 
* sudo make install



Подключите stm32f746g-disco к ПК, откройте терминал и выполните следующую команду: 

* openocd -f board/stm32f746g-disco.cfg -c "init" -c "reset init" -c "reset" -c "shutdown" 

Вы должны увидеть информацию следующего характера:
~~~~
Open On-Chip Debugger 0.10.0+dev-00208-gc892814 (2018-04-02-12:10)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : The selected transport took over low-level target control. The results might differ compared to plain JTAG/SWD
adapter speed: 2000 kHz
adapter_nsrst_delay: 100
srst_only separate srst_nogate srst_open_drain connect_deassert_srst
Info : Unable to match requested speed 2000 kHz, using 1800 kHz
Info : Unable to match requested speed 2000 kHz, using 1800 kHz
Info : clock speed 1800 kHz
Info : STLINK v2 JTAG v25 API v2 SWIM v14 VID 0x0483 PID 0x374B
Info : using stlink api v2
Info : Target voltage: 3.231073
Warn : Silicon bug: single stepping will enter pending exception handler!
Info : stm32f7x.cpu: hardware has 8 breakpoints, 4 watchpoints
target halted due to debug-request, current mode: Thread 
xPSR: 0x01000000 pc: 0x0802a0b8 msp: 0x2004c000
adapter speed: 4000 kHz
shutdown command invoked 
~~~~

При сборке проекта происходит прошивка внутренней и внешней памяти (openocd должен быть отключен).

Для отладки должен быть выбран debug режим и запущен openocd, для запуска выполните команду в терминале:
* openocd -f board/stm32f746g-disco.cfg -c "init" -c "reset init"


Добавьте файл openocd.udev в /etc/udev/rules.d/, после перезапустите правила

Далее необходимо скачать toolchain https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

Настройка Qt Creator:
После процедуры установки, необходимо сконфигурировать необходимый qt kit для работы, для этого нужно выполнить действия показанные на скриншотах ниже. При сборки проекта Вы должны выбирать kit созданный для сборки под микроконтроллеры.


<img src="../doc/1.png" />
<br />


<img src="../doc/2.png" />
<br />


<img src="../doc/3.png" />
<br />


<img src="../doc/4.png" />
<br />


<img src="../doc/5.png" />
<br />


<img src="../doc/6.png" />
<br />


<img src="../doc/7.png" />
<br />


А так же необходимо прописать var objCopyPath = "/home/user/Downloads/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-objcopy" в qbs файле.

Для работы с itm необходимо для openocd прописать следующую конфигурацию.

~~~~
set remote hardware-breakpoint-limit 6
set remote hardware-watchpoint-limit 4
monitor reset halt
load
monitor reset halt
monitor tpiu config internal /home/user/prj.promwad.Fez-Telecom-Engineering/debug.itm uart off 200000000
monitor itm port 0 on
~~~~


Для отладки необходим открытый проект с Qt Creator и терминал в котором запущен openocd.
Запуск openocd: openocd -f board/stm32f746g-disco.cfg -c “init” -c “reset init”

Процедура развертывания проекта для ОС Windows аналогичная.


Тип проекта изменяются в файле stm32f7_voip.qbs:
     * property bool IsBootloader: true - если необходим проект загрузчика
     * property bool IsStm32f1: true - если необходим проект для платы с stm32f1
     * property bool IsStm32f7: true - если необходим проект для платы с stm32f7
     * property bool IsStm32f7: true и property bool SC2board: true - если необходим проект для платы СЦ2
Как подготовить прошивку для обновления:
     * В файле STM32F777IITx_FLASH.ld необходимо изменить адрес "FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 768K", на "FLASH (rx)      : ORIGIN = 0x08040200, LENGTH = 768K"
     * В файле system_stm32f7xx.c закомментировать следующий участок кода.   
~~~~     
/* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM1_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
~~~~
    *Файл прошивки подготовлен
    
