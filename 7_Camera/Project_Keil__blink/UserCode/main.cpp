#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_iwdg.h"
//---------------------------------
#include "MDR32F9Qx_config.h"
//#include "MDR32F9Qx_usb_handlers.h"
//---------------------------------
#include "main.h"

#define delay(T) for(i = T; i > 0; i--) // Определение функции задержки (см. примечение 1)
// Объявление структуры, с помощью которой будет происходить инициализация порта
PORT_InitTypeDef PORT__Init;
int i; // Глобальная переменная счетчика, которая используется в функции delay()

int main (int argc, char** argv)
{
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);

    PORT__Init.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 ;
    PORT__Init.PORT_OE = PORT_OE_OUT;           // Конфигурация группы выводов как выход
    PORT__Init.PORT_FUNC = PORT_FUNC_PORT;      // Работа а режиме порта ввода-вывода
    PORT__Init.PORT_MODE = PORT_MODE_DIGITAL;   // Цифровой режим
    PORT__Init.PORT_SPEED = PORT_SPEED_SLOW;    // Низкая частота тактования порта
    PORT_Init(MDR_PORTC, &PORT__Init);          // Инициализация порта C объявленной структурой
    
    PORT__Init.PORT_Pin = PORT_Pin_10 | PORT_Pin_9 ;
    PORT__Init.PORT_OE = PORT_OE_OUT;
    PORT__Init.PORT_FUNC = PORT_FUNC_PORT;
    PORT__Init.PORT_MODE = PORT_MODE_DIGITAL;
    PORT__Init.PORT_SPEED = PORT_SPEED_SLOW;
    PORT_Init(MDR_PORTB, &PORT__Init);

    while(1)
    {
            PORT_SetBits(MDR_PORTC, PORT_Pin_0);
            PORT_SetBits(MDR_PORTC, PORT_Pin_1);
            PORT_SetBits(MDR_PORTB, PORT_Pin_9);
            PORT_SetBits(MDR_PORTB, PORT_Pin_10);
            delay(0x3FFFF);

            PORT_ResetBits(MDR_PORTC, PORT_Pin_0);
            PORT_ResetBits(MDR_PORTC, PORT_Pin_1);
            PORT_ResetBits(MDR_PORTB, PORT_Pin_9);
            PORT_ResetBits(MDR_PORTB, PORT_Pin_10);
            delay(0x3FFFF);
    }

}
#if 0
Описание структуры, управляющей портом
typedef struct
{
    uint16_t PORT_Pin;                 Определяет какие ножки порта будут сконфигурированы
                                       например значение поля в двоичном коде 0b0000 0001 0000 0010
                                       конфигурирует 8 и 1 бит. Для задания значений можно
                                       воспользоваться стандартными битовыми масками PORT_Pin_x,
                                       которые записываются через побитовое ИЛИ: PORT_Pin_1|PORT_Pin_8.

PORT_OE_TypeDef PORT_OE;               Определяет режим работы порта: ввод или вывод.
                                       ввод                                PORT_OE_IN         (0)
                                       вывод                               PORT_OE_OUT -      (1)

PORT_PULL_UP_TypeDef PORT_PULL_UP;     Определяет включение верхнего подтягивающего резистора
                                       выключен                            PORT_PULL_UP_OFF   (0)
                                       включен                             PORT_PULL_UP_ON    (1)

PORT_PULL_DOWN_TypeDef PORT_PULL_DOWN; Определяет включение нижнего подтягивающего резистора
                                       выключен                            PORT_PULL_DOWN_OFF (0)
                                       включен                             PORT_PULL_DOWN_OFF (1)

PORT_PD_SHM_TypeDef PORT_PD_SHM; Определяет включение или выключение триггера Шмидта
                                 выключен  (гистерезис 200мВ)              PORT_PD_SHM_OFF    (0)
                                 включен   (гистерезис 400мВ)              PORT_PD_SHM_ON     (1)

PORT_PD_TypeDef   PORT_PD;       Определяет режим работы ножки.
                                 Управляемый драйвер                       PORT_PD_DRIVER     (0)
                                 Открытый сток                             PORT_PD_OPEN       (1)

PORT_GFEN_TypeDef PORT_GFEN;     Определяет режим работы входного фильтра ножки.
                                 Выключен                                  PORT_GFEN_OFF      (0)
                                 Включен                                   PORT_GFEN_ON       (1)

PORT_FUNC_TypeDef PORT_FUNC;     Определяет режим работы вывода порта:
                                 - Порт                                    PORT_FUNC_PORT     (0)
                                 - Основная функция                        PORT_FUNC_MAIN     (1)
                                 - Альтернативная функция                  PORT_FUNC_ALTER    (2)
                                 - Переопределенная функция                PORT_FUNC_OVERRID  (3)

PORT_SPEED_TypeDef PORT_SPEED;   Определяет скорость работы порта:
                                 зарезервировано (передатчик отключен)     PORT_OUTPUT_OFF    (0)
                                 медленный фронт (порядка 100 нс)          PORT_SPEED_SLOW    (1)
                                 быстрый фронт (порядка 20 нс)             PORT_SPEED_FAST    (2)
                                 максимально быстрый фронт (порядка 10 нс) PORT_SPEED_MAXFAST (3)

PORT_MODE_TypeDef PORT_MODE;     Определяет режим работы контроллера:
                                 аналоговый                                PORT_MODE_ANALOG  = 0x0
                                 цифровой                                  PORT_MODE_DIGITAL = 0x1
}PORT_InitTypeDef;
#endif
