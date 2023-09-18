/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/httpser-netconn.c 
  * @author  MCD Application Team
  * @brief   Basic http server implementation using LwIP netconn API  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "lwip/apps/fs.h"
#include "string.h"
#include "httpserver-netconn.h"
#include "cmsis_os.h"
#include "dbg_uart.h"
#include "uart.h"
#include "main_logic.h"
#include "global_vars.h"
#include "eeprom_i2c.h"
#include "cmd_response.h"
#include "measurement.h"
#include "stats.h"
#include "encryption.h"

#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEBSERVER_THREAD_PRIO    ( osPriorityAboveNormal )
#define TAG(a,b) "<"a">"b"</"a">\r"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32_t nPageHits = 0;
typedef enum {GET = 0, POST = 1, NONE = 0xFF} tReqType;
uint8_t flag_need_reset = 0; // нужен программный сброс девайса
static portCHAR page_body[4096];

static const char respOK[] = "<resp><result>OK</result></resp>";
static const char respERR[] = "<resp><result>ERR</result></resp>";

/*
 TODO: как сделать авторизацию
 * страница \login, на неё попадаем через 307 Redirect, если в куках неверный логин/пароль
 * После ввода логина/пароля запоминаем сгенерированный хэш в куках и его везде будет браузер лепить ко всем запросам
 * 
 * Для усиления защиты делаем следующее: МК генерит случайное число, JS его принимает и с помощью него
 * кодирует пару логин/пароль и также её хранит в куках, соответственно для разлогина тупо меняем этот параметр в куках
 * В момент смены/задания пароля/логина они будут идти в открытом виде, либо можно слегка чем-нибудь закодировать, что можно легко раскодировать на МК
 * На всех страница сделать выход (обнуление cookie)
 */

// TODO: сделать частичное обновление страницы каждые 10 сек (измерения , ошибки)
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief serve tcp connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
const char content_len_string[] = "Content-Length:";
const char payload_divider_string[] = "\r\n\r\n";
static void http_server_serve(struct netconn *conn) 
{
  struct netbuf *inbuf;
  err_t recv_err;
  char *buf, *p;
  u16_t buflen;
  uint32_t content_len;
  struct fs_file file;
  tReqType req = NONE; // тип HTTP-запроса
  uint32_t url_offset; // с какого символа начинается адрес файла
  static char input_req_buf[INPUT_REQ_BUF_SIZE];
  uint8_t piece = 0;
  uint32_t estimated_packet_size = 0; // ожидаемая длина пакета
  uint32_t current_packet_size = 0; // текущая длина пакета
  
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
  netconn_set_recvtimeout(conn, NETCONN_RECV_TIMEOUT);

  current_packet_size = 0;
  estimated_packet_size = 0;
  input_req_buf[0] = '\0'; // обнуляем строку
  
  while(1) {
    if ((recv_err = netconn_recv(conn, &inbuf)) == ERR_OK) {
      netbuf_data(inbuf, (void**) &buf, &buflen);
      
      buf[buflen] = '\0'; // ставим нуль терминатор на всякий
      current_packet_size += buflen;

      if (piece == 0) {
        
        // определяем длину содержимого
        p = strstr(buf, content_len_string);
        if (p) {
          content_len = atoi(p + strlen(content_len_string));
        } else {
          content_len = 0;
        }
        
//        printf("BEGIN PIECE = %u\r", piece);
//        printf("Content-Length = %u\r", content_len);
//        printf(buf);
//        printf("END OF PIECE\r");
        
        if (content_len == 0) { // в запросе нет полезной нагрузки
//          printf("EXIT (content_len == 0)");
          break;
        } else {
          p = strstr(buf, payload_divider_string);
          // TODO: здесь тонкое место, расчет на то, что в первом куске придёт весь заголовок целиком, включая CRLFCRLF - разделитель для полезной нагрузки
          estimated_packet_size = (uint32_t)(p - buf) + strlen(payload_divider_string) + content_len;
//          printf("Piece_size = %u\r", estimated_packet_size);
//          printf("buflen = %u\r", buflen);
          
          if (buflen >= estimated_packet_size) { // весь запрос пришёл одним пакетом
//            printf("EXIT (buflen >= piece_size)");
            break;
          }
        }
        
        // если сюда дошли, значит запрос состоит из более, чем одного куска
        strncat(input_req_buf, buf, sizeof(input_req_buf)); // сохраняем первый кусок
        
        piece++;
        netbuf_delete(inbuf);
//        puts("netbuf_delete(inbuf)");
        continue; // принимаем следующий кусок
      } else {  // piece >= 1
//        printf("BEGIN PIECE = %u\r", piece);
//        printf(buf);
//        printf("END OF PIECE\r");
        strncat(input_req_buf, buf, sizeof (input_req_buf)); // сохраняем следующий кусок
        if (current_packet_size >= estimated_packet_size) {
//          printf("EXIT (current_packet_size >= estimated_packet_size)");
          buf = input_req_buf;
          buflen = strlen(input_req_buf);
          break;
        } else {
          piece++;
          netbuf_delete(inbuf);
//          puts("netbuf_delete(inbuf)");
          continue; // ждём следующий кусок
        }
      }
    } else {
      printf("Error: recv_err = %i\r", recv_err);
      break;
    }
  }
//  MEM_STATS_DISPLAY();
    
    if (netconn_err(conn) == ERR_OK) 
    {
      /* Is this an HTTP GET command or POST?*/
      if ((buflen >=4) && (strncmp(buf, "GET ", 4) == 0)) {
        req = GET;
        url_offset = 4;
        puts("\r");
        dbgUartPutsUntil(buf, '\n');
      } else if ((buflen >=5) &&(strncmp(buf, "POST ", 5) == 0)) {
        req = POST;
        url_offset = 5;
        puts("\r");
        printf(buf);
      } else {
        req = NONE;
      }
            
      if (req != NONE)
      {
          if (strncmp(&buf[url_offset],"/logo.png",9)==0)
          {
            fs_open(&file, "/logo.png"); 
            netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
            fs_close(&file);
          }
          else if (strncmp(&buf[url_offset],"/favicon.ico",12)==0)
          {
            fs_open(&file, "/favicon.ico"); 
            netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
            fs_close(&file);
          }
          else if((strncmp(&buf[url_offset], "/mainpage.html", 14) == 0)||(strncmp(&buf[url_offset], "/ ", 2) == 0)) 
          {
            if (checkAuthorization(conn, buf)) {
              fs_open(&file, "/mainpage.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
          }
          else if(strncmp(&buf[url_offset], "/netsettings.html", 14) == 0) 
          {
            if (checkAuthorization(conn, buf)) {
              fs_open(&file, "/netsettings.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
          }
          else if(strncmp(&buf[url_offset], "/administration.html", 20) == 0) 
          {
            if (checkAuthorization(conn, buf)) {
              fs_open(&file, "/administration.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
          }
          else if(strncmp(&buf[url_offset], "/login.html", 11) == 0) 
          {
            fs_open(&file, "/login.html"); 
            netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
            fs_close(&file);
          }
          else if(strncmp(&buf[url_offset], "/mchp.js", 8) == 0) 
          {
            fs_open(&file, "/mchp.js"); 
            netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
            fs_close(&file);
          }
          else if(strncmp(&buf[url_offset], "/R6LME02.log", 11) == 0) 
          {
            if (checkAuthorization(conn, buf)) {
              sendLogFile(conn, buf);
            }
          }
          else if(strncmp(&buf[url_offset], "/status.xml", 11) == 0) 
          {
            sendStatusXML(conn, buf);
          }
          else if(strncmp(&buf[url_offset], "/netsettings.xml", 16) == 0) 
          {
            sendNetsettingsXML(conn, buf);
          }
          else if(strncmp(&buf[url_offset], "/credentials.xml", 16) == 0) 
          {
            sendCredentialsXML(conn, buf);
          }
          else if(strncmp(&buf[url_offset], "/save.xml", 9) == 0) 
          {
            if (checkAuthorization(conn, buf)) {
              actionSave(conn, buf);
            }
          }
          else if(strncmp(&buf[url_offset], "/start_cal.xml", 14) == 0) 
          {
            if (checkAuthorization(conn, buf)) {
              actionStartCal(conn, buf);
            }
          }
          else if(strncmp(&buf[url_offset], "/start_meas.xml", 15) == 0) 
          {
            if (checkAuthorization(conn, buf)) {
              actionStartMeas(conn, buf);
            }
          }
          else if(strncmp(&buf[url_offset], "/save_netsettings.xml", 20) == 0) 
          {
            if (checkAuthorization(conn, buf)) {
              actionSaveNetsettings(conn, buf);
            }
          }
          else if(strncmp(&buf[url_offset], "/save_credentials.xml", 20) == 0) 
          {
            if (checkAuthorization(conn, buf)) {
              actionSaveCredentials(conn, buf);
            }
          }
          else if(strncmp(&buf[url_offset], "/check_credentials.xml", 21) == 0) 
          {
            actionCheckCredentials(conn, buf);
          }
          else if(strncmp(&buf[url_offset], "/reset_to_defaults.xml", 21) == 0) 
          {
            actionResetToDefaults(conn, buf);
          }
          else 
          {
            /* Load Error page */
            fs_open(&file, "/404.html");
            netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
            fs_close(&file);
          }
      }
    }
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);
//  puts("netconn_close(conn)");
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
//  puts("netbuf_delete(inbuf)");
}


/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void http_server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, 80);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
          //puts("accept_err == OK");
          /* serve connection */
          http_server_serve(newconn);

          /* delete connection */
          netconn_delete(newconn);
          //puts("netconn_delete(newconn)");
        }

        if (flag_need_reset) {
          flag_need_reset = 0;
          osDelay(3000);
          NVIC_SystemReset();
        }
        
      }
    }
  }
}

/**
  * @brief  Initialize the HTTP server (start its thread) 
  * @param  none
  * @retval None
  */
void http_server_netconn_init()
{  
  sys_thread_new("HTTP", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}

uint8_t checkAuthorization(struct netconn *conn, char *buf) {
  const char redirect_head[] =
  "HTTP/1.1 307 Unauthorized\r\n"
  "location: /login.html\r\n\r\n";
  
  if (strstr(buf, login_password_hash) != NULL) {
    return 1;
  } else {
    dbgUartPuts("Not authorized! Redirect to login.html\r");
    netconn_write(conn, redirect_head, strlen(redirect_head), NETCONN_NOCOPY);
    return 0;
  }
}

void sendError(struct netconn *conn, char *buf) {
  page_body[0] = '\0';
  strcat((char *)page_body, "HTTP/1.1 413 Payload Too Large\r\n\r\n");
  netconn_write(conn, page_body, strlen(page_body), NETCONN_COPY);
}
void sendStatusXML(struct netconn *conn, char *buf) {
  struct fs_file file;
  uint32_t num_chars = 0, i;

  // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/xml
  fs_open(&file, "/status.xml");
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);
  
  // шлём содержимое
  num_chars += sprintf(&page_body[num_chars],"<response>\r");
  for(i = 0; i < NUM_LINES; i++) {
    num_chars += sprintf(&page_body[num_chars],TAG("line_name%u", "%s"), i+1, line_name[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("ref_imp_1khz%u", "%0.2f Ом"), i+1, ref_impedance_1khz[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("ref_imp_80hz%u", "%0.2f Ом"), i+1, ref_impedance_80hz[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("last_meas%u", "%0.2f Ом"), i+1, last_meas[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("cal_err%u", "%u"), i+1, cal_err[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("imp_err%u", "%u"), i+1, imp_err[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("gnd_err%u", "%u"), i+1, gnd_err[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("sht_err%u", "%u"), i+1, sht_err[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("line_enabled%u", "%u"), i+1, line_enabled[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("discrepancy%u", "%i"), i+1, discrepancy[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("top_limit%u", "%u"), i+1, top_limit[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("bot_limit%i", "%i"), i+1, bot_limit[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("err_counter%u", "%u"), i+1, err_counter[i], i+1);
    num_chars += sprintf(&page_body[num_chars],TAG("meas_interval", "%u"), meas_interval);
  }
  num_chars += sprintf(&page_body[num_chars],"</response>");
  
  netconn_write(conn, page_body, strlen(page_body), NETCONN_NOCOPY);
  
  sprintf(str, "sendStatusXML: %u bytes\r", num_chars);
  dbgUartPuts(str);
  if (num_chars >= sizeof(page_body)) dbgUartPuts("ERROR: page_body[] size exceeded!\r");
}
void sendNetsettingsXML(struct netconn *conn, char *buf) {
  struct fs_file file;
  uint32_t num_chars = 0;

  // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/xml
  fs_open(&file, "/status.xml"); // имя файла не имеет значения, заголовок у всех xml одинаковый
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);
  
  // шлём содержимое
  num_chars += sprintf(&page_body[num_chars],"<response>\r");
  num_chars += sprintf(&page_body[num_chars],TAG("loc_ip", "%u.%u.%u.%u"), IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  num_chars += sprintf(&page_body[num_chars],TAG("net_mask", "%u.%u.%u.%u"), NETMASK_ADDRESS[0], NETMASK_ADDRESS[1], NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  num_chars += sprintf(&page_body[num_chars],TAG("net_gate", "%u.%u.%u.%u"), GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
  num_chars += sprintf(&page_body[num_chars],TAG("dhcp_en", "%u"), dhcp_en);
  num_chars += sprintf(&page_body[num_chars],"</response>");
  
  netconn_write(conn, page_body, strlen(page_body), NETCONN_NOCOPY);
  
  sprintf(str, "sendNetsettingsXML: %u bytes\r", num_chars);
  dbgUartPuts(str);
  if (num_chars >= sizeof(page_body)) dbgUartPuts("ERROR: page_body[] size exceeded!\r");
}
void sendCredentialsXML(struct netconn *conn, char *buf) {
  struct fs_file file;
  uint32_t num_chars = 0;

  // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/xml
  fs_open(&file, "/status.xml"); // имя файла не имеет значения, заголовок у всех xml одинаковый
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);
  
  // TODO: засылать причины ошибки блока LME (горит красный СИД ERR)
  // шлём содержимое
  num_chars += sprintf(&page_body[num_chars],"<response>\r");
  num_chars += sprintf(&page_body[num_chars],TAG("firm_ver", "%s"), FIRMWARE_VERSION);
  num_chars += sprintf(&page_body[num_chars],"</response>");
  
  netconn_write(conn, page_body, strlen(page_body), NETCONN_NOCOPY);
  
  sprintf(str, "sendCredentialsXML: %u bytes\r", num_chars);
  dbgUartPuts(str);
  if (num_chars >= sizeof(page_body)) dbgUartPuts("ERROR: page_body[] size exceeded!\r");
}

void sendLogFile(struct netconn *conn, char *buf) {
  struct fs_file file;

  // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/plane
  fs_open(&file, "/R6LME02.log");
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);

  makeLogFile();
  netconn_write(conn, log_file, strlen(log_file), NETCONN_NOCOPY);

  printf("sendLogFile: %u bytes\r", strlen(log_file));
}

// ищет, с какого элемента строки str начинается значение поля field
// возвращает NULL, если не найдено
char* findField(char* str, const char* field) {
  char *res;
  res = strstr(str, field);
  if (res != NULL) res = res + strlen(field) + 1; // +1 - разделитель (не проверяем)
  return res;
}
// ищет строковое поле field в строке in_str и помещает его в out_str
// возвращает -1, если поле не найдено
// в противном случае - длину полученной строки
int16_t getStrField(char* in_str, const char* field, char* out_str, uint16_t out_str_size) {
  char* res;
  uint16_t i = 0;
  
  res = findField(in_str, field);
  
  if (res == NULL) return -1;
  
  while(i < out_str_size-1) {
    if (res[i] == RIGHT_SEPAR) break;
    else out_str[i] = res[i];
    i++;
  }
  out_str[i] = '\0'; // нуль-терминатор
  return i;
}
// ищет числовое поле field в строке in_str и помещает его в out_val
// возвращает -1, если поле не найдено
// в противном случае +1
int16_t getNumField(char* in_str, const char* field, int32_t* out_val) {
  char* res;
  uint16_t i = 0;
  char out_str[16];
  
  res = findField(in_str, field);
  
  if (res == NULL) return -1;
  
  while(i < 16-1) {
    if (res[i] == RIGHT_SEPAR) break;
    else out_str[i] = res[i];
    i++;
  }
  if (i > 10 ) return -1; // переполнение int32_t
  out_str[i] = '\0'; // нуль-терминатор
  
  *out_val = atoi(out_str);
  
  return 1; // OK
}

void actionSave(struct netconn *conn, char *buf) {
  struct fs_file file;
  char field_name[64];
  static const char respOK[] = "<resp><result>OK</result></resp>";
  static const char respERR[] = "<resp><result>ERR</result></resp>";
  static const char divider[] = "save_parameters\n";
  char* payload;
  const char* resp = respOK;
  uint16_t i;
  int32_t t;
  
  payload = findField(buf, divider);
  
//  puts("actionSave raw data:\r");
//  puts(payload);
  
  puts("\r\rProcessed data:\r");
  
  if (payload != NULL) {
    strcpy(field_name, "Lx_NAME");
    for(i = 0; i < NUM_LINES; i++) {
      field_name[1] = '0' + i + 1; // x -> 1,2,3,...
      if (getStrField(payload, field_name, line_name[i], LINE_NAME_LEN) != -1) {
        printf("%s = %s\r", field_name, line_name[i]);
      } else {
        resp = respERR;
        printf("ERROR: field \"%s\" not found\r", field_name);
      }
    }
    eepWriteSeq(EEP_ADDR_line_name, (unsigned char*) &line_name[0][0], sizeof(line_name));
    
    strcpy(field_name, "Lx_LINE_EN");
    for(i = 0; i < NUM_LINES; i++) {
      field_name[1] = '0' + i + 1; // x -> 1,2,3,...
      if (getNumField(payload, field_name, &t) != -1) {
        if ((t == 1)&&(line_enabled[i] == 0)) err_counter[i] = 0; // сбрасываем счетчик ошибок при включении мониторинга линии
        line_enabled[i] = t;
        printf("%s = %u\r", field_name, line_enabled[i]);
      } else {
        resp = respERR;
        printf("ERROR: field \"%s\" not found\r", field_name);
      }
    }
    eepWriteSeq(EEP_ADDR_line_enabled, (unsigned char*) &line_enabled[0], sizeof(line_enabled));
    eepWriteSeq(EEP_ADDR_err_counter, (unsigned char*) &err_counter[0], sizeof(err_counter));
    
    strcpy(field_name, "Lx_TOP_LIMIT");
    for(i = 0; i < NUM_LINES; i++) {
      field_name[1] = '0' + i + 1; // x -> 1,2,3,...
      if (getNumField(payload, field_name, &t) != -1) {
        top_limit[i] = t;
        printf("%s = %u\r", field_name, top_limit[i]);
      } else {
        resp = respERR;
        printf("ERROR: field \"%s\" not found\r", field_name);
      }
    }
    eepWriteSeq(EEP_ADDR_top_limit, (unsigned char*) &top_limit[0], sizeof (top_limit));
    
    strcpy(field_name, "Lx_BOT_LIMIT");
    for(i = 0; i < NUM_LINES; i++) {
      field_name[1] = '0' + i + 1; // x -> 1,2,3,...
      if (getNumField(payload, field_name, &t) != -1) {
        bot_limit[i] = t;
        printf("%s = %i\r", field_name, bot_limit[i]);
      } else {
        resp = respERR;
        printf("ERROR: field \"%s\" not found\r", field_name);
      }
    }
    eepWriteSeq(EEP_ADDR_bot_limit, (unsigned char*) &bot_limit[0], sizeof (bot_limit));

    strcpy(field_name, "MEAS_INTERVAL");
    if (getNumField(payload, field_name, &t) != -1) {
      meas_interval = t;
      printf("%s = %u\r", field_name, meas_interval);
    } else {
      resp = respERR;
      printf("ERROR: field \"%s\" not found\r", field_name);
    }
    eepWriteSeq(EEP_ADDR_meas_interval, (unsigned char*) &meas_interval, sizeof(meas_interval));
    
  } else {
    resp = respERR;
  }
    // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/xml
  fs_open(&file, "/save.xml");
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);

  netconn_write(conn, resp, strlen(resp), NETCONN_COPY);
}

void actionStartMeas(struct netconn *conn, char *buf) {
  struct fs_file file;
  const char* resp = respOK;
  uint8_t res = 0;
  char* p_start_par;
  uint32_t line;
  
  p_start_par = strstr(buf, "?line=");
  sscanf(p_start_par, "?line=%u", &line);
  printf("actionStartMeas. line = %u\r", (unsigned int)line);

  res = measLine((uint8_t)line);
  
  if (res == 0) resp = respERR;
  else resp = respOK;
  
      // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/xml
  fs_open(&file, "/save.xml");
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);

  netconn_write(conn, resp, strlen(resp), NETCONN_COPY);
  
  puts("Send start_meas.xml");
}

void actionStartCal(struct netconn *conn, char *buf) {
  struct fs_file file;
  const char* resp = respOK;
  uint8_t res = 0;
  char* p_start_par;
  uint32_t line;

  p_start_par = strstr(buf, "?line=");
  sscanf(p_start_par, "?line=%u", &line);
  printf("actionStartCal. line = %u\r", (unsigned int) line);

  res = calibrLine((uint8_t) line);

  if (res == 0) resp = respERR;
  else resp = respOK;

  // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/xml
  fs_open(&file, "/save.xml");
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);

  netconn_write(conn, resp, strlen(resp), NETCONN_COPY);

  puts("Send start_cal.xml");
}

void actionSaveNetsettings(struct netconn *conn, char *buf) {
  struct fs_file file;
  char field_name[64];
  char str_ip[12+3+1];
  static const char divider[] = "save_netsettings\n";
  char* payload;
  const char* resp = respOK;
  int32_t t;
  uint8_t old_ip[4], old_net_mask[4], old_gate_addr[4], old_dhcp_en;
  
  payload = findField(buf, divider);
  
  puts("actionSaveNetsettings raw data:\r");
  puts(payload);
  
  puts("\r\rProcessed data:\r");
  
  if (payload != NULL) {
    
    memcpy(old_ip, IP_ADDRESS, sizeof(old_ip));
    memcpy(old_net_mask, NETMASK_ADDRESS, sizeof(old_net_mask));
    memcpy(old_gate_addr, GATEWAY_ADDRESS, sizeof(old_gate_addr));
    old_dhcp_en = dhcp_en;
    
    strcpy(field_name, "loc_ip");
    if (getStrField(payload, field_name, str_ip, sizeof(str_ip)) != -1) {
      strToIP(str_ip, IP_ADDRESS);
      printf("%s = %u.%u.%u.%u\r", field_name, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    } else {
      resp = respERR;
      printf("ERROR: field \"%s\" not found\r", field_name);
    }
    eepWriteSeq(EEP_ADDR_IP_ADDRESS, (unsigned char*) &IP_ADDRESS, sizeof(IP_ADDRESS));

    strcpy(field_name, "net_mask");
    if (getStrField(payload, field_name, str_ip, sizeof (str_ip)) != -1) {
      strToIP(str_ip, NETMASK_ADDRESS);
      printf("%s = %u.%u.%u.%u\r", field_name, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1], NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
    } else {
      resp = respERR;
      printf("ERROR: field \"%s\" not found\r", field_name);
    }
    eepWriteSeq(EEP_ADDR_NETMASK_ADDRESS, (unsigned char*) &NETMASK_ADDRESS, sizeof (NETMASK_ADDRESS));

    strcpy(field_name, "net_gate");
    if (getStrField(payload, field_name, str_ip, sizeof (str_ip)) != -1) {
      strToIP(str_ip, GATEWAY_ADDRESS);
      printf("%s = %u.%u.%u.%u\r", field_name, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
    } else {
      resp = respERR;
      printf("ERROR: field \"%s\" not found\r", field_name);
    }
    eepWriteSeq(EEP_ADDR_GATEWAY_ADDRESS, (unsigned char*) &GATEWAY_ADDRESS, sizeof (GATEWAY_ADDRESS));

    strcpy(field_name, "dhcp_en");
    if (getNumField(payload, field_name, &t) != -1) {
      dhcp_en = t;
      printf("%s = %i\r", field_name, t);
    } else {
      resp = respERR;
      printf("ERROR: field \"%s\" not found\r", field_name);
    }
    eepWriteSeq(EEP_ADDR_dhcp_en, (unsigned char*) &dhcp_en, sizeof (dhcp_en));

    if ((memcmp(old_ip, IP_ADDRESS, sizeof(old_ip)) != 0)||(memcmp(old_net_mask, NETMASK_ADDRESS, sizeof(old_net_mask)) != 0)||(memcmp(old_gate_addr, GATEWAY_ADDRESS, sizeof(old_gate_addr)) != 0) || (old_dhcp_en != dhcp_en)) {
      flag_need_reset = 1; // перезагружаем девайс после всех формальностей
    }
  } else {
    resp = respERR;
  }
    // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/xml
  fs_open(&file, "/save.xml");
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);

  netconn_write(conn, resp, strlen(resp), NETCONN_COPY);
}
void actionSaveCredentials(struct netconn *conn, char *buf) {
  struct fs_file file;
  char field_name[64];
  static const char divider[] = "save_credentials\n";
  char* payload;
  const char* resp = respOK;
  char rec_login[LOGIN_LEN];
  char rec_password[PASSWORD_LEN];
  
  payload = findField(buf, divider);
  
  puts("actionSaveCredentials raw data:\r");
  puts(payload);
  
  puts("\r\rProcessed data:\r");
  
  if (payload != NULL) {
    // TODO: логин/пароль здесь ходят в открытом виде, желательно закодировать хотя бы в base64, а лучше еще с помощью случайного числа
    strcpy(field_name, "login");
    if (getStrField(payload, field_name, rec_login, sizeof(rec_login)) != -1) {
      printf("%s = %s\r", field_name, rec_login);
    } else {
      resp = respERR;
      printf("ERROR: field \"%s\" not found\r", field_name);
    }
    strcpy(field_name, "pass");
    if (getStrField(payload, field_name, rec_password, sizeof(rec_password)) != -1) {
      printf("%s = %s\r", field_name, rec_password);
    } else {
      resp = respERR;
      printf("ERROR: field \"%s\" not found\r", field_name);
    }
    
    if ((strcmp(login, rec_login) == 0) && (strcmp(password, rec_password) == 0)) { // логин/пароль верные
      
      strcpy(field_name, "new_login");
      if (getStrField(payload, field_name, login, sizeof (login)) != -1) {
        printf("%s = %s\r", field_name, login);
      } else {
        resp = respERR;
        printf("ERROR: field \"%s\" not found\r", field_name);
      }
      strcpy(field_name, "new_pass");
      if (getStrField(payload, field_name, password, sizeof (password)) != -1) {
        printf("%s = %s\r", field_name, password);
      } else {
        resp = respERR;
        printf("ERROR: field \"%s\" not found\r", field_name);
      }
      eepWriteSeq(EEP_ADDR_login, (unsigned char*) &login, sizeof(login));
      eepWriteSeq(EEP_ADDR_password, (unsigned char*) &password, sizeof(password));
      makeLoginPassword(login_password, login, password); // делаем пару логин-пароль
      encode(login_password_hash, login_password); // кодируем её
    } else {  // логин/пароль неверные
      resp = respERR;
    }

  } else {
    resp = respERR;
  }
    // шлём заголовок 
  //HTTP/1.0 200 OK
  //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
  //Content-type: text/xml
  fs_open(&file, "/save.xml");
  netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
  fs_close(&file);

  netconn_write(conn, resp, strlen(resp), NETCONN_COPY);
}

void actionCheckCredentials(struct netconn *conn, char *buf) {
    struct fs_file file;
    char field_name[64];
    static const char divider[] = "check_credentials\n";
    char* payload;
    const char* resp = respERR;
    char sid[MAX_LOGIN_PASSWORD_BASE64_LEN];
    
    payload = findField(buf, divider);
    
    puts("actionCheckCredentials raw data:\r");
    puts(payload);
    
    puts("\r\rProcessed data:\r");
    
    if (payload != NULL) {
        
      strcpy(field_name, "sid");
      if (getStrField(payload, field_name, sid, sizeof(sid)) != -1) {
        printf("%s = %s\r", field_name, sid);
        if (strcmp(login_password_hash, sid) == 0) resp = respOK;
        else resp = respERR;
      } else {
        resp = respERR;
        printf("ERROR: field \"%s\" not found\r", field_name);
      }
    } else {
      resp = respERR;
    }

    // шлём заголовок 
    //HTTP/1.0 200 OK
    //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
    //Content-type: text/xml
    fs_open(&file, "/save.xml");
    netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
    fs_close(&file);
  
    netconn_write(conn, resp, strlen(resp), NETCONN_COPY);
}
void actionResetToDefaults(struct netconn *conn, char *buf) {
    struct fs_file file;
    const char* resp = respOK;
    
    puts("actionResetToDefaults\r");
    
    flag_reset_to_defaults = 1;

    // шлём заголовок 
    //HTTP/1.0 200 OK
    //Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
    //Content-type: text/xml
    fs_open(&file, "/save.xml");
    netconn_write(conn, (const unsigned char*) (file.data), (size_t) file.len, NETCONN_NOCOPY);
    fs_close(&file);
  
    netconn_write(conn, resp, strlen(resp), NETCONN_COPY);
}


