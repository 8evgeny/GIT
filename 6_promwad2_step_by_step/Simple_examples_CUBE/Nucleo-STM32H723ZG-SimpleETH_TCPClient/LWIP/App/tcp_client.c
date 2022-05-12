/*
 * Dzialac dziala, potestowac potestowalem wystarczy RTCek dziala po wyjsciu ze Standby i maszyna stanow
 * tam co kolwiek
 * nie jest to dobrze zrobione ale no dziabac dziabie i wgl i w szczegole :)
 */







#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <lwip/tcp.h>
#include "tcp_client.h"
#include "lwip/prot/dhcp.h"
#include "lwip/dhcp.h"
#include "lwip.h"
#include "rtc.h"
#include "usart.h"

#define TIME_FRM "%Y.%m.%d %H:%M:%S"
#define PARAM_FRM "%"U32_F" %"U32_F
static const char ok_msg[]           = "OK";
static const char set_msg[]          = "SET";
static const char bye_msg[]          = "BYE";
static const char error_msg[]        = "ERROR";
static const char param_msg_frm[]    = "PARAM "PARAM_FRM"\r\n";
static const char ok_param_msg_frm[] = "OK "PARAM_FRM"\r\n";
static const char bye_msg_frm[]      = "BYE "TIME_FRM"\r\n";
static const char error_msg_frm[]    = "ERROR "TIME_FRM"\r\n";
static const char set_param_frm[]    = PARAM_FRM;

#define POLL_PER_SECOND             2
#define MSG_SIZE                    32
#define CONNECTION_TIMEOUT          0
#define STANDBY_TIME                1
#define PARAM_NUMBER                2
#define DEFAULT_CONNECTION_TIMEOUT  30
#define DEFAULT_STANDBY_TIME        20
#define MIN_CONNECTION_TIMEOUT      5
#define MAX_CONNECTION_TIMEOUT      600
#define MIN_STANDBY_TIME            1
#define MAX_STANDBY_TIME            1800

struct state {
  err_t (* function)(struct state *, struct tcp_pcb *);
  uint32_t param[PARAM_NUMBER];
  uint32_t timer;
  unsigned idx;
  char msg[MSG_SIZE];
};

static err_t connect_callback(void *, struct tcp_pcb *, err_t);
static err_t recv_callback(void *, struct tcp_pcb *,
                           struct pbuf *, err_t);
static err_t poll_callback(void *, struct tcp_pcb *);
static void  conn_err_callback(void *, err_t);
static err_t tcp_exit(struct state *, struct tcp_pcb *);
static err_t tcp_write_time_msg(struct tcp_pcb *, const char *);
static err_t tcp_write_param_msg(struct tcp_pcb *, struct state *,
                                 const char *);
static err_t StateAutomaton(struct state *, struct tcp_pcb *,
                            struct pbuf *);
static err_t HelloState(struct state *, struct tcp_pcb *);
static err_t ParamState(struct state *, struct tcp_pcb *);



ip_addr_t DestIPaddr;
struct tcp_pcb *echoclient_pcb;
extern struct netif gnetif;


int TCPclientStart(uint16_t port) {
	ip_addr_t DestIPaddr;
	IP4_ADDR( &DestIPaddr, 192, 168, 0, 11 );


	 struct tcp_pcb *pcb;
	  struct state *state;
	  err_t err;

	  state = mem_malloc(sizeof(struct state));
	  if (state == NULL)

	    return -1;
	  state->function = HelloState;
	  /* TODO: Przeczytać ustawienia z rejestrów zapasowych. */
	  state->param[CONNECTION_TIMEOUT] = DEFAULT_CONNECTION_TIMEOUT;
	  state->param[STANDBY_TIME] = DEFAULT_STANDBY_TIME;
	  state->timer = state->param[CONNECTION_TIMEOUT];
	  state->idx = 0;


	  uint32_t Timeoutdhcp=HAL_GetTick();
	  struct dhcp *dhcp;
	  dhcp = netif_dhcp_data(&gnetif);
	  while(dhcp->state != DHCP_STATE_BOUND ) //blokujaco do oporu az sie polaczy... XD
	  {						
		  MX_LWIP_Process ();  //wiadomo libka musi dzialac wczesniej se ja zblokowalem
		  	  	  	  	  //dobrze ze sie szybko kapnolem
		  	if( (HAL_GetTick()-Timeoutdhcp) > 10000 )  //timeout 10sek
		  	{
		  		break;
		  	}
	  }			//Ja nie wiem czego ST tego nie umieszcza np w przerwaniu od systicka :/	  	  
	  	  	  	  	  	  //no Najlepiej to RTOSik odzielny task i po zawodach :) i tak w RTOSie moze to robia nwm nie sprawdzalem
	  	  	  	  	  	  	  	

	  /* create new tcp pcb */
	 pcb = tcp_new();

	  if (pcb == NULL) {
	    mem_free(state);
	    return -1;
	  }

	  tcp_arg(pcb, state);
	  tcp_err(pcb, conn_err_callback);
	  tcp_recv(pcb, recv_callback);
	  tcp_poll(pcb, poll_callback, POLL_PER_SECOND);

	  err = tcp_connect(pcb, &DestIPaddr, port, connect_callback);

	  if (err != ERR_OK) {
	     mem_free(state);
	     /* Trzeba zwolnić pamięć, poprzednio było tcp_abandon(pcb, 0); */
	     tcp_close(pcb);
	     return -1;
	   }
	  return 0;
}

err_t connect_callback(void *arg, struct tcp_pcb *pcb, err_t err) {
return	tcp_write_time_msg(pcb, "A");
}

#define ERR_EXIT 100

err_t recv_callback(void *arg, struct tcp_pcb *pcb,
                    struct pbuf *p, err_t err) {
  /* Z analizy kodu wynika, że zawsze err == ERR_OK.
  if (err != ERR_OK)
    return err;
  */
  if (p) {
    /* Odebrano dane. */
    tcp_recved(pcb, p->tot_len);
    err = StateAutomaton(arg, pcb, p);
    if (err == ERR_OK || err == ERR_EXIT)
      pbuf_free(p);
    if (err == ERR_EXIT) {
      err = tcp_exit(arg, pcb);   //jak zwrocono ERR_EXIT zamykam polaczenie i idem spac na 5sek...
    }
  }
  else {
    /* Druga strona zamknęła połączenie. */
    err = tcp_exit(arg, pcb);
  }
  return err;
}

err_t poll_callback(void *arg, struct tcp_pcb *pcb) {
  struct state *state = arg;

  if (--(state->timer) <= 0) {
    tcp_write_time_msg(pcb, error_msg_frm);
    return tcp_exit(arg, pcb);
  }
  return ERR_OK;
}

void conn_err_callback(void *arg, err_t err) {
 // struct state *state = arg;

  /* TODO: Zapisać ustawienia do rejestrów zapasowych. */
  /* Nie musimy zwalniać pamięci, bo i tak resetujemy. Przed
     zaśnieciem należy poczekać na zakończenie transmisji. */
	EnterStandbyByMe(5);
}

err_t tcp_exit(struct state *state, struct tcp_pcb *pcb) {
  /* Nie musimy zwalniać pamięci, bo i tak resetujemy. */
  if (tcp_close(pcb) != ERR_OK) {
    /* Funkcja tcp_abort woła funkcję conn_err_callback. */
    tcp_abort(pcb);
    return ERR_ABRT;
  }
  else {
    /* TODO: Zapisać ustawienia do rejestrów zapasowych. */
		EnterStandbyByMe(5);
    return ERR_OK;
  }
}

err_t tcp_write_time_msg(struct tcp_pcb *pcb, const char *format) {

	RTC_TimeTypeDef RtcTime;
  RTC_DateTypeDef RtcDate;
  uint16_t Milliseconds;

  uint8_t Message[64];
  uint8_t MessageLen;

	  HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN);
	 Milliseconds = ((RtcTime.SecondFraction-RtcTime.SubSeconds)/((float)RtcTime.SecondFraction+1) * 100);
	  HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN);
	  MessageLen = sprintf((char*)Message," HELLO: Date: %02d.%02d.20%02d Time: %02d:%02d:%02d:%02d\n\r", RtcDate.Date,
	 		 		 	RtcDate.Month, RtcDate.Year, RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds, Milliseconds);
  return tcp_write(pcb, Message, MessageLen, TCP_WRITE_FLAG_COPY);
}

err_t tcp_write_param_msg(struct tcp_pcb *pcb, struct state *s,
                          const char *format) {
  char msg[MSG_SIZE];
  size_t size;

  size = snprintf(msg, MSG_SIZE, format,
                  s->param[CONNECTION_TIMEOUT],
                  s->param[STANDBY_TIME]);

  if (size >= MSG_SIZE)
    size = MSG_SIZE - 1;
  return tcp_write(pcb, msg, size, TCP_WRITE_FLAG_COPY);
}

err_t StateAutomaton(struct state *s, struct tcp_pcb *pcb,
                     struct pbuf *p) {
  s->timer = s->param[CONNECTION_TIMEOUT];
  for (;;) {
    uint8_t *c = (uint8_t *)p->payload;
    uint16_t i;
    err_t err;

    for (i = 0; i < p->len; ++i) {
      if (c[i] == '\n') {
        s->msg[s->idx++] = c[i];
        s->msg[s->idx] = '\0';
        s->idx = 0;
        err = s->function(s, pcb);
        if (err != ERR_OK)
          return err;
      }
      /* Muszą się jeszcze zmieścić dwa znaki: '\n' i '\0'. */
      else if (s->idx < MSG_SIZE - 2)
        s->msg[s->idx++] = c[i];
    }
    if (p->len == p->tot_len)
      break;
    else
      p = p->next;
  }
  return ERR_OK;
}

#define msglen(s) (sizeof(s) - 1)
#define msgncmp(s1, s2, n) \
						(strncmp(s1, s2, n) == 0 && isspace((int)s1[n]))

err_t HelloState(struct state *state, struct tcp_pcb *pcb) {
  if (msgncmp(state->msg, ok_msg, msglen(ok_msg))) {
    state->function = ParamState;
    return tcp_write_param_msg(pcb, state, param_msg_frm);
  }
  else if (msgncmp(state->msg, set_msg, msglen(set_msg))) {


	  RTC_DateTypeDef DateToUpdate={0};
	  RTC_TimeTypeDef sTime={0};

		  	sTime.Hours=	  atoi( (char*)((uint8_t*)state->msg+15) );
		  	sTime.Minutes=	  atoi( (char*)((uint8_t*)state->msg+18) );
		  	sTime.Seconds=	  atoi( (char*)((uint8_t*)state->msg+21) );

	  	DateToUpdate.Date=	  atoi( (char*)((uint8_t*)state->msg+12) );
	  	DateToUpdate.Month=	  atoi( (char*)((uint8_t*)state->msg+9) );
	  	DateToUpdate.WeekDay=	1;
	  	DateToUpdate.Year=atoi( (char*)((uint8_t*)state->msg+6) );
		SetRTC(&sTime,&DateToUpdate);
		tcp_write_time_msg(pcb, "A");
		return ERR_EXIT;
  }
  else if (msgncmp(state->msg, bye_msg, msglen(bye_msg)) ||
           msgncmp(state->msg, error_msg, msglen(error_msg)))
    return ERR_EXIT;
  else {
    tcp_write_time_msg(pcb, error_msg_frm);
    return ERR_EXIT;
  }
}

err_t ParamState(struct state *state, struct tcp_pcb *pcb) {  // to nie dziala nie chcialo mi sie naprawiac
																	//ale ogolna idea jest - mozna naprawic dla chetnych
  if (msgncmp(state->msg, ok_msg, msglen(ok_msg))) {
    tcp_write_time_msg(pcb, bye_msg_frm);
  }
  else if (msgncmp(state->msg, set_msg, msglen(set_msg))) {
    uint32_t timeout, sleep_time;
    int ret;

    ret = sscanf(state->msg + msglen(set_msg), set_param_frm,
                 &timeout, &sleep_time);

    if (ret != 2 ||
        timeout < MIN_CONNECTION_TIMEOUT ||
        timeout > MAX_CONNECTION_TIMEOUT ||
        sleep_time < MIN_STANDBY_TIME ||
        sleep_time > MAX_STANDBY_TIME) {
      tcp_write_time_msg(pcb, error_msg_frm);
    }
    else {
      state->param[CONNECTION_TIMEOUT] = timeout;
      state->param[STANDBY_TIME] = sleep_time;
      tcp_write_param_msg(pcb, state, ok_param_msg_frm);
      tcp_write_time_msg(pcb, bye_msg_frm);
    }
  }
  else if (!msgncmp(state->msg, bye_msg, msglen(bye_msg)) &&
           !msgncmp(state->msg, error_msg, msglen(error_msg)))
    tcp_write_time_msg(pcb, error_msg_frm);
  return ERR_EXIT;
}
