#ifndef FLAGS_H
#define FLAGS_H

#include <stdint.h>

#include "log.h"

#define FIRMWARE_VERSION        "1.02"

#define INPUT_REQ_BUF_SIZE      4096    // размер входного буфера под GET/POST запрос
#define NETCONN_RECV_TIMEOUT    5000 // таймаут ожидания TCP пакета при вызове netconn_recv()

#define DEFAULT_LOGIN           "admin"
#define DEFAULT_PASSWORD        "admin"

#define DEFAULT_IP0             192
#define DEFAULT_IP1             168
#define DEFAULT_IP2             0
#define DEFAULT_IP3             10

#define DEFAULT_NETMASK0        255
#define DEFAULT_NETMASK1        255
#define DEFAULT_NETMASK2        255
#define DEFAULT_NETMASK3        0

#define DEFAULT_GATEWAY0        192
#define DEFAULT_GATEWAY1        168
#define DEFAULT_GATEWAY2        0
#define DEFAULT_GATEWAY3        149

#define DEFAULT_LINE_NAME       "Линия " // + номер линии

#define DEFAULT_LINE_EN_DIS     1 // состояние линии (line_enabled) по-умолчанию (1 - включена, 0 - выключена)

#define DEFAULT_TOP_LIMIT       10 // %
#define DEFAULT_BOT_LIMIT       -10 // %

#define DEFAULT_MEAS_INTERVAL   600 // сек.

#define LOGIN_LEN                       (31*2+2) // длина логина плюс нуль-терминатор, 1 байт для выравнивания
#define PASSWORD_LEN                    (31*2+2) // длина пароля плюс нуль-терминатор, 1 байт для выравнивания
#define IP_SIZE                         4 // размер в байтах для IP-адреса, маски подсети и адреса шлюза
#define NUM_LINES                       6 // количество линий
#define LINE_NAME_LEN                   (31*2+2) // длина имени линии, включая 0-терминатор, 1 байт для выравнивания Note: т.к. у нас строки в UTF-8, то места надо в 2 раза больше


#define MAX_LOGIN_PASSWORD_COUPLE_LEN       (LOGIN_LEN+PASSWORD_LEN+1+1) // плюс двоеточие, плюс нуль-терминатор
#define MAX_LOGIN_PASSWORD_BASE64_LEN       (MAX_LOGIN_PASSWORD_COUPLE_LEN*4/3+4+1)

// разделители для формата файлов xml от ПК в МК
#define         LEFT_SEPAR              '\x01'
#define         MIDDLE_SEPAR            '\x02'
#define         RIGHT_SEPAR             '\n'

extern char str[256]; // строка для отладочного вывода

// все нижеследующие параметры дублируются в EEPROM
// размеры менять только вместе с адресами!
// размеры массивов прописывать даже для extern, иначе не будет работать sizeof
//--------------------------------------------------------------------- EEPROM Vars
extern uint8_t dhcp_en; // DHCP включен
extern char login[LOGIN_LEN];
extern char password[PASSWORD_LEN];
extern uint8_t IP_ADDRESS[4]; // IP адрес
extern uint8_t NETMASK_ADDRESS[4]; // маска подсети
extern uint8_t GATEWAY_ADDRESS[4]; // адрес шлюза
extern char line_name[NUM_LINES][LINE_NAME_LEN]; // имена линий
extern char line_enabled[NUM_LINES]; // 1 - линия включена, 0 - линия выключена
extern int16_t top_limit[NUM_LINES]; // Верхний порог отклонения,% 
extern int16_t bot_limit[NUM_LINES]; // Нижний порог отклонения,%
extern uint32_t meas_interval; // Текущий интервал измерения, сек
extern float recalc_coef[NUM_LINES]; // коэффициенты пересчета импеданса с 1 кГц на 80 Гц (не используется)
extern uint8_t cal_err[NUM_LINES]; // ошибка калибровки (калибровка ни разу не была произведена)
extern float ref_impedance_1khz[NUM_LINES];
extern float ref_impedance_80hz[NUM_LINES];
extern uint32_t err_counter[NUM_LINES]; // счетчик ошибок
//--------------------------------------------------------------------- END EEPROM Vars

#define ERR_NO_DATA         2   // состояние ошибки "нет данных", т.е. невозможно сказать, есть данная ошибка или нет

extern float last_meas[NUM_LINES];
extern uint8_t imp_err[NUM_LINES]; // ошибка импеданса
extern uint8_t gnd_err[NUM_LINES]; // ошибка по замыканию на землю
extern uint8_t sht_err[NUM_LINES]; // ошибка по КЗ линии
extern int32_t discrepancy[NUM_LINES]; // Текущее отклонение импеданса,%
extern uint8_t MK_out_data[NUM_LINES]; // состояния выходов ошибок на МК
extern uint8_t line_control_data[NUM_LINES]; // состояния линий 1-усилитель подключен, 0-усилитель отключен

extern char login_password[MAX_LOGIN_PASSWORD_COUPLE_LEN];
extern char login_password_hash[MAX_LOGIN_PASSWORD_BASE64_LEN];
extern uint8_t flag_login_password_error; // ошибка логина/пароля (напр., слишком длинные)

extern uint8_t err_eeprom_fail;
extern uint8_t err_no_conn_with_MK_board; // ошибка: нет связи с платой МК

extern uint8_t flag_force_start; // принудительный запуск измерения
extern uint8_t flag_reset_to_defaults;

// лежат в EEPROM по адресам:
#define EEP_ADDR_login                          0
#define EEP_ADDR_password                       (EEP_ADDR_login + sizeof(login))
#define EEP_ADDR_IP_ADDRESS                     (EEP_ADDR_password + sizeof(password))
#define EEP_ADDR_NETMASK_ADDRESS                (EEP_ADDR_IP_ADDRESS + sizeof(IP_ADDRESS))
#define EEP_ADDR_GATEWAY_ADDRESS                (EEP_ADDR_NETMASK_ADDRESS + sizeof(NETMASK_ADDRESS))
#define EEP_ADDR_line_name                      (EEP_ADDR_GATEWAY_ADDRESS + sizeof(GATEWAY_ADDRESS))
#define EEP_ADDR_line_enabled                   (EEP_ADDR_line_name + sizeof(line_name))
#define EEP_ADDR_top_limit                      (EEP_ADDR_line_enabled + sizeof(line_enabled))
#define EEP_ADDR_bot_limit                      (EEP_ADDR_top_limit + sizeof(top_limit))
#define EEP_ADDR_meas_interval                  (EEP_ADDR_bot_limit + sizeof(bot_limit))
#define EEP_ADDR_dhcp_en                        (EEP_ADDR_meas_interval + sizeof(meas_interval))
#define EEP_ADDR_cal_err                        (EEP_ADDR_dhcp_en + sizeof(dhcp_en))
#define EEP_ADDR_recalc_coef                    (EEP_ADDR_cal_err + sizeof(cal_err))
#define EEP_ADDR_ref_impedance_1khz             (EEP_ADDR_recalc_coef + sizeof(recalc_coef))
#define EEP_ADDR_ref_impedance_80hz             (EEP_ADDR_ref_impedance_1khz + sizeof(ref_impedance_1khz))
#define EEP_ADDR_err_counter                    (EEP_ADDR_ref_impedance_80hz + sizeof(ref_impedance_80hz))
#define EEP_ADDR_TEST_BYTE                      (EEP_ADDR_err_counter + sizeof(err_counter)) // должен разделять переменные и лог

#define EEP_ADDR_LOG                            (EEP_ADDR_TEST_BYTE + 1)

#define TEST_BYTE           0xAB

// для лога
#define EEP_SIZE            8192
#define LOG_RECORD_SIZE     (sizeof(tLogRecord)) // размер одной записи лога, для одной линии
//#define LOG_SIZE            (((EEP_SIZE-EEP_ADDR_TEST_BYTE)/LOG_RECORD_SIZE)*LOG_RECORD_SIZE) // байт (лог на всю свободную память)
#define LOG_SIZE            (LOG_RECORD_SIZE * (100+1)) // лог на 100 записей (так в ТЗ) одна запись максимум 60 байт, см размер log_file[]
#define LOG_NUM_RECORDS     (LOG_SIZE/LOG_RECORD_SIZE) // записей, включая запись с маркером конца

void eepLoadVars(void);
void eepWriteDefaults(void);
void saveLastState(uint8_t last_st);
void strToIP(char *str, uint8_t *value);

#endif
