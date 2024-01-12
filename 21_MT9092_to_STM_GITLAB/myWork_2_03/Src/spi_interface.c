/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   spi_interface.c
 * Author: roman.ganaza
 * 
 * Created on October 21, 2020, 3:04 PM
 */

#include <stdio.h>
#include <string.h>

#include "spi_interface.h"
#include "main.h"
#include "uart.h"
#include "mt9092_logic.h"
#include "mt9092_registers.h"

#define DBG_SPI_TO_UART
//#define DBG_READ_SPI_FROM_MT9092 // чтение по SPI4 ответа на команду чтения регистра от MT9092
 // фильтруем чтение C-channel (0x29 >> 1 = 0x14)
//#define DBG_SPI_FILTER	(spi_in_buf[0] != ((C_CHANNEL_REGISTER << 1) | 0x01))
#define DBG_SPI_FILTER	(((spi_in_buf[0] >> 1) != HDLC_STATUS_REGISTER)&&((spi_in_buf[0] >> 1) != HDLC_TX_RX_FIFO)&&(spi_in_buf[0] != ((C_CHANNEL_REGISTER << 1) | 0x01))&&\
				((spi_in_buf[0] >> 1) != HDLC_CONTROL_REGISTER_1)&& ((spi_in_buf[0] >> 1) != HDLC_INTERRUPT_STATUS_REGISTER))
//#define DBG_SPI_FILTER	((spi_in_buf[0] != ((C_CHANNEL_REGISTER << 1) | 0x01))&&\
//				((spi_in_buf[0] >> 1) != HDLC_INTERRUPT_STATUS_REGISTER))

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi4;
uint8_t spi_in_buf[SPI_BUF_SIZE];
uint8_t dbg_spi_in_buf[SPI_BUF_SIZE];
uint8_t spi_out_buf[SPI_BUF_SIZE];
uint8_t spi_dummy_out[2] = {0x00}; // только для старта механизма SPI
uint8_t spi_dummy_in[2]; // только для старта механизма SPI
uint8_t spi_dummy_out2[2] = {0x00}; // только для старта механизма SPI
uint8_t spi_dummy_in2[2]; // только для старта механизма SPI
static char str[64];
uint8_t cs_timeout = 0;
uint8_t flag_spi_read = 0;
uint32_t spi_led_timer = 0;
uint32_t err_led_timer = 0;

uint8_t flag_spi_restart = 0;
uint32_t spi_restart_timer = 0;

enum {REC_IDLE = 0, REC_ADDR_COMM, REC_DATA, REC_MORE_THEN_TWO_BYTES} spi_rec_state;

// prototypes
void SPI_2linesRxISR_8BIT_Fast(struct __SPI_HandleTypeDef *hspi); // прием по SPI2 от Mega128
void SPI_2linesTxISR_8BIT_Fast(struct __SPI_HandleTypeDef *hspi);
void DBG_SPI_RxISR_8BIT_Fast(struct __SPI_HandleTypeDef *hspi); // прием по SPI4 от MT9092
void spiLedRefresh(void);
void spiSetErrLed(void);

// functions
void spi_enable(void) {
	CLEAR_BIT((&hspi2)->Instance->CR1, SPI_CR1_CPHA); // change SPI CLK phase on default
	
	spi_dummy_in[0] = *((__IO uint8_t *) & hspi2.Instance->DR); // считываем на всякий случай, чтобы не случилось сразу прерывание
	spi_dummy_in[0] = *((__IO uint8_t *) & hspi2.Instance->DR);
	__HAL_SPI_ENABLE(&hspi2);
	
	spi_rec_state = REC_ADDR_COMM; // взводим механизм приема
	flag_spi_read = 0;
}
void spi_disable(void) {
	spi_rec_state = REC_IDLE;
	__HAL_SPI_DISABLE(&hspi2);
}

void spi_interface_Init(void) {
	 // используем HAL_SPI_TransmitReceive_IT просто для запуска непрерывного процесса приема/передачи,
	 // dummy буфера используются при мгновенном срабатывании прерывания после включения до назначения
	 // своих обработчиков ISR
	HAL_SPI_TransmitReceive_IT(&hspi2, spi_dummy_out, spi_dummy_in, 2);
	
	hspi2.RxISR = SPI_2linesRxISR_8BIT_Fast; // заменяем стандартный обработчик Rx ISR своим
	hspi2.TxISR = SPI_2linesTxISR_8BIT_Fast; // заменяем стандартный обработчик Tx ISR своим
	__HAL_SPI_DISABLE_IT(&hspi2, SPI_IT_TXE); // прерывание по освобождению Tx буфера нам не нужно

#ifdef DBG_READ_SPI_FROM_MT9092
	HAL_SPI_TransmitReceive_IT(&hspi4, spi_dummy_out2, spi_dummy_in2, 2);

	hspi4.RxISR = DBG_SPI_RxISR_8BIT_Fast; // заменяем стандартный обработчик Rx ISR своим
#endif
	
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  
	if (HAL_GPIO_ReadPin(SPI2_nCS_GPIO_Port, SPI2_nCS_Pin) == GPIO_PIN_RESET) {
		spi_rec_state = REC_ADDR_COMM;
		cs_timeout = CS_TIMEOUT;
	} else {
		spi_rec_state = REC_IDLE;
		__HAL_SPI_DISABLE(&hspi2);
		__HAL_SPI_DISABLE(&hspi4);
	}

}

void spi_begin_restart(void) { // стартует сброс всей системы SPI
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	spi_disable();
	flag_spi_restart = 1;
	spi_restart_timer = SPI_RESTART_DELAY;
    uartPuts("SPI restart begins...\r\n");
}


void spi_end_restart(void) { // заканчивает сброс всей системы SPI, включает SPI по-новой
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    uartPuts("SPI restart ends.\r\n");
	flag_spi_restart = 0;
}

void SPI_2linesRxISR_8BIT_Fast(struct __SPI_HandleTypeDef *hspi) {
	static unsigned char reg_addr;
	
	/* Receive data in 8bit mode */
	spiLedRefresh();
	
	switch (spi_rec_state) {
		
		case REC_ADDR_COMM:
			spi_in_buf[0] = *((__IO uint8_t *) & hspi->Instance->DR);
// if (spi_in_buf[0] != 0x29)
//     uartPutchar(spi_in_buf[0]);
			reg_addr = spi_in_buf[0] >> 1;

            if (spi_in_buf[0] & 0x01) { // на чтение 19 стр Даташит 9092
				// у нас прием идет по спаду, а передача по нарастанию, поэтому приходится
				// менять фазу в случае запроса на чтение от Меги
				__HAL_SPI_DISABLE(&hspi2);
				SET_BIT(hspi->Instance->CR1, SPI_CR1_CPHA); // change SPI CLK phase
				__HAL_SPI_ENABLE(&hspi2);
								
				 if (mt9092ProcessReadCommand(reg_addr, &spi_out_buf[1]) != 0) { // обрабатываем команду на чтение - формируем ответ
					 spi_begin_restart(); // если неуспешно, то сбрасываем
				 }
				
				*(__IO uint8_t *)&hspi->Instance->DR = spi_out_buf[1]; // загружаем в передающий буфер ответ
				
	#ifdef DBG_SPI_TO_UART
				if (DBG_SPI_FILTER) { //не выводим в UART лишнее
					flag_spi_read = 1;
					strcpy(str, "R: 0x");
					HexToChar(&str[5], spi_in_buf[0] >> 1);
					str[7] = ' ';
					strcpy(&str[8], "A: 0x");
					HexToChar(&str[13], spi_out_buf[1]);
		#ifdef DBG_READ_SPI_FROM_MT9092
					str[15] = ' ';
		#else			
					str[15] = '\r';
		#endif
					str[16] = '\0';
					uartPuts(str);
                    uartPuts("\r\n");
				} else {
					//HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
				}
	#endif
	#ifdef DBG_READ_SPI_FROM_MT9092
				// далее SPI для временного чтения c mt9092
				spi_dummy_in2[0] = *((__IO uint8_t *) & hspi4.Instance->DR); // читаем, чтобы не случилось сразу же прервание
				spi_dummy_in2[0] = *((__IO uint8_t *) & hspi4.Instance->DR); // второй раз для верности
				__HAL_SPI_ENABLE(&hspi4);
	#endif
				
			} else { // на запись
				*(__IO uint8_t *)&hspi->Instance->DR = 0; // загружаем в передающий буфер нуль
	#ifdef DBG_SPI_TO_UART
				if (DBG_SPI_FILTER) { //не выводим в UART лишнее
					strcpy(str, "W: 0x");
					HexToChar(&str[5], spi_in_buf[0] >> 1);
					str[7] = ' ';
					str[8] = '\0';
					uartPuts(str);
//                    uartPuts("\r\n");
				}
	#endif
			}
			
			spi_rec_state = REC_DATA;
			break;
			
		case REC_DATA:
			spi_in_buf[1] = *((__IO uint8_t *) & hspi->Instance->DR);
			
			// возвращаем фазу в исходное состояние
			__HAL_SPI_DISABLE(&hspi2);
			CLEAR_BIT(hspi->Instance->CR1, SPI_CR1_CPHA); // change SPI CLK phase
			__HAL_SPI_ENABLE(&hspi2);
			
			*(__IO uint8_t *)&hspi->Instance->DR = spi_out_buf[0]; // загружаем в передающий буфер

				if ((spi_in_buf[0] & 0x01)) { // чтение
					// ничего не делаем, т.к. при чтении 2-й байт пустой
					// при чтении шлем в UART то, что мы отсылем по SPI ("A:"), а также
					// будет выдан байт принятый от MT9092 в приеме DBG_SPI ("M:")
				} else { // запись  добавляем к W: 0xYY 
#ifdef DBG_SPI_TO_UART
					if (DBG_SPI_FILTER) { // не выводим в UART лишнее
						str[0] = '0';
						str[1] = 'x';
						HexToChar(&str[2], spi_in_buf[1]);
						str[4] = '\r';
						str[5] = '\0';
						uartPuts(str);
                        uartPuts("\r\n");
					}
#endif
					if (mt9092ProcessWriteCommand(reg_addr, spi_in_buf[1]) != 0) { // обрабатывем команду на запись
						spi_begin_restart(); // если неуспешно, то сбрасываем
					}
				}
			spi_rec_state = REC_MORE_THEN_TWO_BYTES;
			break;
			
		case REC_MORE_THEN_TWO_BYTES:
			spiSetErrLed();
			spi_dummy_in[0] = *((__IO uint8_t *) & hspi->Instance->DR);
			uartPuts("Error: REC_MORE_THEN_TWO_BYTES. Last byte:");
			HexToChar(&str[0], spi_dummy_in[0]);
			uartPuts(str);
            uartPuts("\r\n");
			break;
	}
}

void SPI_2linesTxISR_8BIT_Fast(struct __SPI_HandleTypeDef *hspi) {
	// не должны сюда попадать
	*(__IO uint8_t *)&hspi->Instance->DR = 0;
}

void DBG_SPI_RxISR_8BIT_Fast(struct __SPI_HandleTypeDef *hspi) { // прием по SPI4 от MT9092
	/* Receive data in 8bit mode */
//	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

	dbg_spi_in_buf[0] = *((__IO uint8_t *) & hspi->Instance->DR);
	__HAL_SPI_DISABLE(&hspi4);
	
	if (flag_spi_read) {
		flag_spi_read = 0;
#ifdef DBG_SPI_TO_UART
		strcpy(str, "M: 0x");
		HexToChar(&str[5], dbg_spi_in_buf[0]);
		str[7] = ' ';
		str[8] = '\r';
		str[9] = '\0';
		uartPuts(str);
        uartPuts("\r\n");
#endif
	}

//	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
}

void nCS_IRQHandler(void) {
	__HAL_GPIO_EXTI_CLEAR_IT(SPI2_nCS_Pin);
	
	if (!(SPI2_nCS_GPIO_Port->IDR & SPI2_nCS_Pin)) { // CS On
		spi_enable();	
		cs_timeout = CS_TIMEOUT; // запускаем отсчёт таймаута
	} else { // CS Off
		spi_disable();
		cs_timeout = 0; // выключаем отсчёт таймаута
	}	
}

void doCsTimeout(void) {
	if (cs_timeout) {
		cs_timeout--;
		if (cs_timeout == 0) {
			HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); // чтобы не было коллизии
			if (HAL_GPIO_ReadPin(SPI2_nCS_GPIO_Port, SPI2_nCS_Pin) == GPIO_PIN_RESET) { // если CS всё ещё активен
				uartPuts("Error: nCS Timeout! ");
                uartPuts("Restart SPI.\r\n");
				// пересбрасываем SPI
				spi_disable();
				spi_enable();
			}
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); // включаем обратно
		}
	}
	
	if (spi_restart_timer) {
		spi_restart_timer--;
		
		if (spi_restart_timer == 0) {
			spi_end_restart();
		}
	}
}

/**
 * @brief  SPI error callback.
 * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @retval None
 */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
  sprintf(str, "SPI error: %u\r\n", hspi->ErrorCode);
  uartPuts(str);

  spiSetErrLed();
	// используем HAL_SPI_TransmitReceive_IT просто для запуска непрерывного процесса приема/передачи,
	// dummy буфера используются при мгновенном срабатывании прерывания после включения до назначения
	// своих обработчиков ISR
	HAL_SPI_TransmitReceive_IT(&hspi2, spi_dummy_out, spi_dummy_in, 2);

	hspi2.RxISR = SPI_2linesRxISR_8BIT_Fast; // заменяем стандартный обработчик Rx ISR своим
	hspi2.TxISR = SPI_2linesTxISR_8BIT_Fast; // заменяем стандартный обработчик Tx ISR своим
	__HAL_SPI_DISABLE_IT(&hspi2, SPI_IT_TXE); // это прерывание нам не нужно
}

void spiLedRefresh(void) {
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		spi_led_timer = HAL_GetTick() + SPI_LED_AFTERLIGHT;
}

void spiSetErrLed(void) {
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
  err_led_timer = HAL_GetTick() + SPI_ERR_LED_AFTERLIGHT;
}

void spiLedService(void) {
	if (HAL_GetTick() >= spi_led_timer) {
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	}
	if (HAL_GetTick() >= err_led_timer) {
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
	}
}
