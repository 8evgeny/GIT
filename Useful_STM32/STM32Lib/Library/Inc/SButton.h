#ifndef _SBUTTON_H
#define _SBUTTON_H

/*
	SButton - библиотека для многофункциональной отработки нажатия кнопки.
	За основу взята библиотека Гайвера с сайта: https://alexgyver.ru/gyverbutton/

	Возможности:
	- Работа с нормально замкнутыми и нормально разомкнутыми кнопками
	- Работа с подключением PULL_UP и PULL_DOWN
	- Опрос кнопки с программным антидребезгом контактов (настраиваемое время)	
	- Отработка нажатия, удерживания, отпускания, клика по кнопке (+ настройка таймаутов)	
	- Отработка одиночного, двойного и тройного нажатия (вынесено отдельно)
	- Отработка любого количества нажатий кнопки (функция возвращает количество нажатий)	
	- Функция изменения значения переменной с заданным шагом и заданным интервалом по времени
	- Возможность работы с "виртуальными" кнопками (все возможности библиотеки используются для матричных и резистивных клавиатур)
	- !!! В отличие от оригинальной библиотеки, автоопрос кнопки включён по умолчанию, так как режим опроса вручную
	  используется редко.
	
	Документацию на оригинальную библиотеку читай здесь: https://alexgyver.ru/gyverbutton/
		
 
    v3.7: исправления от Dryundel: 
    - Любой таймаут удержания
    - Single, Double и Triple теперь не мешают hasClicks и getClicks и работают совместно
    - isStep() тоже теперь ничего не мешает и он работает более корректно
    v3.8: исправления от Dryundel:
*/

#include "STM32.h"
#include <gpio_main.h>

//#pragma pack(push,1)
typedef struct {		
	bool btn_deb      : 1;
	bool hold_flag    : 1;
	bool counter_flag : 1;
	bool isHolded_f   : 1;
	bool isRelease_f  : 1;
	bool isPress_f    : 1;
	bool step_flag    : 1;
	bool oneClick_f   : 1;
	bool isOne_f      : 1;
	bool inv_state    : 1;
	bool mode         : 1;
	bool type         : 1;
	bool tickMode     : 1;
	bool noPin        : 1;
	bool counter_reset: 1;
} SButtonFlags;
//#pragma pack(pop)

#define BTN_NO_PIN  -1
#define HIGH_PULL   0
#define LOW_PULL    1
#define NORM_OPEN   0
#define NORM_CLOSE  1
#define MANUAL      0
#define AUTO        1

// Инициализация:
// SButton btn(GPIOx_Port, GPIO_Pin, тип подключ., тип кнопки);

class SButton
{
  public:
    SButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, bool type, bool dir);       // конструктор кнопки, принимает порт, пин,
    // тип type (HIGH_PULL / LOW_PULL)
    // HIGH_PULL (0) - кнопка подключена к GND, пин подтянут к VCC
    // LOW_PULL  (1) - кнопка подключена к VCC,  пин подтянут к GND
    //
    // и направление dir (NORM_OPEN / NORM_CLOSE)
		// NORM_OPEN  (0) - кнопка по умолчанию разомкнута
		// NORM_CLOSE (1)- кнопка по умолчанию замкнута
	
    void setDebounce(uint16_t debounce);				                                // установка времени антидребезга (по умолчанию 80 мс)
    void setTimeout(uint16_t new_timeout);				                              // установка таймаута удержания (по умолчанию 300 мс)
    void setClickTimeout(uint16_t new_timeout);			                            // установка таймаута между кликами (по умолчанию 500 мс)
    void setStepTimeout(uint16_t step_timeout);			                            // установка таймаута между инкрементами (по умолчанию 400 мс)
    void setDirection(bool dir);						                                    // установка направления (разомкнута/замкнута)
	
    void setTickMode(bool tickMode);					                                  // (MANUAL / AUTO) ручной или автоматический опрос кнопки функцией tick()
														                                                    // MANUAL - нужно вызывать функцию tick() вручную
														                                                    // AUTO - tick() входит во все остальные функции и опрашивается сама
	
    void tick();										                                            // опрос кнопки
    void tick(bool state);							                                        // опрос внешнего значения (0 нажато, 1 не нажато) (для матричных, резистивных клавиатур и джойстиков)
	
    bool isPress();		                                                          // возвращает true при нажатии на кнопку. Сбрасывается после вызова
    bool isRelease();	                                                          // возвращает true при отпускании кнопки. Сбрасывается после вызова
    bool isClick();		                                                          // возвращает true при клике. Сбрасывается после вызова
    bool isHolded();		                                                        // возвращает true при удержании дольше timeout. Сбрасывается после вызова
    bool isHold();		                                                          // возвращает true при нажатой кнопке, не сбрасывается
    bool state();		                                                            // возвращает состояние кнопки

    bool isSingle();		                                                        // возвращает true при одиночном клике. Сбрасывается после вызова
    bool isDouble();		                                                        // возвращает true при двойном клике. Сбрасывается после вызова
    bool isTriple();		                                                        // возвращает true при тройном клике. Сбрасывается после вызова
	
    bool hasClicks();	                                                          // проверка на наличие кликов. Сбрасывается после вызова
    uint8_t getClicks();	                                                      // вернуть количество кликов
    uint8_t getHoldClicks();                                                    // вернуть количество кликов, предшествующее удерживанию
	
    bool isStep(uint8_t clicks = 0);                                            // возвращает true по таймеру setStepTimeout, смотри пример
	
    void resetStates();		                                                      // сбрасывает все is-флаги и счётчики
	
  private:
    GPIO_TypeDef *_GPIOx_IN;
    uint16_t      _GPIO_Pin_IN;
    SButtonFlags  flags;
    uint16_t      _debounce = 60;
    uint16_t      _timeout = 500;
    uint16_t      _click_timeout = 500;
    uint16_t      _step_timeout = 400;
    uint8_t       btn_counter = 0, last_counter = 0, last_hold_counter = 0;
    uint32_t      btn_timer = 0;
    bool          btn_state = false;
    bool          btn_flag = false;
};
#endif /* _H */
