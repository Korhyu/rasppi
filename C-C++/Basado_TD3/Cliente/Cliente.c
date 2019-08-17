#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define MSGBUFSIZE 256

int main(int argc, char *argv[])
{
    if (argc != 3) {
       printf("Command line args should be multicast group and port\n");
       printf("(e.g. for SSDP, `listener 239.255.255.250 1900`)\n");
       return 1;
    }

    char* group = argv[1]; // e.g. 239.255.255.250 for SSDP
    int port = atoi(argv[2]); // 0 if error, which is an invalid port

    // Creo un socket del tipo UDP
    //
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // Permito que muchos sockets usen el mismo puerto
    //
    int yes = 1;
    if (
        setsockopt(
            fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)
        ) < 0
    ){
       perror("Reusing ADDR failed");
       return 1;
    }

    // Configuro la direccion de destino
    //
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   // Es dinstinta al programa que envia
    addr.sin_port = htons(port);

    // Bindeo el socket
    //
    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    // use setsockopt() to request that the kernel join a multicast group
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (
        setsockopt(
            fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)
        ) < 0
    ){
        perror("setsockopt");
        return 1;
    }

    // now just enter a read-print loop
    //
    while (1) {
        char msgbuf[MSGBUFSIZE];
        int addrlen = sizeof(addr);
        int nbytes = recvfrom(
            fd,
            msgbuf,
            MSGBUFSIZE,
            0,
            (struct sockaddr *) &addr,
            &addrlen
        );
        if (nbytes >= 0)
        {
            sendto(sockfd, msgbuf, sizeof(msgbuf), MSG_DONTWAIT,
                (struct sockaddr *) &addr, addrlen);
        }
        else
        {
            perror("recvfrom");
            return 1;
        }
        
        msgbuf[nbytes] = '\0';
        puts(msgbuf);
     }
}
