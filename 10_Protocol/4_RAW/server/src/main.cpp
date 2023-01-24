#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
 int s;
 struct sockaddr_in src_addr;
 char packet[50];

 if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
  perror("error:");
  exit(EXIT_FAILURE);
 }

 memset(packet, 0, sizeof(packet));
 socklen_t *len = (socklen_t *)sizeof(src_addr);
 int fromlen = sizeof(src_addr);

 while(42)
 {
  if (recvfrom(s, &packet, sizeof(packet), 0, (struct sockaddr *)&src_addr, (socklen_t *)&fromlen) < 0)
   perror("uh oh:");

//  int i = sizeof(struct iphdr);
  int i = 0;
  for(; i < sizeof(packet); i++)
  {
   printf("%X", packet[i]);
  }
  printf("\n");
 }
 return(0);
}
