/*
Не блокирующая реализация I2C для микроконтроллеров STM32F030
Прерывания не используются
SPL используется только для выбора альтернативной функции
При желании можно настроить AF ручками и выкинуть SPL совсем
--------------------------
Автор Zlodey
masya-chel@mail.ru
masya-chel@yandex.ru
--------------------------
Март 2015г.
*/

#include "i2c.h"


/*
Выполняет первичную настройку шины.
Настраивает порты, альтернативные функции.
Настраивает тайминги для работы шины.
*/
void I2C_Initialization (void)
{
	// Тактирование GPIO не включаю, т.к. оно уже включено в начале программы
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;		// Включаю тактирование I2C
	// Настройка ног PB6, PB7
	GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;			// Режим альтернативной функции
	GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7;				// Открытый коллектор
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR6 | GPIO_OSPEEDR_OSPEEDR7;	// Максимальная скорость
	// Выбор альтернативной функции
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);	// I2C1_SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);	// I2C1_SDA
	
	I2C_BUS->CR1 &= ~I2C_CR1_PE;			// Отключаю I2C
	while (I2C_BUS->CR1 & I2C_CR1_PE) {};	// Жду пока отключится
	
	// Частота тактирования модуля I2C = 48 МГц
	// Частота шины I2C = 100 kHz
	// Настраиваю тайминги
	I2C_BUS->TIMINGR |= \
	(0xB  << I2C_OFFSET_TIMINGR_PRESC)|\
	(0x13 << I2C_OFFSET_TIMINGR_SCLL)|\
	(0xF  << I2C_OFFSET_TIMINGR_SCLH)|\
	(0x4  << I2C_OFFSET_TIMINGR_SCLDEL)|\
	(0x2  << I2C_OFFSET_TIMINGR_SDADEL);
	
	I2C_BUS->CR1 |= I2C_CR1_PE;					// Включаю I2C
	while ((I2C_BUS->CR1 & I2C_CR1_PE)==0) {};	// Жду пока включится
}


/*
Это служебная функция, использовать её не нужно.
Устанавливает направление данных - приём или передача.
Задаёт объём пересылаемых данных.
Задаёт адрес ведомого устройства.
Выдаёт старт на шину.
Параметры:
Direction - направление (0-отправка, 1-приём)
Adress - адрес ведомого устройства
Size - размер данных (от 1 до 255 байт)
*/
void I2C_Start_Direction_Adress_Size (I2C_Direction Direction, u8 Adress, u8 Size)
{
	//I2C_BUS->CR2 &= ~I2C_CR2_AUTOEND;				// Выдавать стоп вручную
	//I2C_BUS->CR2 &= ~I2C_CR2_RELOAD;				// Не использовать режим перезагрузки
	if (Direction) I2C_BUS->CR2 |= I2C_CR2_RD_WRN;	// Режим приёма
	else I2C_BUS->CR2 &= ~I2C_CR2_RD_WRN;			// Режим передачи
	I2C_BUS->CR2 &= ~I2C_CR2_NBYTES;				// Очистить размер данных
	I2C_BUS->CR2 |= Size<<I2C_OFFSET_CR2_NBYTES;	// Установить размер данных
	I2C_BUS->CR2 &= ~I2C_CR2_SADD;	// Очистить адрес ведомого устройства
	I2C_BUS->CR2 |= Adress;			// Установить адрес ведомого устройства
	I2C_BUS->CR2 |= I2C_CR2_START;					// Выдать старт на шину
	while ((I2C_BUS->ISR & I2C_ISR_BUSY)==0) {};	// Ожидать выдачу старта
}


/*
Это служебная функция, использовать её не нужно.
Выдаёт стоп на шину.
Очищает флаги.
Проверяет наличие ошибок, очищает флаги ошибок.
*/
void I2C_Stop (void)
{
	I2C_BUS->CR2 |= I2C_CR2_STOP;				// Выдать стоп на шину
	while (I2C_BUS->ISR & I2C_ISR_BUSY) {};		// Ожидать выдачу стопа
	// Очищаю флаги - необходимо для дальнейшей работы шины
	I2C_BUS->ICR |= I2C_ICR_STOPCF;		// STOP флаг
	I2C_BUS->ICR |= I2C_ICR_NACKCF;		// NACK флаг
	// Если есть ошибки на шине - очищаю флаги
	if (I2C_BUS->ISR & (I2C_ISR_ARLO | I2C_ISR_BERR))
	{
		I2C_BUS->ICR |= I2C_ICR_ARLOCF;
		I2C_BUS->ICR |= I2C_ICR_BERRCF;
	}
}


/*
Выполняет транзакцию записи Size байт в регистр Register по адресу Adress.
Параметры:
Adress - адрес ведомого устройства
Register - регистр, в который хотим передать данные
Data - указывает откуда брать данные для передачи
Size - сколько байт хотим передать (от 1 до 254)
Возвращает:
1 - если данные успешно переданы
0 - если произошла ошибка
*/
u8 I2C_Write_Transaction (u8 Adress, u8 Register, u8 *Data, u8 Size)
{
	u8 Count=0;	// Счётчик успешно переданных байт
	// Старт
	I2C_Start_Direction_Adress_Size (I2C_Transmitter, Adress, 1+Size);
	// Сейчас либо I2C запросит первый байт для отправки,
	// Либо взлетит NACK-флаг, говорящий о том, что микросхема не отвечает.
	// Если взлетит NACK-флаг, отправку прекращаем.
	while ((((I2C_BUS->ISR & I2C_ISR_TXIS)==0) && ((I2C_BUS->ISR & I2C_ISR_NACKF)==0)) && (I2C_BUS->ISR & I2C_ISR_BUSY)) {};
	if (I2C_BUS->ISR & I2C_ISR_TXIS) I2C_BUS->TXDR=Register;	// Отправляю адрес регистра
	
	// Отправляем байты до тех пор, пока не взлетит TC-флаг.
	// Если взлетит NACK-флаг, отправку прекращаем.
	while ((((I2C_BUS->ISR & I2C_ISR_TC)==0) && ((I2C_BUS->ISR & I2C_ISR_NACKF)==0)) && (I2C_BUS->ISR & I2C_ISR_BUSY))
	{
		if (I2C_BUS->ISR & I2C_ISR_TXIS) I2C_BUS->TXDR=*(Data+Count++);	// Отправляю данные
	}
	I2C_Stop();
	if (Count == Size) return 1; return 0;
}


/*
Выполняет транзакцию чтения Size байт из регистра Register по адресу Adress.
Параметры:
Adress - адрес ведомого устройства
Register - регистр, из которого хотим принять данные
Data - указывает куда складывать принятые данные
Size - сколько байт хотим принять (от 1 до 255)
Возвращает:
1 - если данные успешно приняты
0 - если произошла ошибка
*/
u8 I2C_Read_Transaction (u8 Adress, u8 Register, u8 *Data, u8 Size)
{
	u8 Count=0;	// Счётчик успешно принятых байт
	// Старт
	I2C_Start_Direction_Adress_Size (I2C_Transmitter, Adress, 1);
	// Сейчас либо I2C запросит первый байт для отправки,
	// Либо взлетит NACK-флаг, говорящий о том, что микросхема не отвечает.
	// Если взлетит NACK-флаг, отправку прекращаем.
	while ((((I2C_BUS->ISR & I2C_ISR_TC)==0) && ((I2C_BUS->ISR & I2C_ISR_NACKF)==0)) && (I2C_BUS->ISR & I2C_ISR_BUSY))
	{
		if (I2C_BUS->ISR & I2C_ISR_TXIS) I2C_BUS->TXDR = Register;	// Отправляю адрес регистра
	}
	// Повторный старт
	I2C_Start_Direction_Adress_Size (I2C_Receiver, Adress, Size);
	// Принимаем байты до тех пор, пока не взлетит TC-флаг.
	// Если взлетит NACK-флаг, приём прекращаем.
	while ((((I2C_BUS->ISR & I2C_ISR_TC)==0) && ((I2C_BUS->ISR & I2C_ISR_NACKF)==0)) && (I2C_BUS->ISR & I2C_ISR_BUSY))
	{
		if (I2C_BUS->ISR & I2C_ISR_RXNE) *(Data+Count++) = I2C_BUS->RXDR;	// Принимаю данные
	}
	I2C_Stop();
	if (Count == Size) return 1; return 0;
}

