#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <linux/if_ether.h> // ETH_P_ALL
#include <linux/if_packet.h> // sockaddr_ll

int main(int argc, char **argv)
{
    int s;
    struct sockaddr_in src_addr;
    char packet[100] = {0};
    struct sockaddr_ll    sll = {0};

    if ((s = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) < 0) { // (1)
        perror("socket");
        return -1;
    }

    sll.sll_family        = AF_PACKET;
    sll.sll_ifindex        = 3; // Interface index (2)
    sll.sll_protocol    = htons(ETH_P_ALL);

    if( bind(s, (struct sockaddr *) &sll, sizeof(sll)) == -1 ) {
        perror("bind");
    }

    socklen_t *len = (socklen_t *)sizeof(src_addr);
    int fromlen = sizeof(src_addr);

    while(1) {
        if (recvfrom(s, &packet, sizeof(packet), 0,
                    (struct sockaddr *)&src_addr, (socklen_t *)&fromlen) < 0)
            perror("recvfrom");

        int i = sizeof(struct iphdr); // print the payload

        for(; i < sizeof(packet); i++) {
            printf("%02X ", (unsigned char) packet[i]);
        }

        printf("\n");
    }
    return 0;
}
