#include "testTasks.h"
#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "../Debug/debug.h"
#include "rs232_printf.h"

#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "SEGGER_RTT.h"
#include <stdio.h>
volatile int _Cnt;
volatile int _Delay;
static char r;
extern uint8_t boardType;
extern uint8_t pinNormaState;
extern uint8_t pinMkState;
extern unsigned char zvon3_raw[];
extern unsigned int zvon3_raw_len;
extern uint8_t TLC59116F_max_address;

extern lfs_t lfs;
extern lfs_file_t file;
extern uint32_t lastTimePressed;

#ifdef __cplusplus
extern "C" {
#endif

extern SAI_HandleTypeDef audioTxSai;
extern osSemaphoreId Netif_LinkSemaphore;

void SeggerTest(void) {

  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

  SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");
  SEGGER_RTT_WriteString(0, "###### Testing SEGGER_printf() ######\r\n");

  SEGGER_RTT_printf(0, "printf Test: %%c,         'S' : %c.\r\n", 'S');
  SEGGER_RTT_printf(0, "printf Test: %%5c,        'E' : %5c.\r\n", 'E');
  SEGGER_RTT_printf(0, "printf Test: %%-5c,       'G' : %-5c.\r\n", 'G');
  SEGGER_RTT_printf(0, "printf Test: %%5.3c,      'G' : %-5c.\r\n", 'G');
  SEGGER_RTT_printf(0, "printf Test: %%.3c,       'E' : %-5c.\r\n", 'E');
  SEGGER_RTT_printf(0, "printf Test: %%c,         'R' : %c.\r\n", 'R');

  SEGGER_RTT_printf(0, "printf Test: %%s,      \"RTT\" : %s.\r\n", "RTT");
  SEGGER_RTT_printf(0, "printf Test: %%s, \"RTT\\r\\nRocks.\" : %s.\r\n", "RTT\r\nRocks.");

  SEGGER_RTT_printf(0, "printf Test: %%u,       12345 : %u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%+u,      12345 : %+u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%.3u,     12345 : %.3u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%.6u,     12345 : %.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%6.3u,    12345 : %6.3u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%8.6u,    12345 : %8.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%08u,     12345 : %08u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%08.6u,   12345 : %08.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%0u,      12345 : %0u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-.6u,    12345 : %-.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-6.3u,   12345 : %-6.3u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-8.6u,   12345 : %-8.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08u,    12345 : %-08u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08.6u,  12345 : %-08.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-0u,     12345 : %-0u.\r\n", 12345);

  SEGGER_RTT_printf(0, "printf Test: %%u,      -12345 : %u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%+u,     -12345 : %+u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%.3u,    -12345 : %.3u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%.6u,    -12345 : %.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%6.3u,   -12345 : %6.3u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%8.6u,   -12345 : %8.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%08u,    -12345 : %08u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%08.6u,  -12345 : %08.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%0u,     -12345 : %0u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-.6u,   -12345 : %-.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-6.3u,  -12345 : %-6.3u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-8.6u,  -12345 : %-8.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08u,   -12345 : %-08u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08.6u, -12345 : %-08.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-0u,    -12345 : %-0u.\r\n", -12345);

  SEGGER_RTT_printf(0, "printf Test: %%d,      -12345 : %d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%+d,     -12345 : %+d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%.3d,    -12345 : %.3d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%.6d,    -12345 : %.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%6.3d,   -12345 : %6.3d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%8.6d,   -12345 : %8.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%08d,    -12345 : %08d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%08.6d,  -12345 : %08.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%0d,     -12345 : %0d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-.6d,   -12345 : %-.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-6.3d,  -12345 : %-6.3d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-8.6d,  -12345 : %-8.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08d,   -12345 : %-08d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08.6d, -12345 : %-08.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-0d,    -12345 : %-0d.\r\n", -12345);

  SEGGER_RTT_printf(0, "printf Test: %%x,      0x1234ABC : %x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%+x,     0x1234ABC : %+x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%.3x,    0x1234ABC : %.3x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%.6x,    0x1234ABC : %.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%6.3x,   0x1234ABC : %6.3x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%8.6x,   0x1234ABC : %8.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%08x,    0x1234ABC : %08x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%08.6x,  0x1234ABC : %08.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%0x,     0x1234ABC : %0x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-.6x,   0x1234ABC : %-.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-6.3x,  0x1234ABC : %-6.3x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-8.6x,  0x1234ABC : %-8.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-08x,   0x1234ABC : %-08x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-08.6x, 0x1234ABC : %-08.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-0x,    0x1234ABC : %-0x.\r\n", 0x1234ABC);

  SEGGER_RTT_printf(0, "printf Test: %%p,      &_Cnt      : %p.\r\n", &_Cnt);

  SEGGER_RTT_WriteString(0, "###### SEGGER_printf() Tests done. ######\r\n");
//  do
//  {
    _Cnt++;
//  }
//  while (1);
}
void SeggerTest2(void)
{
    SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n");
    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    do {
      r = SEGGER_RTT_WaitKey();
      SEGGER_RTT_Write(0, &r, 1);
      r++;
    } while (1);
}


void testLed1()
{
    osThreadDef(simpleLedTest1_RTOS, simpleLedTest1_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(simpleLedTest1_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpleLedTest1_RTOS");
    }
}
void testLed2()
{
    osThreadDef(simpleLedTest2_RTOS, simpleLedTest2_RTOS, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE *5 );
    if ((osThreadCreate(osThread(simpleLedTest2_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpleLedTest2_RTOS");
    }
}
void testLed3()
{
//    osDelay (8000);
//    osSemaphoreRelease(Netif_LinkSemaphore);

    osThreadDef(simpleLedTest3_RTOS, simpleLedTest3_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(simpleLedTest3_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpleLedTest3_RTOS");
    }
}
void testUART()
{
    osThreadDef(simpletestUART_RTOS, simpletestUART_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(simpletestUART_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpletestUART_RTOS");
    }
}

void pinNorma_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 500;
    uint32_t timeReset = 500;
    uint32_t timeSetFast = 100;
    uint32_t timeResetFast = 100;

    for(;;)
    {
        if ((boardType == sc2) && (pinNormaState == pinNormaBlink))
        {
            if(reset)
            {
                if (HAL_GetTick() > tickstart + timeReset)
                {
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
                     reset = false;
                     tickstart = HAL_GetTick();
                }
            }

            if(!reset)
            {
                if (HAL_GetTick() > tickstart + timeSet)
                {
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
                     reset = true;
                     tickstart = HAL_GetTick();
                }
            }
        }

        if ((boardType == sc2) && (pinNormaState == pinNormaBlinkFast))
        {
            if(reset)
            {
                if (HAL_GetTick() > tickstart + timeResetFast)
                {
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
                     reset = false;
                     tickstart = HAL_GetTick();
                }
            }

            if(!reset)
            {
                if (HAL_GetTick() > tickstart + timeSetFast)
                {
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
                     reset = true;
                     tickstart = HAL_GetTick();
                }
            }
        }

    if (((boardType == sc4) || (boardType == sl1))  && (pinNormaState == pinNormaBlink))
    {
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }

        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
    }

    if (((boardType == sc4) || (boardType == sl1)) && (pinNormaState == pinNormaBlinkFast))
    {
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeResetFast)
            {
                 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }

        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSetFast)
            {
                 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
    }

    if (((boardType == sc4) || (boardType == sl1)) && (pinNormaState == pinNormaReset))
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
    }

    if (((boardType == sc4) || (boardType == sl1)) && (pinNormaState == pinNormaSet))
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET); //Пин Норма
    }


    osDelay(1);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void pinMk_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 500;
    uint32_t timeReset = 500;
    uint32_t timeSetFast = 100;
    uint32_t timeResetFast = 100;

    for(;;)
    {
    if ((boardType == sc4)  && (pinMkState == pinMkBlink))
    {
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }

        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
    }

    if ((boardType == sc4)  && (pinMkState == pinMkBlinkFast))
    {
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeResetFast)
            {
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }

        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSetFast)
            {
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
    }

    if ((boardType == sc4)  && (pinMkState == pinMkReset))
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }

    if ((boardType == sc4)  && (pinMkState == pinMkSet))
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }

    osDelay(1);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void writeVolSensToFlash_RTOS(void const *argument)
{
    (void)argument;
    while (pinNormaState != pinMkSet)
    {
        osDelay(10);
    }
    int vol = -24;
    int sens = 4;
    lfs_file_open(&lfs, &file, "vol", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &vol, sizeof(vol));
    GPIO::getInstance()->dacDriverGainValue = vol;
    GPIO::getInstance()->changeVolumeMute();
    lfs_file_close(&lfs, &file);
    RS232::getInstance().term << "current vol = " << vol << "\r\n";

    lfs_file_open(&lfs, &file, "sens", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &sens, sizeof(sens));
    GPIO::getInstance()->dacDriverSensValue = sens;
    GPIO::getInstance()->changeSensMute();
    lfs_file_close(&lfs, &file);
    RS232::getInstance().term << "current sens = " << sens << "\r\n";
    RS232::getInstance().term << "vol min max = " << GPIO::getInstance()->dacDriverGainValueMin
                              << " "<< GPIO::getInstance()->dacDriverGainValueMax<<"\r\n";
    RS232::getInstance().term << "sense min max = " << GPIO::getInstance()->dacDriverSensValueMin
                              << " "<< GPIO::getInstance()->dacDriverSensValueMax<<"\r\n";

    for(;;)
    {
        if (lastTimePressed  + 10000 < HAL_GetTick()) //Проверяем раз в 10 секунд
        {
            lastTimePressed = HAL_GetTick();
            if (vol != GPIO::getInstance()->dacDriverGainValue)
            {
                vol = GPIO::getInstance()->dacDriverGainValue;
                lfs_file_open(&lfs, &file, "vol", LFS_O_WRONLY );
                lfs_file_write(&lfs, &file, &vol, sizeof(vol));
                lfs_file_close(&lfs, &file);
                RS232::getInstance().term << "vol stored " << vol << "\r\n";
            }
            if (sens != GPIO::getInstance()->dacDriverSensValue)
            {
                sens = GPIO::getInstance()->dacDriverSensValue;
                lfs_file_open(&lfs, &file, "sens", LFS_O_WRONLY );
                lfs_file_write(&lfs, &file, &sens, sizeof(sens));
                lfs_file_close(&lfs, &file);
                RS232::getInstance().term << "sens stored " << sens << "\r\n";
            }
        }

        osDelay(1);
    } //end for(;;)
    vTaskDelete(nullptr);
}


void pinNormaStart()
{
    osThreadDef(pinNormaRTOS, pinNorma_RTOS, osPriorityHigh, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(pinNormaRTOS), nullptr)) == nullptr)
    {
        term("Failed to create pinNorma_RTOS");
    }
}

void pinMkStart()
{
    osThreadDef(pinMkRTOS, pinMk_RTOS, osPriorityHigh, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(pinMkRTOS), nullptr)) == nullptr)
    {
        term("Failed to create pinMk_RTOS");
    }
}

void writeVolSensToFlashStart()
{
    osThreadDef(writeVolSensToFlash, writeVolSensToFlash_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(writeVolSensToFlash), nullptr)) == nullptr)
    {
        term("Failed to create writeVolSensToFlash_RTOS");
    }
}


//char *logTasks = new char[2048];
static char logTasks[2048];
//static char logTasksTime[2048];
void testTasksLog()
{
    osThreadDef(TasksLog, TasksLog, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(TasksLog), nullptr)) == nullptr)
    {
        term("Failed to create TasksLog");
    }
}

void simpleLedTest1_RTOS(void const *argument)
{
    osDelay(5000);
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 3000;

term("startingSimpleLedTest1_RTOS")

    for(;;)
    {
    if (boardType == sc2)
    {
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_SET);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }

        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
    }
        for (uint8_t i = 0; i < TLC59116F_max_address * 8; ++i)
        {
            GPIO::getInstance()->aLeds[i].ledState = true;
            osDelay(1500);
            GPIO::getInstance()->aLeds[i].ledState = false;
            osDelay(5);
        }

    osDelay(10);
    } //end for(;;)

    vTaskDelete(nullptr);
}
void simpleLedTest2_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 2000;

osDelay(50);
term("startingSimpleLedTest2_RTOS")

    for(;;)
    {
    if (boardType == sc2)
    {
            if(reset)
            {
                if (HAL_GetTick() > tickstart + timeReset)
                {
                     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
                     reset = false;
                     tickstart = HAL_GetTick();
                }
            }
            if(!reset)
            {
                if (HAL_GetTick() > tickstart + timeSet)
                {
                     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);
                     reset = true;
                     tickstart = HAL_GetTick();
                }
            }
    }

//    SEGGER_RTT_Write(0, "01234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n", 82);
        SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n");
//        SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
//        do {
//          r = SEGGER_RTT_WaitKey();
//          SEGGER_RTT_Write(0, &r, 1);
//          r++;
//        } while (1);
    osDelay(10);
    } //end for(;;)

    vTaskDelete(nullptr);
}
void simpleLedTest3_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 2000;

osDelay(100);
term("--- simpleLedTest3_RTOS ---")

    for(;;)
    {
//        osDelay (8000);
//        osSemaphoreRelease(Netif_LinkSemaphore);
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_SET);

                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }
        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

                 reset = true;
                 tickstart = HAL_GetTick();

//                 HAL_SAI_Transmit_DMA(&audioTxSai, zvon3_raw, zvon3_raw_len/2);
            }
        }
    osDelay(1);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void simpletestUART_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 3000;
    uint32_t timeReset = 3000;

osDelay(150);
term("startingSimpleTestUART_RTOS")

    for(;;)
    {

        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
//                 term("*************  SimpleTestUART_RTOS  *************")
//GPIO::getInstance()->aLeds[3].ledState = 1;
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }
        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
//                term("#############  SimpleTestUART_RTOS  #############")
                reset = true;
                tickstart = HAL_GetTick();
            }
        }
    osDelay(1);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void TasksLog(void const *argument)
{
    (void)argument;

    osDelay(6000);
    term("--- TasksLog ---")

        for(;;)
    {
        vTaskList(logTasks);
//        vTaskGetRunTimeStats(logTasksTime);
        term2(logTasks)
        term1("heap size") term(xPortGetFreeHeapSize())
        osDelay(30000);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void testXOR()
{
    char temp[100];
    std::string *stringOrigin = new std::string{
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
        "qwertyQWERTYUIOPASDFGHqwertyuiopasdfghQWERTYUIOPASDFGHqwertyuiopasdfgh123456789012345678901234567890endend11"
    };
    std::string keyTest = "108_!@#$%^&*()_+_108!@#$%^&*()_+42";
    const char* key = keyTest.c_str();
    const char* input = stringOrigin->c_str();
    uint32_t inputLength = stringOrigin->size();
    uint8_t keyLength = keyTest.size();
    char *encoded = new char[inputLength];
    char *decoded = new char[inputLength];
    for (int i =0; i<inputLength;++i)
    {
        encoded[i] = 0x00;
        decoded[i] = 0x00;
    }
    for (uint32_t i = 0; i < inputLength + 1; ++i)
    {
        encoded[i] = input[i] ^ key[i % keyLength + 1];
    }
    for (int i = 0; i < inputLength + 1; ++i)
    {
        decoded[i] = encoded[i] ^ key[i % keyLength + 1];
    }
    term2("test XOR")
    RS232::getInstance().term << "key for test = ";
    snprintf(temp, keyLength + 1,"%s", key); term2(temp)
    RS232::getInstance().term << "key lentgh = ";
    sprintf(temp, "%d", keyLength); term2(temp)
    RS232::getInstance().term << "input lentgh = ";
    sprintf(temp, "%d", inputLength); term2(temp)
//    sprintf(temp,"origin  text =  %s",stringOrigin->c_str()); term2(temp)
//    RS232::getInstance().term << "encoded text = ";
//    snprintf(temp, inputLength + 1, "%s",encoded); term2(temp)

//    RS232::getInstance().term << "decoded text = ";
//    snprintf(temp, inputLength + 1,"%s", decoded); term2(temp)

    if (strcmp(stringOrigin->c_str(), decoded) == 0)
    {
        term2("test XOR passed")
    }
    else
    {
        term2("test XOR failed")
    }
    delete stringOrigin;
    delete[]  encoded;
    delete[]  decoded;
}


#ifdef __cplusplus
}
#endif

//void test_UART()
//{
//    RS232::getInstance().test();
//}
