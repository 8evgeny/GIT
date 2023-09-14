#include "global_vars.h"
#include "eeprom_i2c.h"
#include "dbg_uart.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char str[256]; // строка для отладочного вывода

// все нижеследующие параметры дублируются в EEPROM
// размеры менять только вместе с адресами!
//--------------------------------------------------------------------- EEPROM Vars
uint8_t dhcp_en = 0; // 1 - DHCP включен, 0 - DHCP - выключен
char login[LOGIN_LEN];
char password[PASSWORD_LEN];
char line_name[NUM_LINES][LINE_NAME_LEN]; // имена линий
char line_enabled[NUM_LINES]; // 1 - линия включена (мониторинг включен), 0 - линия выключена (мониторинг выключен)
int16_t top_limit[NUM_LINES]; // Верхний порог отклонения,% 
int16_t bot_limit[NUM_LINES]; // Нижний порог отклонения,%
uint32_t meas_interval; // Текущий интервал измерения, сек
float recalc_coef[NUM_LINES]; // коэффициенты пересчета импеданса с 1 кГц на 80 Гц (не используется)
uint8_t cal_err[NUM_LINES]; // ошибка калибровки (калибровка ни разу не была произведена)
float ref_impedance_1khz[NUM_LINES];
float ref_impedance_80hz[NUM_LINES];
uint32_t err_counter[NUM_LINES]; // счетчик ошибок
//--------------------------------------------------------------------- END EEPROM Vars
float last_meas[NUM_LINES] = {0,0,0,0,0,0};
uint8_t imp_err[NUM_LINES] = {0,0,0,0,0,0}; // ошибка импеданса
uint8_t gnd_err[NUM_LINES] = {0,0,0,0,0,0}; // ошибка по замыканию на землю
uint8_t sht_err[NUM_LINES] = {0,0,0,0,0,0}; // ошибка по КЗ линии
int32_t discrepancy[NUM_LINES] = {0, 0, 0, 0, 0, 0}; // Текущее отклонение импеданса,%
uint8_t MK_out_data[NUM_LINES] = {0, 0, 0, 0, 0, 0}; // состояния выходов ошибок на МК
uint8_t line_control_data[NUM_LINES] = {1, 1, 1, 1, 1, 1}; // состояния линий 1-усилитель подключен, 0-усилитель отключен

/* TODO: на данный момент не поддерживаются кириллические логин и пароль.
 * решение есть в "подвале" administration.html function foo()
 * Но оно потребует не хранить в MCU в открытом виде логин и пароль, а только в виде хэша пары логин/пароль,
 * что в общем-то правильно, т.к. решение из foo не совместимо с библиотекой base64.c.
 * Также это потребует при смене пароля / логина передачи старых и новых учетных данных в виде хэша, что
 * тоже более правильно
 * */
char login_password[MAX_LOGIN_PASSWORD_COUPLE_LEN];
char login_password_hash[MAX_LOGIN_PASSWORD_BASE64_LEN];
uint8_t flag_login_password_error = 0; // ошибка логина/пароля (напр., слишком длинные)

uint8_t err_eeprom_fail = 0;
uint8_t err_no_conn_with_MK_board = 0; // ошибка: нет связи с платой МК

uint8_t flag_force_start = 0; // принудительный запуск измерения
uint8_t flag_reset_to_defaults = 0;

void eepLoadVars(void) {
  eepReadSeq(EEP_ADDR_login, (unsigned char*) &login[0], sizeof (login));
  eepReadSeq(EEP_ADDR_password, (unsigned char*) &password[0], sizeof (password));

  eepReadSeq(EEP_ADDR_IP_ADDRESS, (unsigned char*) &IP_ADDRESS[0], sizeof (IP_ADDRESS));
  eepReadSeq(EEP_ADDR_NETMASK_ADDRESS, (unsigned char*) &NETMASK_ADDRESS[0], sizeof (NETMASK_ADDRESS));
  eepReadSeq(EEP_ADDR_GATEWAY_ADDRESS, (unsigned char*) &GATEWAY_ADDRESS[0], sizeof (GATEWAY_ADDRESS));

  eepReadSeq(EEP_ADDR_line_name, (unsigned char*) &line_name[0][0], sizeof (line_name));

  eepReadSeq(EEP_ADDR_line_enabled, (unsigned char*) &line_enabled[0], sizeof (line_enabled));
  
  eepReadSeq(EEP_ADDR_top_limit, (unsigned char*) &top_limit[0], sizeof (top_limit));
  eepReadSeq(EEP_ADDR_bot_limit, (unsigned char*) &bot_limit[0], sizeof (bot_limit));
  eepReadSeq(EEP_ADDR_recalc_coef, (unsigned char*) &recalc_coef[0], sizeof (recalc_coef));
  eepReadSeq(EEP_ADDR_cal_err, (unsigned char*) &cal_err[0], sizeof (cal_err));
  eepReadSeq(EEP_ADDR_ref_impedance_1khz, (unsigned char*) &ref_impedance_1khz[0], sizeof (ref_impedance_1khz));
  eepReadSeq(EEP_ADDR_ref_impedance_80hz, (unsigned char*) &ref_impedance_80hz[0], sizeof (ref_impedance_80hz));
  eepReadSeq(EEP_ADDR_err_counter, (unsigned char*) &err_counter[0], sizeof (err_counter));

  eepReadSeq(EEP_ADDR_meas_interval, (unsigned char*) &meas_interval, sizeof (meas_interval));

  eepReadSeq(EEP_ADDR_dhcp_en, (unsigned char*) &dhcp_en, sizeof (dhcp_en));
}

void eepWriteDefaults(void) {
  uint8_t test_byte, i;

  dbgUartPuts("eepWriteDefaults\r");

  strcpy(login, DEFAULT_LOGIN);
  eepWriteSeq(EEP_ADDR_login, (unsigned char*) &login[0], sizeof(login));

  strcpy(password, DEFAULT_PASSWORD);
  eepWriteSeq(EEP_ADDR_password, (unsigned char*) &password[0], sizeof(password));

  IP_ADDRESS[0] = DEFAULT_IP0;
  IP_ADDRESS[1] = DEFAULT_IP1;
  IP_ADDRESS[2] = DEFAULT_IP2;
  IP_ADDRESS[3] = DEFAULT_IP3;
  eepWriteSeq(EEP_ADDR_IP_ADDRESS, (unsigned char*) &IP_ADDRESS[0], sizeof(IP_ADDRESS));

  NETMASK_ADDRESS[0] = DEFAULT_NETMASK0;
  NETMASK_ADDRESS[1] = DEFAULT_NETMASK1;
  NETMASK_ADDRESS[2] = DEFAULT_NETMASK2;
  NETMASK_ADDRESS[3] = DEFAULT_NETMASK3;
  eepWriteSeq(EEP_ADDR_NETMASK_ADDRESS, (unsigned char*) &NETMASK_ADDRESS[0], sizeof(NETMASK_ADDRESS));

  GATEWAY_ADDRESS[0] = DEFAULT_GATEWAY0;
  GATEWAY_ADDRESS[1] = DEFAULT_GATEWAY1;
  GATEWAY_ADDRESS[2] = DEFAULT_GATEWAY2;
  GATEWAY_ADDRESS[3] = DEFAULT_GATEWAY3;
  eepWriteSeq(EEP_ADDR_GATEWAY_ADDRESS, (unsigned char*) &GATEWAY_ADDRESS[0], sizeof(GATEWAY_ADDRESS));

  for (i = 0; i < NUM_LINES; i++) {
    sprintf(str, "%s%u", DEFAULT_LINE_NAME, i+1);
    strcpy(line_name[i], str);
  }
  eepWriteSeq(EEP_ADDR_line_name, (unsigned char*) &line_name[0][0], sizeof(line_name));

  for (i = 0; i < NUM_LINES; i++) {
    line_enabled[i] = DEFAULT_LINE_EN_DIS;
    top_limit[i] = DEFAULT_TOP_LIMIT;
    bot_limit[i] = DEFAULT_BOT_LIMIT;
    recalc_coef[i] = 1.0;
    cal_err[i] = 1;
    ref_impedance_1khz[i] = 1000.0;
    ref_impedance_80hz[i] = 1000.0;
    err_counter[i] = 0;
  }
  eepWriteSeq(EEP_ADDR_line_enabled, (unsigned char*) &line_enabled[0], sizeof(line_enabled));
  eepWriteSeq(EEP_ADDR_top_limit, (unsigned char*) &top_limit[0], sizeof(top_limit));
  eepWriteSeq(EEP_ADDR_bot_limit, (unsigned char*) &bot_limit[0], sizeof(bot_limit));
  eepWriteSeq(EEP_ADDR_recalc_coef, (unsigned char*) &recalc_coef[0], sizeof(recalc_coef));
  eepWriteSeq(EEP_ADDR_cal_err, (unsigned char*) &cal_err[0], sizeof(cal_err));
  eepWriteSeq(EEP_ADDR_ref_impedance_1khz, (unsigned char*) &ref_impedance_1khz[0], sizeof(ref_impedance_1khz));
  eepWriteSeq(EEP_ADDR_ref_impedance_80hz, (unsigned char*) &ref_impedance_80hz[0], sizeof(ref_impedance_80hz));
  eepWriteSeq(EEP_ADDR_err_counter, (unsigned char*) &err_counter[0], sizeof(err_counter));

  meas_interval = DEFAULT_MEAS_INTERVAL;
  eepWriteSeq(EEP_ADDR_meas_interval, (unsigned char*) &meas_interval, sizeof (meas_interval));

  dhcp_en = 0;
  eepWriteSeq(EEP_ADDR_dhcp_en, (unsigned char*) &dhcp_en, sizeof (dhcp_en));

  test_byte = TEST_BYTE;
  eepWriteSeq(EEP_ADDR_TEST_BYTE, (unsigned char*) &test_byte, sizeof (test_byte));
}

void strToIP(char *str, uint8_t *value) {
  /* ERROR CHECKING MISSED */
    size_t index = 0;
    
    memset((void*)value, 0, 4);
    
    while (*str) {
        if (isdigit(*str)) {
            value[index] *= 10;
            value[index] += *str - '0';
        } else {
            index++;
        }
        str++;
    }
    return;
}

