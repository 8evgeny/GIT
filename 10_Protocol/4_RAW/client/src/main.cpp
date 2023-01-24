#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>

#define DEST "127.0.0.1"
#define REMOTE_PORT 1001
using std::chrono::system_clock;

int main(int argc, char **argv)
{

 int s;
 struct sockaddr_in dst_addr;
 char packet[100];

 struct iphdr *ip = (struct iphdr *)packet;

 if((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
  perror("error:");
  exit(EXIT_FAILURE);
 }

 dst_addr.sin_family = AF_INET;
 dst_addr.sin_port = htons(REMOTE_PORT);
 dst_addr.sin_port = 0;
 inet_pton(AF_INET, DEST, (struct in_addr *)&dst_addr.sin_addr.s_addr);
 memset(dst_addr.sin_zero, 0, sizeof(dst_addr.sin_zero));

 memset(packet, 0x33, sizeof(packet));   /* payload will be all As */

 ip->ihl = 5;
 ip->version = 4;
 ip->tos = 0;
 ip->tot_len = htons(100);
 ip->frag_off = 0;  /* NF */
 ip->ttl = 64;
 ip->protocol = IPPROTO_RAW; /* this has to be IPPROTO_RAW */
 ip->check = 0;
 ip->saddr = dst_addr.sin_addr.s_addr;
 ip->daddr = dst_addr.sin_addr.s_addr;

 while(42)
 {
     for (int i = 1;i<256;++i)
     {
         memset(packet, i, sizeof(packet));
         ip->ihl = 5;
         ip->version = 4;
         ip->tos = 0;
         ip->tot_len = htons(40);
         ip->frag_off = 0;  /* NF */
         ip->ttl = 64;
         ip->protocol = IPPROTO_RAW; /* this has to be IPPROTO_RAW */
         ip->check = 0;
         ip->saddr = dst_addr.sin_addr.s_addr;
         ip->daddr = dst_addr.sin_addr.s_addr;

         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
         if (sendto(s, packet, sizeof(packet), 0, (struct sockaddr *)&dst_addr, (socklen_t)sizeof(dst_addr)) < 0)
             perror("uh oh:");
     }
 }
 return(0);
}
