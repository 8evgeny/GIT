/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main_logic.c
 * Author: roman.ganaza
 * 
 * Created on June 22, 2023, 2:06 PM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "main_logic.h"
#include "ST-BUS_SAI_in_out.h"
#include "HDLC_Rx_low_level.h"
#include "HDLC_Tx.h"
#include "HDLC_Tx_low_level.h"
#include "dbg_uart.h"
#include "tastature.h"
#include "g711.h"
#include "volume_control.h"
#include "codec_TLV320AIC3100.h"

#define REG_STEP_TIMEOUT      5000 // таймаут для прохождения каждого шага регистрации на централи
#define ADDR_SIZE             2

uint8_t flag_abon_registered = 0;
tAbonSt abonent_status = ABON_ST_NOT_CONNECTED;
uint32_t reg_step = 0; // шаг регистрации на централи
uint32_t tmr, delay;
char our_addr[ADDR_SIZE+1]; // наш адрес 1-й и 2-й байты в исходящих телеграммах ("??" - неизвестен)
char dest_addr[ADDR_SIZE+1]; // адрес назначения 3-й и 4-й байты в исходящих телеграммах ("##" - централь)
tSoundCh sound_channel = SOUND_CH_B1; // в каком канале ЗВУК (B1 или B2)
uint8_t loud_speaker_en; // вкл/выкл динамик громкой связи
uint8_t handset_speaker_en; // вкл/выкл динамик трубки
uint8_t dialtone_generation_en = 0; // вкл/выкл генерацию длинных гудков
uint8_t short_busytone_generation_en = 0; // вкл генерацию короткого сигнала "занято"
uint8_t handset_support_en = 0; // поддержка трубки/гарнитуры (включается соответствующей телеграммой)
uint8_t handset_poll_en = 0; // включает опрос кнопки и рычага трубки
char hook_key_number[] = "03"; // номер кнопки, на которой запрограммирован "рычаг" трубки
uint8_t handset_hook_inverse = 0; // инверсия сигнала с рычага трубки
uint8_t flag_sound_en = 0; // если установлен в 0, то входной звук с линии заменяется на 0

// TODO: возможно количество групп управления громкостью может быть больше 1, тогда нужно делать массивы
char vcr_grp_addr[16]; // адрес группы управления громкостью, когда приходит Y##aa01 начинаем ослаблять или усиливать звук согласно запрограммированной величине (телеграмма вида P##30H22V-25PZ-)
int32_t vcr_att_dB = 0; // ослабление/усиление звука в группе управления громкость, в децибеллах
float vcr_att = 1; // ослабление/усиление звука в группе управления громкостью, коэффициент (меньше 1  - ослабление, больше 1 - усиление)

// Prototypes
void sendTelegram(char header, char* dst_addr, char* payload, uint8_t payload_len);
void sendTelegramKeepAlive(void);

// Function
void setAddr(char* addr, char* str) {
  addr[0] = str[0];
  addr[1] = str[1];
}


void setTimeout(uint32_t ms) {
  tmr = HAL_GetTick() + ms;
}

void stopTimeout(void) {
  tmr = 0;
}

uint8_t checkTimeout(void) {
  if ((HAL_GetTick() >= tmr)&&(tmr != 0)) return 1; else return 0;
}

void setDelay(uint32_t ms) {
  delay = HAL_GetTick() + ms;
}

void stopDelay(void) {
  delay = 0;
}

uint8_t checkDelay(void) {
  if ((HAL_GetTick() >= delay)&&(delay != 0)) return 1; else return 0;
}

void initMainLogic(void) {
  volumeControlInit();
  C_channel_out = 0x14;
}
void regReset(void) {
  flag_abon_registered = 0;
  abonent_status = ABON_ST_NOT_CONNECTED;
  reg_step = 0;
  led_test_enabled = 0;
  att = 1.0;
}

void signalGeneration(void) { // генерирует различные звуковые сигналы
  static uint32_t tmr;
  static uint32_t tmr2;
  static uint8_t Pulse_nPause; // 1 - гудок, 2 - пауза
  const uint32_t pulse_duty = 1100ul;
  const uint32_t pause_duty = 2500ul;
  const uint32_t busy_tone_duty = 150ul;
  
  if (dialtone_generation_en) { // длинные гудки во время вызова
    if (HAL_GetTick() >= tmr) {
      if (Pulse_nPause == 1) {
        Pulse_nPause = 0;
        tmr = HAL_GetTick() + pause_duty;
        DAC_mode = DM_SOUND;
      } else {
        Pulse_nPause = 1;
        tmr = HAL_GetTick() + pulse_duty;        
        DAC_mode = DM_TONE;
      }
    }
  } else {
    Pulse_nPause = 1;
    tmr = HAL_GetTick() + pulse_duty;
  }
  if (short_busytone_generation_en) { // короткий гудок занято при вызове отключенного абонента
    if (HAL_GetTick() >= tmr2) {
      short_busytone_generation_en = 0;
      if (abonent_status != ABON_ST_CONNECTED) muteHlt();
      //unmuteLoudspeaker();
      DAC_mode = DM_SOUND;
    }
  } else {
    tmr2 = HAL_GetTick() + busy_tone_duty;
  }
}
////////////////////////////////////////////////////////////////////////
// обработка телеграмм, не относящихся к регистрации на централи:
// управление аппаратурой (зуммер, динамик, светодиоды), сброс и т.п.
// может сбрасывать флаг пришедшей телеграммы или оставлять установленным,
// если не завершена регистрация
////////////////////////////////////////////////////////////////////////

void processNonRegistrativeTelegrams(void) {
  if (flag_telegram_received) {

    if (((telegram_string[3] != our_addr[0]) || (telegram_string[4] != our_addr[1]))&&(reg_step >= 2)&&(telegram_string[0] != 'i')&&
      ((telegram_string[1] != our_addr[0]) || (telegram_string[2] != our_addr[1])) && 
        ((telegram_string[3] != vcr_grp_addr[0]) || (telegram_string[4] != vcr_grp_addr[1]))) { // не наш адрес и не статус Интрона
      // непонятно, почему, но у ОТПРАВИТЕЛЯ тоже может быть наш адрес,
      // такие телеграммы приходят от интрона в ответ на нажатие у нас кнопки полудуплексной или симплексной связи,
      // т.е. когда мы являемся инициатором данных видов связи
      puts("Wrong address telegram ignored!");
      flag_telegram_received = 0; // игнорируем телеграмму
    } else {
      
      //------------------------------------------------------------------------------------------------ z-telegram
      if (telegram_string[0] == 'z') { // телеграмма сброса
        
        puts("\"Reset\" telegram received");
        
        
//        сбрасывать нельзя, попадём в бесконечный цикл инициализации, через секунду после приема "z##.." мы должны запросить "Y??##"
//        puts("Device now reboot...");
//        HAL_Delay(100);
//        NVIC_SystemReset();
//        HAL_Delay(3000);
        
        flag_abon_registered = 0;
        abonent_status = ABON_ST_NOT_CONNECTED;
        flag_telegram_received = 0;
        setTimeout(1000);
        reg_step = 255;
        
        //------------------------------------------------------------------------------------------------ i-telegram
      } else if (telegram_string[0] == 'i') { // rec iR1 (готовность интрона: 1 - готов, 0 - не готов), send Q313?

        flag_telegram_received = 0;
        sendTelegram('Q', dest_addr, NULL, 0);

        
        //------------------------------------------------------------------------------------------------ H-telegram
      } else if (telegram_string[0] == 'H') { // телеграммы управления аппаратурой
        const uint8_t NUM_LEDS = 16;
        
        flag_telegram_received = 0;

        switch (telegram_string[5]) {
		  char cmd_string[16];
          case 'g': // тестирование светодиодов
            // telegram_string[6] равно 'L'
            if (telegram_string[7] == '1') {
              tastLedTestOn();
            } else if (telegram_string[7] == '0') {
              tastLedTestOff();
            } else {
              puts("Wrong HxxxxgLy telegram!");
            }
            break;
          case 'L':
            if (telegram_len == 10) {
              puts("Direct led output telegram.");
              if (led_test_enabled) {
                puts("Ignored because LED TEST ENABLED!");
              } else {
                strncpy(cmd_string, &telegram_string[5], 5);
                cmd_string[1] += 1; // в телеграммах кейпады нумеруются с '0', а в протоколе клавишных модулей с '1'
                cmd_string[5] = '\0'; // нуль-терминатор
                tastSetIndication(IND_MANUAL);
                tastCmdLeds(cmd_string);

                printf("Sended to Keypad: ");
                puts(cmd_string);
              }
            } else {
              puts("ERROR: direct led output telegram lenght is invalid!");
            }
            break;
          case 'J': // telegram to set all LED modes of one keypad at once
            
            if (telegram_len == (6 + 1 + NUM_LEDS / 2)) {
              char keypad_num;
              uint8_t i;
              uint8_t led_mode;
              
              keypad_num = telegram_string[6] + 1;
              
              puts("Compressed led output telegram.");
              
              if (led_test_enabled) {
                puts("Ignored because LED TEST ENABLED!");
              } else {
                for (i = 0; i < NUM_LEDS; i++) {

                  // вычленяем полубайты
                  led_mode = telegram_string[7 + i / 2];
                  if ((i & 0x01) == 0) {
                    led_mode >>= 4;
                  } else {
                    led_mode &= 0xF;
                  }
                  if (led_mode != 0xE) {
                    sprintf(cmd_string, "L%c%02u%1u", keypad_num, i + 1, led_mode);
                    tastSetIndication(IND_MANUAL);
                    tastCmdLeds(cmd_string);

                    printf("Sended to Keypad: ");
                    puts(cmd_string);
                  }
                }
              }
            } else {
              puts("ERROR: compressed LED output telegram lenght is invalid!");
            }
            break;
          case 'R': // реле номер 1
            if (telegram_string[6] == '0') {
              HAL_GPIO_WritePin(REL_GPIO_Port, REL_Pin, GPIO_PIN_RESET);
            } else if (telegram_string[6] == '1') {
              HAL_GPIO_WritePin(REL_GPIO_Port, REL_Pin, GPIO_PIN_SET);
            }
            break;
          case 'r': // реле номер 2-5
            if (telegram_string[6] == '2') { // реле 2 (у нас это оптрон EN_25W включения усилителя - возможно неверно!)
              if (telegram_string[7] == '0') {
                //HAL_GPIO_WritePin(EN_25W_GPIO_Port, EN_25W_Pin, GPIO_PIN_RESET);
              } else if (telegram_string[7] == '1') {
                //HAL_GPIO_WritePin(EN_25W_GPIO_Port, EN_25W_Pin, GPIO_PIN_SET);
              }
            }
            break;
          case 'T':
            puts("Buzzer control telegram.");
            if (telegram_string[6] == '0') { // H##31T0--1 - звук отключен
           // в MT9092 происходило бы следующее
//            W: 0x1E 0x21 DSP Microprogramm = "Gain control program"
//            W: 0x0E 0xCE Use Hands free mic, MIC_EN = 1, the Filter/CODEC is powered up, additional high pass,  SPKR = 1 - динамик гр. связи вкл., HSSPKR = 0 - динамик handset выкл.
//            W: 0x1D 0x36 Gain = +9.0dB
              DAC_mode = DM_SOUND;
              dialtone_generation_en = 0;
            } else if (telegram_string[6] == '6') { // H##31T6--1 - Зуммер: Сигнал тревоги
            // в MT9092 происходило бы следующее
//              W: 0x1E 0x00 DSP Microprogramm = "Power up reset program", Reset
//              W: 0x23 0x10 Tone Coeff Register 1-DTMF or Tone Ringer, COEF = 8000 / Frequency
//              W: 0x24 0x20 Tone Coeff Register 2-DTMF or Tone Ringer, COEF = 8000 / Frequency
//              W: 0x26 0xFF Tone Ringer Warble Rate-Tone Ringer период чередования частот заданных выше
//              W: 0x1E 0x61 DSP Microprogramm = "Tone Ringer & Gain control program"
//              R: 0x0E A: 0xCC
//              W: 0x0E 0xCF Use Hands free mic, MIC_EN = 1, the Filter/CODEC is powered up, additional high pass, SPKR = 1, HSSPKR = 1 - все динамики вкл.
//              W: 0x1D 0x2E  Rx pcm gain -3dB
              tone_coef_1 = 0x10;
              tone_coef_2 = 0x20;
              warble_rate = 0xFF;
              DAC_mode = DM_TONE;
              unmuteLoudspeaker();
              unmuteHlt();
              dialtone_generation_en = 0;
            } else if (telegram_string[6] == '7') { // H##31T7--3 - Зуммер: Вызывной сигнал
            // в MT9092 происходило бы следующее
//              W: 0x1E 0x00 DSP Microprogramm = "Power up reset program", Reset
//              W: 0x23 0x08 Tone Coeff Register 1-DTMF or Tone Ringer, COEF = 8000 / Frequency
//              W: 0x24 0x10 Tone Coeff Register 2-DTMF or Tone Ringer, COEF = 8000 / Frequency
//              W: 0x26 0x10 Tone Ringer Warble Rate-Tone Ringer период чередования частот заданных выше
//              W: 0x1E 0x61 DSP Microprogramm = "Tone Ringer & Gain control program"
//              R: 0x0E A: 0xCE
//              W: 0x0E 0xCF Use Hands free mic, MIC_EN = 1, the Filter/CODEC is powered up, additional high pass, SPKR = 1, HSSPKR = 1 - все динамики вкл.
//              W: 0x1D 0x2E  Rx pcm gain -3dB
              tone_coef_1 = 0x08;
              tone_coef_2 = 0x10;
              warble_rate = 0x10;
              DAC_mode = DM_TONE;
              unmuteLoudspeaker();
              unmuteHlt();
              dialtone_generation_en = 0;
            } else if (telegram_string[6] == '8') {  // H##31T8--1 - Зуммер: Сигнал занятости
            // в MT9092 происходило бы следующее
//              W: 0x1E 0x00 DSP Microprogramm = "Power up reset program", Reset
//              W: 0x23 0x10 Tone Coeff Register 1-DTMF or Tone Ringer, COEF = 8000 / Frequency
//              W: 0x24 0x01 Tone Coeff Register 2-DTMF or Tone Ringer, COEF = 8000 / Frequency
//              W: 0x26 0xFF Tone Ringer Warble Rate-Tone Ringer период чередования частот заданных выше
//              W: 0x1E 0x61 DSP Microprogramm = "Tone Ringer & Gain control program"
//              R: 0x0E A: 0xCF
//              W: 0x0E 0xCF Use Hands free mic, MIC_EN = 1, the Filter/CODEC is powered up, additional high pass, SPKR = 1, HSSPKR = 1 - все динамики вкл.
//              W: 0x1D 0x2E
              tone_coef_1 = 0x10;
              tone_coef_2 = 0x01;
              warble_rate = 0xFF;
              DAC_mode = DM_TONE;
              
              if (handset_is_off) { // трубка снята
                unmuteHlt();
                muteLoudspeaker();
              } else {
                muteHlt();
                unmuteLoudspeaker();
              }
              
              dialtone_generation_en = 0;
              short_busytone_generation_en = 1;
            } else if (telegram_string[6] == 'A') {  // H##31TA--1 - Зуммер: Длинные гудки
              // в MT9092 происходило бы следующее
//                  02.08.2023 11 : 26 : 36.134 [RX] - W : 0x1E 0x00
//                  W : 0x23 0x14
//                  W : 0x24 0x00
//                  W : 0x26 0xFF
//                  W : 0x1E 0x61
//                  R : 0x0E A : 0xCF
//                  W : 0x0E 0xCF
//                  W : 0x1D 0x2E
//
//                  02.08.2023 11 : 26 : 37.061 [RX] - W : 0x1E 0x21
//                  W : 0x1D 0x36
//
//                  02.08.2023 11 : 26 : 39.553 [RX] - W : 0x1E 0x00
//                  W : 0x23 0x14
//                  W : 0x24 0x00
//                  W : 0x26 0xFF
//                  W : 0x1E 0x61
//                  R : 0x0E A : 0xCF
//                  W : 0x0E 0xCF
//                  W : 0x1D 0x2E
//
//                  02.08.2023 11 : 26 : 40.464 [RX] - W : 0x1E 0x21
//                  W : 0x1D 0x36
              // Т.е. секунду гудок, потом 2,5 секунды пауза и по-новой
              tone_coef_1 = 0x14;
              tone_coef_2 = 0x00;
              warble_rate = 0xFF;
              DAC_mode = DM_TONE;
//              unmuteLoudspeaker();
//              unmuteHlt();
              dialtone_generation_en = 1;
            } else {
              puts("Unknown T-parameter for H-telegram.");
            }
            break;
          case 'N': // управление гарнитурой ("H##31NH1") и трубкой ("H##31Nh1")
            
            switch (telegram_string[6]) {

              case 'H':
                puts("Switch to Headset Microphone State Telegram");
                if (telegram_string[7] == '1') {
                  selectHandsetMic();
                } else if (telegram_string[7] == '0') {
                  selectLoudspeakMic();
                } else {
                  puts("Wrong H##aaNHx-telegramm x-parameter");
                }
                break;
              case 'h':
                puts("Switch to Handset State Telegram");
                if (telegram_string[7] == '1') {
                  selectHandsetMic();
                  muteLoudspeaker();
                  unmuteHlt();
                } else if (telegram_string[7] == '0') {
                  selectLoudspeakMic();
                  unmuteLoudspeaker();
                  muteHlt();
                } else {
                  puts("Wrong H##aaNhx-telegramm x-parameter");
                }
                break;
              default:
                puts("Unknown H-telegram with 'N'-parameter");
                break;
            }
            break;
            
          default:
            puts("Unknown H-telegram.");
            break;
        }
        sendTelegram('Q', &telegram_string[1], NULL, 0); // квитирование (адрес назначения берем из адреса отправителя)
        
        //------------------------------------------------------------------------------------------------ W-, X-telegram
      } else if ((telegram_string[0] == 'W')|| // телеграмма "Установление TW-/PA-соединения между абонентскими устройствами" (TW - two way, PA - видимо в одну сторону)
        (telegram_string[0] == 'X')) { // телеграмма Установление дуплексного соединения
        
        flag_telegram_received = 0;
        
        
        sendTelegram('Q', dest_addr, NULL, 0); // квитирование (адрес назначения - линейная карта)
        
          // если адрес источника наш, значит мы являемся инициатором соединения
          // тогда приходит телеграмма вида W31##1A00, 1-счетчик попыток, A - приоритет (не исп.), 0 - кодек G711, второй 0 - 0-бит mSpeaker, 1-бит mBooster (все выкл)
          // в MT9092 происходило бы следующее
//          R: 0x0E A: 0xCE
//          W: 0x0E 0xCE
//          R: 0x0E A: 0xCE
//          W: 0x0E 0xCE Use Hands free mic, MIC_EN = 1, the Filter/CODEC is powered up, additional high pass, SPKR = 1 - динамик гр. связи вкл., HSSPKR = 0 - динамик handset выкл.
         
          // если инициатор соединения не мы
          // тогда приходит телеграмма вида W##311A03, 1-счетчик попыток, A - приоритет (не исп.), 0 - кодек G711, 3 - 0-бит mSpeaker, 1-бит mBooster (все вкл)
          // в MT9092 происходило бы следующее
//          W: 0x1D 0x36 Gain = +9.0dB
//          R: 0x0E A: 0xCE
//          W: 0x0E 0xCF  Use Hands free mic, MIC_EN = 1, the Filter/CODEC is powered up, additional high pass, SPKR = 1, HSSPKR = 1 - все динамики вкл.
        
//        unmuteLoudspeaker();
//        
//        if ((telegram_string[1] == our_addr[0]) && (telegram_string[2] == our_addr[1])) { // адрес источника наш, значит мы являемся инициатором соединения
//          muteHlt();
//        } else { // не мы иниициатор
//          unmuteHlt();
//        }
        
        abonent_status = ABON_ST_CONNECTED;
        sendTelegramKeepAlive(); // передаем изменившийся статус
        
        //------------------------------------------------------------------------------------------------ w-, x-telegram
      } else if ((telegram_string[0] == 'w') || // телеграмма "Завершение TW-/PA-соединения между абонентскими устройствами"
        (telegram_string[0] == 'x')) { // телеграмма Завершение дуплексного соединения между абонентскими устройствами
        flag_telegram_received = 0;
        
        // здесь приходит телеграмма вида w##311A, 1-счетчик попыток, A - приоритет (не исп.)
        sendTelegram('Q', dest_addr, NULL, 0); // квитирование (адрес назначения - линейная карта)
        // в MT9092 происходило бы следующее
//        R: 0x1E A: 0x21
//        W: 0x1E 0x00
//        W: 0x1E 0x21
//        R: 0x0E A: 0xCF
//        W: 0x0E 0xCE  Use Hands free mic, MIC_EN = 1, the Filter/CODEC is powered up, additional high pass, SPKR = 1 - динамик гр. связи вкл., HSSPKR = 0 - динамик handset (трубка) выкл.
        //muteHlt();
        DAC_mode = DM_SOUND;
        abonent_status = ABON_ST_NOT_CONNECTED;
        sendTelegramKeepAlive(); // передаем изменившийся статус
        
        //------------------------------------------------------------------------------------------------ Q-telegram
      }  else if (telegram_string[0] == 'Q') {
        // флаг не сбрасываем
        // квитирование - такие телеграммы обрабатываются либо при регистрации в doRegistration(),
        // либо во время ответа от Интрона на нажатие клавиш в processKeys()
        
        //------------------------------------------------------------------------------------------------ Y-telegram
      }  else if (telegram_string[0] == 'Y') {
        flag_telegram_received = 0;

        // здесь приходит телеграмма вида Y##2201, где 22 - адрес группы управления громкостью, 01 или 00 на конце, соответственно изменить громкость или не менять
        sendTelegram('Q', dest_addr, NULL, 0); // квитирование (адрес назначения - линейная карта)
        
        if (telegram_string[6] == '1') {
          puts("Volume control group ON");
          att = vcr_att;
        } else if (telegram_string[6] == '0') {
          puts("Volume control group OFF");
          att = 1.0;
        }
        //------------------------------------------------------------------------------------------------ Unknown telegram
      } else {
        if (flag_abon_registered) {
          flag_telegram_received = 0;
          puts("Unknown telegram!");
        }
      }
    }
  }
}
////////////////////////////////////////////
// регистрация в централи (Intron)
////////////////////////////////////////////
void doRegistration(void) {
  static uint32_t prev_step = 0xFF;
  static uint32_t prev_sync_bit = 0xFF;
  
  
  if (prev_sync_bit != sync_bit) { // только при изменении бита синхронизации
    prev_sync_bit = sync_bit;
    if (prev_sync_bit == 0) { // нет синхронизации
      puts("No sync!");
      flag_sound_en = 0;
      regReset();
      tastSetIndication(IND_MANUAL);
      tastCmdLeds("L1015");
      tastCmdLeds("L1025");
    } else {
      puts("Sync!");
      tastSetIndication(IND_BLINK_500);
    }
  }
  
  if (checkTimeout()) { // таймаут
    setTimeout(REG_STEP_TIMEOUT);
    puts("Timeout!");
    regReset();
  }

  if (prev_step != reg_step) {
    prev_step = reg_step;
    puts("\r===============================");
    printf("reg_step = %u\r", reg_step);
    puts("===============================\r");
  }
  
  switch(reg_step) {
    case 0: // wait sync, send "Y??##"
      if (sync_bit) {
        setAddr(our_addr, "??");
        sendTelegram('Y',"##", NULL, 0);
        setTimeout(REG_STEP_TIMEOUT);
        reg_step++;
      }
      break;
    case 1: // rec "Ixxxx1" or "Ixxxx2", send "Rssdd"
      if (flag_telegram_received) {
        flag_telegram_received = 0;
        
        flag_sound_en = 1;
        
        if (telegram_string[0] == 'I') {
          sound_channel = (tSoundCh) (telegram_string[5] - '0');
        }
        
        if ((telegram_string[0] == 'I') ||(telegram_string[0] == 'Q')) {
          setAddr(our_addr, &telegram_string[3]);
          setAddr(dest_addr, &telegram_string[1]);

          sendTelegram('R', dest_addr, NULL, 0);
          setTimeout(REG_STEP_TIMEOUT);
          reg_step++;
        } else {
          reg_step = 0;
        }
      }
      break;
    case 2:
      if (flag_telegram_received) { // rec "B3?31WwGgXxLlYi"
        flag_telegram_received = 0;

        if (telegram_string[0] == 'B') {
          setTimeout(REG_STEP_TIMEOUT);
          reg_step++;
        } else {
          reg_step = 0;
        }
      }
      break;
    case 3: // rec "Q3?31", send "P31##RsInduD++"
      if (flag_telegram_received) {
        flag_telegram_received = 0;

        tastSetIndication(IND_BLINK_1000);
        
        if (telegram_string[0] == 'Q') {

          sendTelegram('P', "##", "RsInduD++", 9); // по-видимому, "начать программирование"
          puts("To Intron: Start programming");
          
          setTimeout(REG_STEP_TIMEOUT);
          reg_step++;
        } else {
          reg_step = 0;
        }
      }
      break;
    case 4: // rec "Q##31"
      if (flag_telegram_received) {
        flag_telegram_received = 0;

        if (telegram_string[0] == 'Q') {
          
          setTimeout(REG_STEP_TIMEOUT);
          reg_step++;
        } else {
          reg_step = 0;
        }
      }      
      break;
      // программирующие телеграммы
      // rec "P##31SN", "P##31V0500", "P##31oD0", (P##31KX03H00000N), P##31C00c0014, P##31B00010D0, P##31EInduD++ send Q31## и прочие программирующие телеграммы
      // также здесь принимаем P##31q, что означает, что данный порт не запрограммирован
      // в скобках - дополнительная телеграмма, которая приходит, когда сконфигурирована гарнитура,
      // означает на 12-м (с нуля) кейпаде ("X"), 3-ей кнопке ("03") соответствует Hook/Рычаг ("H00000"), с нормальным ("N") режимом работы,
      // бывает еще инверсный ("I")
      // P##31EInduD++ - телеграмма "конец программирования", после неё P-телеграмм не будет
    case 5: //case 6: case 7: case 8: case 9: case 10:
      if (flag_telegram_received) {
        flag_telegram_received = 0;

        if (telegram_string[0] == 'P') {
          sendTelegram('Q', "##", NULL, 0);

          setTimeout(REG_STEP_TIMEOUT);
          
          printf("Programming telegram: ");
          
          if (telegram_string[5] == 'q') { // приняли P##31q
            puts("station not used");
            abonent_status = ABON_ST_UNPROGRAMMED;
            reg_step = 11;
          } else if (telegram_string[5] == 'E') { // приняли P##31EInduD++
            puts("end of programming");
            reg_step = 11;
          } else if (telegram_string[5] == 'K') { // приняли P##31KX03H00000N
            if (telegram_string[6] == 'X') { // 12 с нуля кейпад
              puts("hook programming (handset) ");
              handset_support_en = 1;
              if (strncmp(&telegram_string[9], "H00000", 6) == 0) {
                hook_key_number[0] = telegram_string[7];
                hook_key_number[1] = telegram_string[8];
                printf("hook key number = %02s\r", hook_key_number);
                
                if (telegram_string[15] == 'N') {
                  puts("Normal mode\r");
                  handset_hook_inverse = 0;
                } else if (telegram_string[15] == 'I'){
                  puts("Inverse mode\r");
                  handset_hook_inverse = 1;
                }
              }
            }
          } else if (telegram_string[5] == 'H') { // приняли P##31H22V-25PZ-, где 22 номер (адрес?) группы управления громкостью, -25(дБ) - децибелы прибавляем/убавляем при включении группы
            printf("volume control group. ");
            sscanf(&telegram_string[5], "H%2sV%iPZ-", &vcr_grp_addr[0], &vcr_att_dB);
            vcr_att = powf(10, (float)vcr_att_dB/20);
            printf("group addr = %02s, vol. change = %i dB = %5f\r", vcr_grp_addr, vcr_att_dB, vcr_att);
          } else {
            puts("unknown programming telegram");
          }
          if (reg_step > 10) setDelay(100);
        } else {
          reg_step = 0;
        }
      }   
      break;
    case 11: // send V31##K001000:001
      if (checkDelay()) {

        
        if (abonent_status == ABON_ST_UNPROGRAMMED) { // порт не запрограммирован
          sendTelegram('V', "##", "TsS26H03", 8); // шлём наш тип устройства
          reg_step = 16; // переходим на предпоследний шаг
        } else {
          sendTelegram('V', "##", "K001000:001", 11);
          reg_step++;
        }

        setTimeout(REG_STEP_TIMEOUT);
      }   
      break;
    case 12: // rec "Q##31", send V31##TsS26H03
      if (flag_telegram_received) {
        flag_telegram_received = 0;

        if (telegram_string[0] == 'Q') {
          sendTelegram('V', "##", "TsS26H03", 8); // шлём наш тип устройства

          setTimeout(REG_STEP_TIMEOUT);
          reg_step++;
        } else {
          puts("Wrong Answer!");
          reg_step = 0;
        }
      }  
      break;
    case 13: // rec "Q##31", send V31##H1
      if (flag_telegram_received) {
        flag_telegram_received = 0;

        tastSetIndication(IND_MANUAL);
        tastCmdLeds("L?000"); // вырубаем мигание
        
        if (telegram_string[0] == 'Q') {
          sendTelegram('V', "##", "H1", 2);

          setTimeout(REG_STEP_TIMEOUT);
          reg_step++;
        } else {
          reg_step = 0;
        }
      }  
      break;
    case 14: // rec "Q##31", send V31##hE000000000000
      if (flag_telegram_received) {
        flag_telegram_received = 0;

        if (telegram_string[0] == 'Q') {
          sendTelegram('V', "##", "hE000000000000", 14);

          setTimeout(REG_STEP_TIMEOUT);
          reg_step++;
        } else {
          reg_step = 0;
        }
      } 
      break;
    case 15: // rec "Q##31", send P31##Q
      if (flag_telegram_received) {
        flag_telegram_received = 0;

        if (telegram_string[0] == 'Q') {
          sendTelegram('P', "##", "Q", 1);

          setTimeout(REG_STEP_TIMEOUT);
          reg_step++;
        } else {
          reg_step = 0;
        }
      } 
      break;
    case 16: // rec "Q##31", send A31##x
      if (flag_telegram_received) {
        flag_telegram_received = 0;

        if (telegram_string[0] == 'Q') {
          sendTelegramKeepAlive();

          reg_step++;
          setDelay(3000);
          stopTimeout();
          flag_abon_registered = 1;
          
          unmuteLoudspeaker();
          
          if (abonent_status == ABON_ST_UNPROGRAMMED) {
            tastSetIndication(IND_BLINK_UNPROGRAMMED);
            puts("Abonent successfully registered but port unprogrammed!");
          } else {
            puts("Abonent successfully registered!");
          }
        } else {
          reg_step = 0;
        }
      } 
      break;
    case 17: // send A31##x periodically (keepalive)
      if (checkDelay()) {

        if (handset_support_en) {
          handset_poll_en = 1;
        }
        sendTelegramKeepAlive();
        setDelay(3000);
      }
      break;
      ////////////////////////
    case 255: // здесь ожидаем паузу выйдем по таумауту
      
      break;
  }
}
// обрабатывает нажатия/отжатия клавиш,
// шлёт соответствующие телеграммы,
// принимает квитанции, перепосылает в случае отсутствия оных
void processKeys(uint8_t key_event) {
  static char tel_payload[16];
  static uint8_t key_sended = 0;
  static uint8_t attempt = 1; // число попыток отправки нажатия/отжатия клавиши
  static uint32_t timeout_time;
  const uint32_t timeout_value = 100; // мс таймаут
  
  if (flag_abon_registered) { // начинаем работать только после регистрации на централи
    if (key_event) {
      puts("Key event!");
      
      // key_event_str имеет такой формат:
      // K316 ; Нажатие на клавишу 16 Pad_3 
      // k316 ; Отжатие клавиши 16 Pad_3 
      
      // реакция на снятие/повешение трубки  (не нужна)
//      if ((key_event_str[1] == HANDSET_KEYPAD_NUMBER)&&(strncmp(&key_event_str[2], hook_key_number, 2) == 0)) {
//        if (key_event_str[0] == 'K') {
//
//        } else { // 'k'
//          
//        }
//      }
      
      attempt = 1; // первая попытка
      tel_payload[0] = attempt + '0';
      tel_payload[1] = key_event_str[1] - 1; // номер клавишного блока 0-9, если превышает 9, то дальше идут от 'A' до 'J'
      tel_payload[2] = key_event_str[2]; tel_payload[3] = key_event_str[3]; // нажатая клавиша '01'-'16', десятичка в символьном виде
      
      sendTelegram(key_event_str[0], "##", tel_payload, 4);
      
      timeout_time = HAL_GetTick() + timeout_value;
      key_sended = 1;
    }
    if (flag_telegram_received) {
      if (telegram_string[0] == 'Q') {
        flag_telegram_received = 0;

        if (key_sended) {
          key_sended = 0;
          puts("Key event quittance!");
        }
      }
    }
    if (key_sended) {
      if (HAL_GetTick() >= timeout_time) { // вышел таймаут прихода квитирования от интрона
        attempt++;
        if (attempt > 3) {
          key_sended = 0; // исчерпали число попыток
        } else {
          timeout_time = HAL_GetTick() + timeout_value;
          tel_payload[0] = attempt + '0';
          sendTelegram(key_event_str[0], "##", tel_payload, 4); // перепосылаем
        }
      }
    }
  }
}

void doMainLogic(void) {
  uint8_t key_evnt;

  processNonRegistrativeTelegrams(); // может сбрасывать flag_telegram_received и сам процесс регистрации, вызывать обязательно перед doRegistration
  doRegistration(); // отрабатывает процесс регистрации на централи
  
  key_evnt = doTastature(); // обрабатывает сообщения от клавиатуры, приходящие в процессе её опроса
  
  processKeys(key_evnt);
  
  signalGeneration();
  
  doVolumeControl();
}

// отправляет телеграммы вида:
//TABCD{параметр}
// заголовок
//1. Byte : T : Тип телеграммы
// адрес источника
//2. Byte : A : Источник : Номер линейной карты(LC)
//3. Byte : B : Источник : Номер абонентского устройства
// адрес назначения
//4. Byte : C : Адресат : Номер линейной карты(LC)
//5. Byte : D : Адресат : Номер абонентского устройства
// полезная нагрузка
//6. Byte : Параметр : - дополнительные данные, зависящие от типа телеграммы (может не быть)
void sendTelegram(char header, char* dst_addr, char* payload, uint8_t payload_len) {
  uint8_t i;
  
  if (payload_len + 5 >= MAX_TX_TEL_LEN) {
    puts("Error: too large tx telegram payload!");
    return;
  }
  
  while(!isHdlcFifoEmpty()); // ждём освобождения буфера
  
  writeHdlcTxFifo(header);
  writeHdlcTxFifo(our_addr[0]);
  writeHdlcTxFifo(our_addr[1]);
  writeHdlcTxFifo(dst_addr[0]);
  writeHdlcTxFifo(dst_addr[1]);
  if ((payload_len != 0)&&(payload != 0)) {
    for(i = 0; i < payload_len; i++) {
      writeHdlcTxFifo(payload[i]);
    }
  }
  if (!flag_HDLC_transmitter_busy) startHdlcTransmition(); // если не занят передатчик, передаем сразу
}
// статус - отсылается в Интрон каждые 3 секунды в телеграмме alive-вида "Ass##x", где x - статус
// '0' - зарегистрирован
// '2' - установлено PA/TW-соединение
void sendTelegramKeepAlive(void) {
  char str[8];
  sprintf(str, "%1u", (uint32_t)abonent_status);
  sendTelegram('A', "##", str, 1);
}
