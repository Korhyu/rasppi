#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define EXAMPLE_PORT 4000
#define EXAMPLE_GROUP "239.0.0.1"
#define DELAY_ENVIO 2
#define MSGBUFSIZE 200


int main(int argc, char *argv[])
{
    char* group;
    char N_IP[] = EXAMPLE_GROUP;
    struct timeval tv;
    int port;
    int nbytes[2];
    long resultado;
    
    if (argc != 3)
    {
       printf("Ni IP ni puerto especificado\n");
       printf("Se asigna IP %s y %d\n", EXAMPLE_GROUP, EXAMPLE_PORT);

       group = &N_IP;
       port = EXAMPLE_PORT;
       //return 1;
    }

    else
    {
        char* group = argv[1];      // e.g. 239.255.255.250 for SSDP
        port = atoi(argv[2]);       // 0 if error, which is an invalid port
    }
    
    // !!! If test requires, make these configurable via args
    //
    const int delay_secs = DELAY_ENVIO;
    const char message[MSGBUFSIZE] = "Cliente conectado";


    // create what looks like an ordinary UDP socket
    //
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // set up destination address
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(group);
    addr.sin_port = htons(port);

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

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr))==-1)
    {
        perror("bind failed");
        exit(1);
    }

    //sendto( fd, message, strlen(message), 0, (struct sockaddr*) &addr, sizeof(addr) );
    //printf("%s\n", message);

    // now just sendto() our destination!
    while (1) 
    {
        nbytes[0] = recvfrom( fd, (void *)message, sizeof(message), 0, (struct sockaddr*) &addr, &addrlen );
        resultado = strtol(message, &message, 10);
        //printf("%s\n", message);
        nbytes[1] = sendto( fd, resultado, strlen(resultado), 0, (struct sockaddr*) &addr, sizeof(addr) );
        printf("%s\n", resultado);
        memset(message,0,strlen(message));
        if (nbytes[0] < 0) {
            perror("recvfrom");
            return 1;
        }
        
        if (nbytes[1] < 0) {
            perror("sendto");
            return 1;
        }


        sleep(delay_secs); // Unix sleep is seconds
     }


    return 0;
}