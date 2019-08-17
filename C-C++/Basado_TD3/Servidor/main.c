#include sys/types.h
#include sys/socket.h
#include netinet/in.h
#include arpa/inet.h
#include time.h
#include string.h
#include stdio.h
#include stdlib.h


#include "aux.h"

#define EXAMPLE_PORT 6000
#define EXAMPLE_GROUP "239.0.0.1"



int main (int argc, char *argv[])
{
    int sockfd, addrlen, cnt;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[256];
    char message[50];


    if (argc < 2)
    {
         fprintf(stderr,"ERROR, no se indico un puerto\n");
         exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        perror("ERROR abriendo socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(EXAMPLE_PORT);
    addrlen = sizeof(serv_addr);

    serv_addr.sin_addr.s_addr = inet_addr(EXAMPLE_GROUP);

    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        return 1;
    }


    while (1)
    {
        time_t t = time(0);
        sprintf(message, "time is %-24.24s", ctime(&t));
        printf("Cnt=%d\n", cnt);
        printf("sending : %s\n", message);
        cnt = sendto(sockfd, message, sizeof(message), MSG_DONTWAIT,
                (struct sockaddr *) &serv_addr, addrlen);
        printf("Cnt=%d\n", cnt);
        if (cnt < 0)
        {
            printf("No se envio el mensaje\n");
            perror("sendto\n");
            exit(1);
        }
        printf("Espera...");
        sleep(5);
    }

    return 0;
}