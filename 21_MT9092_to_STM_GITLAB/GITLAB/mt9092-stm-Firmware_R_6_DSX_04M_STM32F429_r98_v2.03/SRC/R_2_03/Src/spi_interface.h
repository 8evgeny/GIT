/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   spi_interface.h
 * Author: roman.ganaza
 *
 * Created on October 21, 2020, 3:04 PM
 */

#ifndef SPI_INTERFACE_H
#define SPI_INTERFACE_H

#define SPI_BUF_SIZE    2
#define CS_TIMEOUT      20 // мс  - сколько ждем клоков после активации пина CS, если не приходят, пересбрасываем SPI
#define SPI_RESTART_DELAY       20 // мс  - столько ждем до повторного включения SPI при аварийном рестарте
#define SPI_LED_AFTERLIGHT      5 // мс - время свечения светодиода после активности по SPI
#define SPI_ERR_LED_AFTERLIGHT  1000 // мс - время свечения светодиода ошибки по SPI

void spi_interface_Init(void);
void doCsTimeout(void);
void spiLedService(void);
void nCS_IRQHandler(void);

#endif /* SPI_INTERFACE_H */
