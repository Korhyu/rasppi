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
#define EXAMPLE_GROUP "239.0.0.1"
#define DELAY_ENVIO 2
#define MSGBUFSIZE 200


int main(int argc, char *argv[])
{
    char* group;
    char N_IP[] = EXAMPLE_GROUP;
    char * auxiliarresto;
    struct timeval tv;
    int port;
    int nbytes[2];
    long resultado;
    long ticks[2];
    int delay_secs = DELAY_ENVIO;
    char mess_rec[MSGBUFSIZE];
    char mess_env[MSGBUFSIZE] = "Cliente 1";
    
    if (argc != 3)
    {
       printf("Ni IP ni puerto especificado\n");
       printf("Se asigna IP %s y %d\n", EXAMPLE_GROUP, EXAMPLE_PORT);

       group = N_IP;
       port = EXAMPLE_PORT;
       //return 1;
    }

    else
    {
        char* group = argv[1];      // Entra por agumento el IP
        port = atoi(argv[2]);       // Entra por agumento el puerto
    }
    
    // !!! If test requires, make these configurable via args
    //



    // Creo el socket UDP
    //
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    // Configuro el IP y puerto
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(group);
    addr.sin_port = htons(port);

    // Uso setsockopt() para pedirle al kernel unirse a un multicast group
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    
    if ( setsockopt( fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq) ) < 0 )
    {
        perror("setsockopt");
        return 1;
    }
    
     if(connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    { 
        printf("\n Error : Connect Failed \n"); 
        exit(0); 
    } 

    
    printf("Envio al servidor\n");
    nbytes[1] = sendto( fd, mess_env, sizeof(mess_env), 0, (struct sockaddr*) &addr, sizeof(addr) );
    
    sleep(1);

    // Envio en loop
    while (1) 
    {
        nbytes[0] = recvfrom( fd, (void *)mess_rec, sizeof(mess_rec), 0, (struct sockaddr*) &addr, &addrlen );
        printf("Chequeo Recibo\n");
        //resultado = strtol(mess_env, &auxiliarresto, 10);
        //printf("%s\n", mess_env);
        
        sleep(1);

        /*
        gettimeofday(&tv, NULL);
        ticks[0] = ((tv.tv_sec * 1000000 + tv.tv_usec));
        //sprintf(mess_env, &ticks[0], sizeof(ticks));
        */

        nbytes[1] = sendto( fd, mess_env, sizeof(mess_env), 0, (struct sockaddr*) &addr, sizeof(addr) );
        printf("Chequeo Envio\n");

        //printf("Recibido: %s \t a %ld ticks\n", mess_rec, ticks[0]);
        memset(mess_env,0,sizeof(mess_env));
        
        if (nbytes[0] < 0) {
            perror("recvfrom");
            return 1;
        }
        
        if (nbytes[1] < 0) {
            perror("sendto");
            return 1;
        }


        //sleep(delay_secs); // Unix sleep is seconds
     }


    return 0;
}