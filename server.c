#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define EXAMPLE_PORT 54000
#define EXAMPLE_IP "239.0.0.1"
#define DELAY_ENVIO 5
#define MSGBUFSIZE 200

int main(int argc, char *argv[])
{
    char* n_ip;
    char N_IP[] = EXAMPLE_IP;
    struct timeval tv;
    int port;
    int nbytes[2];
    long ticks[2];
    int size_addr;
    int clientes_conectados=0;


    uint delay_secs = DELAY_ENVIO;
    char message[MSGBUFSIZE] = "Server enviando";
    char messagr[MSGBUFSIZE] = "Server reciviendo";
    char t_enviado[MSGBUFSIZE];
    char t_recivido[MSGBUFSIZE];


    if (argc != 3)
    {
       printf("Ni IP ni puerto especificado\n");
       printf("Se asigna IP %s y %d\n", EXAMPLE_IP, EXAMPLE_PORT);

       n_ip = N_IP;
       port = EXAMPLE_PORT;
       //return 1;
    }

    else
    {
        char* n_ip = argv[1];      // e.g. 239.255.255.250 for SSDP
        port = atoi(argv[2]);       // 0 if error, which is an invalid port
    }
    


    // Creo un socket UDP
    //
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }


    // Configuro la IP y el puerto
    struct sockaddr_in addr;
    size_addr = sizeof(addr);
    memset(&addr, 0, size_addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(n_ip);
    addr.sin_port = htons(port);

    // Uso setsockopt() para solicitarle al kernel unirse a un multicast n_ip
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(n_ip);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq) ) < 0 )
    {
        perror("setsockopt");
        return 1;
    }

    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
    {
        perror("bind");
        return 1;
    }
    
    while (1)
    {
        if(clientes_conectados < 1)
        {
            printf("Esperando cliente\n");
            nbytes[1] = recvfrom( fd, (void *)message, sizeof(message), 0, (struct sockaddr*) &addr, &size_addr );
            printf("Cliente Conectado\n");
            strcpy(message, "!");
            clientes_conectados ++;
            sleep(2);
        }

        nbytes[0] = sendto( fd, message, strlen(message), 0, (struct sockaddr*) &addr, sizeof(addr) );
        printf("Chequeo Envio %s\n", message);

        gettimeofday(&tv, NULL);
        ticks[0] = ((tv.tv_sec * 1000000 + tv.tv_usec));
        //sprintf(message, "%ld", ticks[0]);

       
        if (nbytes[0] < 0)
        {
            perror("sendto");
            return 1;
        }


        nbytes[1] = recvfrom( fd, (void *)message, sizeof(message), 0, (struct sockaddr*) &addr, &size_addr );
        printf("Chequeo Recibo %s\n", message);

        gettimeofday(&tv, NULL);
        ticks[1] = ((tv.tv_sec * 1000000 + tv.tv_usec));
        //sprintf(message, "%ld", ticks);
        
        if (nbytes[1] < 0)
        {
            perror("recvfrom");
            return 1;
        }

        // Imprimo la diferencia
        printf("Enviado a: %ld\nRecibido a: %ld\n", ticks[0], ticks[1]);

        /* Imprimo el eco
        nbytes[1] = recvfrom( fd, (void *)messagr, sizeof(messagr), 0, (struct sockaddr*) &addr, sizeof(addr) );
        if (nbytes[1] < 0)
        {
            perror("recvfrom");
            return 1;
        }

        printf("R: %s\n", messagr);
        */
        sleep(delay_secs); // Unix sleep is seconds    
     }


    return 0;
}