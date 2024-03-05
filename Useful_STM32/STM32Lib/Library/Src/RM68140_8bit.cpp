/*
 * 8bit TFT Library for STM32F103
 * based on MCUFRIEND_kbv.cpp by David Prentice
 * https://github.com/prenticedavid/MCUFRIEND_kbv
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RM68140_8bit.h"

#define MIPI_DCS_REV1   (1<<0)
#define AUTO_READINC    (1<<1)
#define READ_BGR        (1<<2)
#define READ_LOWHIGH    (1<<3)
#define READ_24BITS     (1<<4)
#define XSA_XEA_16BIT   (1<<5)
#define READ_NODUMMY    (1<<6)
#define INVERT_GS       (1<<8)
#define INVERT_SS       (1<<9)
#define MV_AXIS         (1<<10)
#define INVERT_RGB      (1<<11)
#define REV_SCREEN      (1<<12)
#define FLIP_VERT       (1<<13)
#define FLIP_HORIZ      (1<<14)

//static uint8_t is8347 = 0;
#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif

#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

// Constructor when using 8080 mode of control.
RM68140_8bit::RM68140_8bit(void)
// Временно : Adafruit_GFX(TFTWIDTH, TFTHEIGHT) {
{
  WIDTH       = 320;
  HEIGHT      = 480;
  _width      = WIDTH;
  _height     = HEIGHT;
  rotation    = 0;
  cursor_y    = cursor_x    = 0;
  textsize_x  = textsize_y  = 1;
  textcolor   = textbgcolor = 0xFFFF;
  wrap        = true;
  _cp437      = false;
  gfxFont     = NULL;

  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                                           // Подаём тактирование на порты A и B
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

  DATA_Port->BSRR     = (GPIO_PIN_0
                       |GPIO_PIN_1
                       |GPIO_PIN_2
                       |GPIO_PIN_3
                       |GPIO_PIN_4
                       |GPIO_PIN_5
                       |GPIO_PIN_6
                       |GPIO_PIN_7) << 16;                                      // Затираем порты B0-B7
  DATA_Port->CRL      = 0x33333333;                                             // Настраиваем B0-B7 на выход с максимальной скоростью в режиме PUSH/PULL

  // Обнуляем порты управления дисплеем
  CS_GPIO_Port->BSRR  = CS_Pin << 16;
  RS_GPIO_Port->BSRR  = RS_Pin << 16;
  WR_GPIO_Port->BSRR  = WR_Pin << 16;
  RD_GPIO_Port->BSRR  = RD_Pin << 16;
  RST_GPIO_Port->BSRR = RST_Pin << 16;
  BK_GPIO_Port->BSRR  = BK_Pin << 16;

  DATA_Port->CRH      &= ~0x0000ffff;                                           // Выставляем порт B8-B11 в исходное состояние как при сбросе
  DATA_Port->CRH      |= 0x00003333;                                                 // Настраиваем B8-B11 на выход с максимальной скоростью в режиме PUSH/PULL

  GPIOA->CRH          &= ~0x000ff000;                                                     // Выставляем порт A11-A12 в исходное состояние как при сбросе
  GPIOA->CRH          |= 0x00033000;                                                     // Настраиваем A11-A12 на выход с максимальной скоростью в режиме PUSH/PULL

  CS_IDLE; // Disable CS
//  CD_DATA; // Enable Command
  WR_IDLE; // Disable WR
  RD_IDLE; // Disable RD
}

void RM68140_8bit::fillScreen(uint16_t color)
{
  WR_REG(CMD_SET_X);
  WR_DATA(0);
  WR_DATA(0);
  WR_DATA(_width>>8);
  WR_DATA(_width&0xff);

  WR_REG(CMD_SET_Y);
  WR_DATA(0);
  WR_DATA(0);
  WR_DATA(_height>>8);
  WR_DATA(_height&0xff);

  WR_REG(0x21);
  delay(2000);
  WR_REG(0x20);
  delay(2000);

  WR_REG(CMD_RAM_WRITE);
  uint32_t I=_width*_height;
  while(I)
  {
    WR_DATA(0xe0);

    //    WR_DATA(color>>8);
//    WR_DATA(color&0xff);
    I--;
  }
}

void RM68140_8bit::drawPixel(int16_t x, int16_t y, uint16_t color)
{
//  WR_REG(CMD_SET_X);
//  WR_DATA(x>>8);
//  WR_DATA(x&0xff);
//  WR_DATA(x>>8);
//  WR_DATA(x&0xff);

//  WR_REG(CMD_SET_Y);
//  WR_DATA(y>>8);
//  WR_DATA(y&0xff);
//  WR_DATA(y>>8);
//  WR_DATA(y&0xff);

  WR_REG(CMD_RAM_WRITE);
  WR_DATA(color>>8);
  WR_DATA(color&0xff);
}

void RM68140_8bit::setWriteDataBus(void)
{
  DATA_Port->CRL = 0x33333333;
}

void RM68140_8bit::setReadDataBus(void)
{
  DATA_Port->CRL = 0x44444444;
}

void RM68140_8bit::WR_REG(uint8_t Reg)
{
  CD_COMMAND;
  CS_ACTIVE;
  DATA_Port->BSRR = ((~Reg)<<16) | (Reg); //Set pins to the 8 bit number
  WR_STROBE;
  CS_IDLE;
}

void RM68140_8bit::WR_DATA(uint8_t Data)
{
  CD_DATA;
  CS_ACTIVE;
  DATA_Port->BSRR = ((~Data)<<16) | (Data); //Set pins to the 8 bit number
  WR_STROBE;
  CS_IDLE;
}

void RM68140_8bit::init(void)
{
  // Дёрнем хардварный сброс
  RST_IDLE;   // Сброс = 1
  delay(5);
  RST_ACTIVE; // Сброс = 0
  delay(20);
  RST_IDLE;   // Сброс = 1
  delay(150);

  // Устанавливаем порт данных на запись
  setWriteDataBus();

  WR_REG(0x01);               // Програмный сброс
  delay(150);

  WR_REG(0x28);               // Дисплей отключить

  WR_REG(0xc0);               // Контроллер питания 1 [0E 0E]
  WR_DATA(0x10);
  WR_DATA(0x10);

  //0xC1, 1, 0x41,
  WR_REG(0xc1);               // Контроллер питания 2 [43]
  WR_DATA(0x41);

  WR_REG(0xc5);               // VCOM  контроллер 1 [00 40 00 40]
  WR_DATA(0x00);
  WR_DATA(0x22);
  WR_DATA(0x80);
  WR_DATA(0x40);

  WR_REG(0x36);               // Доступ к памяти [00]
//  WR_DATA(0x68);
  WR_DATA(0x08);

  WR_REG(0x33);               //
  WR_DATA(0x00);
  WR_DATA(0x00);
  WR_DATA(0x01);
  WR_DATA(0xe0);
  WR_DATA(0x00);
  WR_DATA(0x00);

  WR_REG(0x37);
  WR_DATA(0x00);
  WR_DATA(0x00);

//  WR_REG(0xb0);               // Интерфейс     [00] и так по нулям
//  WR_DATA(0x00);

  WR_REG(0xB1);               // Контроллер частоты кадров [B0 11]
  WR_DATA(0xB0);
  WR_DATA(0x11);

  WR_REG(0xb4);               // Контроллер инверсии [02]
  WR_DATA(0x02);

  WR_REG(0xb6);               // Display Function Control [02 02 3B] .kbv NL=480
//  WR_DATA(0x02);
//  WR_DATA(0x02);
  WR_DATA(0x00);
  WR_DATA(0x22);
  WR_DATA(0x3b);

  WR_REG(0xb7);               // Entry Mode      [06]
  WR_DATA(0xc6);

  WR_REG(0x3a);               // Interlace Pixel Format [XX]
  WR_DATA(0x55);

  WR_REG(0xf7);               // Контроллер яркости 3 [A9 51 2C 82]     // Такого не существует
  WR_DATA(0xa9);
  WR_DATA(0x51);
  WR_DATA(0x2c);
  WR_DATA(0x82);

  // Гамма
  WR_REG(0xE0);
  WR_DATA(0x00);
  WR_DATA(0x07);
  WR_DATA(0x10);
  WR_DATA(0x09);
  WR_DATA(0x17);
  WR_DATA(0x0B);
  WR_DATA(0x40);
  WR_DATA(0x8A);
  WR_DATA(0x4B);
  WR_DATA(0x0A);
  WR_DATA(0x0D);
  WR_DATA(0x0F);
  WR_DATA(0x15);
  WR_DATA(0x16);
  WR_DATA(0x0F);

  WR_REG(0x11);               // Выход из сна

  delay(150);

  WR_REG(0x29);               // Включение дисплея
  delay(25);
  // Окно вывода во весь экран
//  WR_REG(CMD_SET_X);
//  WR_DATA(0);
//  WR_DATA(0);
//  WR_DATA(_width>>8);
//  WR_DATA(_width&0xff);

//  WR_REG(CMD_SET_Y);
//  WR_DATA(0);
//  WR_DATA(0);
//  WR_DATA(_height>>8);
//  WR_DATA(_height&0xff);

}

