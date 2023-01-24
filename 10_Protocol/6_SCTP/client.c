//==============================include header file============================
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ren_unp.h"
//=============================================================================

//==============================define variable================================
#define BUFFERSIZE 8192
#define SERV_PORT 9877
#define LISTENQ 1024
#define MAXLINE 4096
#define SCTP_MAXLINE 800
#define SERV_MAX_SCTP_STRM 10
//=============================================================================

//==============================define function================================
void sctpstr_cli(FILE *fp,int sock_fd,struct sockaddr *to,socklen_t tolen);
void sctpstr_cli_echoall(FILE *fp,int sock_fd,struct sockaddr *to,socklen_t tolen);
void TurnAllNotification(int sock_fd);
//=============================================================================

//==============================main function==================================
int main(int argc,char **argv)
{
	int sock_fd;	// client所建立的socket
	struct sockaddr_in servaddr;	// 記錄server的sockaddr_in的相關資訊
	struct sctp_event_subscribe evnts;	// 設定要觀查的事件
	int echo_to_all = 0;		// 決定server是否有多個
	int ret_value = 0;
	
	// 沒有設定server的ip
	if (argc < 2) {
		printf("Missing host argument - use '%s host [echo]'\n",argv[0]);
		exit(-1);
	}
	// 設定多個server的ip
	if (argc > 2) {
		printf("Echoing messages to all streams\n");
		echo_to_all = 1;
	}

	// init setting
	sock_fd = socket(AF_INET,SOCK_SEQPACKET,IPPROTO_SCTP);	// 設定為sctp為one-to-many的型態
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	// servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	// 感覺這一行應該是多餘的
	servaddr.sin_port = htons(SERV_PORT);
	ret_value = inet_pton(AF_INET,argv[1],&servaddr.sin_addr);	// 把字串轉換成網路位元組序二進位值,當回傳為1時,表轉換成功
	if (ret_value != 1) {
		printf("translate server ip address error!!\n");
		exit(-1);
	}

	ret_value = connect(sock_fd,(struct sockaddr *) &servaddr,sizeof(servaddr));

	TurnAllNotification(sock_fd);

	if (ret_value == -1) {
		printf("connect error\n");
		exit(-1);
	}

	heartbeat_action(sock_fd,(struct sockaddr *) &servaddr,sizeof(servaddr),10);

	if (echo_to_all == 0) {
		// 此client只傳給一個server
		sctpstr_cli(stdin,sock_fd,(struct sockaddr *) &servaddr,sizeof(servaddr));
	}
	else {
		// 要設定傳給許多個server
		sctpstr_cli_echoall(stdin,sock_fd,(struct sockaddr *) NULL,0);
	}
	close(sock_fd);
	return 0;
}
//=============================================================================

//==============================function implemen==============================
void sctpstr_cli(FILE *fp,int sock_fd,struct sockaddr *to,socklen_t tolen)
{
	struct sockaddr_in peeraddr;	// client的相關位址資料
	struct sctp_sndrcvinfo sri;	// send and recv端的資料
	char sendline[MAXLINE],recvline[MAXLINE];
	socklen_t len;
	int out_sz,rd_sz;
	int msg_flags;
	int ret_value=0;

	bzero(&sri,sizeof(sri));
	// block住,等待client端輸入
	while (fgets(sendline,MAXLINE,fp) != NULL) {
		if (sendline[0] != '[') {
			printf("Error, line must be of the form '[streamnum]text'\n");
			continue;
		}
		// string to long integer,表示sendline這一個字串是base在10進位去轉換
		// 透過設定的stream number來傳送資料
		sri.sinfo_stream = strtol(&sendline[1],NULL,0);
		out_sz = strlen(sendline);	// 輸入字串的長度,用來記錄等一下要傳送幾個字元
		ret_value = sctp_sendmsg(sock_fd,sendline,out_sz,to,tolen,0,0,sri.sinfo_stream,0,0);
		if (ret_value == -1) {
			printf("sctp_sendmsg error\n");
			exit(-1);
		}

		len = sizeof(peeraddr);
		rd_sz = sctp_recvmsg(sock_fd,recvline,sizeof(recvline),(struct sockaddr *) &peeraddr,&len,&sri,&msg_flags);
		if (msg_flags & MSG_NOTIFICATION) {
			printf("I got a notification\n");
			continue;
		}
		printf("From str:%d seq:%d (assoc:0x%x):",sri.sinfo_stream,sri.sinfo_ssn,(u_int) sri.sinfo_assoc_id);
		printf("%.*s",rd_sz,recvline);
	}
}

void sctpstr_cli_echoall(FILE *fp,int sock_fd,struct sockaddr *to,socklen_t tolen)
{
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[SCTP_MAXLINE], recvline[SCTP_MAXLINE];
	socklen_t len;
	int rd_sz,i,strsz;
	int msg_flags;
	int ret_value=0;

	// the client initializes the sri structure used to set up the stream it will be sending and receiving from
	bzero(sendline,sizeof(sendline));
	bzero(&sri,sizeof(sri));
	while(fgets(sendline,SCTP_MAXLINE-9,fp) != NULL) {
		strsz = strlen(sendline);
		// delete the newline character that is at the end of the buffer
		if (sendline[strsz-1] == '\n') {
			sendline[strsz-1]='\0';
			strsz--;
		}
		// 傳送SERV_MAX_SCTP_STRM筆資料給server
		for (i=0;i<SERV_MAX_SCTP_STRM;i++) {
			snprintf(sendline+strsz,sizeof(sendline)-strsz,".msg.%d",i);
			ret_value = sctp_sendmsg(sock_fd,sendline,sizeof(sendline),to,tolen,0,0,i,0,0);
			if (ret_value == -1) {
				printf("sctp_sendmsg error\n");
				exit(-1);
			}
		}
		// 由server接收SERV_MAX_SCTP_STRM筆資料
		for (i=0;i<SERV_MAX_SCTP_STRM;i++) {
			len = sizeof(peeraddr);
			rd_sz = sctp_recvmsg(sock_fd,recvline,sizeof(recvline),(struct sockaddr *) &peeraddr,&len,&sri,&msg_flags);
			printf("From str:%d seq:%d (assoc:0x%x):",sri.sinfo_stream,sri.sinfo_ssn,(u_int) sri.sinfo_assoc_id);
			printf("%.*s\n",rd_sz,recvline);
		}
	}
}

void TurnAllNotification(int sock_fd)
{
	struct sctp_event_subscribe evnts;
	int ret_value;
	bzero(&evnts,sizeof(evnts));
	evnts.sctp_data_io_event=1;
	evnts.sctp_shutdown_event=1;
	evnts.sctp_association_event=1;
	evnts.sctp_address_event=1;
	evnts.sctp_send_failure_event=1;
#ifdef UN_MOD
	evnts.sctp_adaptation_layer_event=1;
#else
	evnts.sctp_adaption_layer_event=1;
#endif
	ret_value = setsockopt(sock_fd,IPPROTO_SCTP,SCTP_EVENTS,&evnts,sizeof(evnts));
	if (ret_value == -1) {
		printf("setsockopt error\n");
		exit(-1);
	}
}
