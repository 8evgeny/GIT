#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <time.h>

#define PORT_NUM 9124
#define MAX_BUFFER 1024
#define LOCALTIME_STREAM 0
#define GMT_STREAM 1
#define NUM_MSG 2
#define NUM_PARAM 2

int main(int argc, char *argv[]){
    int connection_socket, in, i, flags;
    struct sockaddr_in servaddr;
    struct sctp_sndrcvinfo sndrcvinfo;
    struct sctp_event_subscribe events;
    char buffer[MAX_BUFFER+1];


    //input params
    if(argc != NUM_PARAM) {
        printf("Program args error: expected SERVER_ADDR\n");
        exit( EXIT_FAILURE );
    }

    //SCTP socket
    connection_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP );
    if( connection_socket < 0 ) {
      perror( "Socket errror" );
      exit( EXIT_FAILURE );
    }

    //Specify the peer endpoint to which we'll connect
    memset( (void *)&servaddr, 0, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NUM);
    servaddr.sin_addr.s_addr = inet_addr( argv[1] );

    //Connect to the server
    in = connect( connection_socket, (struct sockaddr *)&servaddr, sizeof(servaddr) );
    if( in < 0 ) {
      perror( "Connect error" );
      exit( EXIT_FAILURE );
    }

    //Enable receipt of SCTP Snd/Rcv Data via sctp_recvmsg
    memset( (void *)&events, 0, sizeof(events) );
    events.sctp_data_io_event = 1;
    setsockopt( connection_socket, IPPROTO_SCTP, SCTP_EVENTS, (const void *)&events, sizeof(events) );

    //no flags
    flags = 0;

    //Expect two messages from the peer
    for (i = 0 ; i < NUM_MSG ; i++) {
        memset( (void *)&buffer, 0, sizeof(buffer) );
    printf("Trying to rcv ...\n");
    //SCTP rcv
        in = sctp_recvmsg( connection_socket, (void *)buffer, sizeof(buffer), (struct sockaddr *)NULL, 0, &sndrcvinfo, &flags );
    if( in < 0 ) {
      printf("errno : %d\n", errno);
      perror( "SCTP_rcv error" );
      exit( EXIT_FAILURE );
    }
    printf("Received %d bytes\n", in);
        //Null terminate the incoming string
        buffer[in] = 0;

        if (sndrcvinfo.sinfo_stream == LOCALTIME_STREAM) {
            printf("Stream: %d \n %s\n", sndrcvinfo.sinfo_stream, buffer);
        } else if(sndrcvinfo.sinfo_stream == GMT_STREAM) {
            printf("Stream: %d \n %s\n", sndrcvinfo.sinfo_stream, buffer);
        }

    }

    //Close our socket and exit
    close(connection_socket);

    return 0;
}
