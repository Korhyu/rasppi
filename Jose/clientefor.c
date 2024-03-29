/* Receiver/client multicast Datagram example. */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "defines.h"
   
struct sockaddr_in localSock;
struct ip_mreq group;
int sd;
int datalen;
char databuf[MSJ_LENGTH];

FILE * fptr;


int main(int argc, char *argv[])
{
	/* Create a datagram socket on which to receive. */
	sd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sd < 0)
	{
		perror("Opening datagram socket error");
		exit(1);
	}
	else
		printf("Opening datagram socket....OK.\n");

	/* Enable SO_REUSEADDR to allow multiple instances of this */
	/* application to receive copies of the multicast datagrams. */
	int reuse = 1;
	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
	{
		perror("Setting SO_REUSEADDR error");
		close(sd);
		exit(1);
	}
	else
		printf("Setting SO_REUSEADDR...OK.\n");
    

	/* Bind to the proper port number with the IP address */

	/* specified as INADDR_ANY. */

	memset((char *) &localSock, 0, sizeof(localSock));
	localSock.sin_family = AF_INET;
	localSock.sin_port = htons(4321);
	localSock.sin_addr.s_addr = INADDR_ANY;

	if(bind(sd, (struct sockaddr*)&localSock, sizeof(localSock)))
	{
		perror("Binding datagram socket error");
		close(sd);
		exit(1);
	}

	else
		printf("Binding datagram socket...OK.\n");

	/* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
	/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
	/* called for each local interface over which the multicast */
	/* datagrams are to be received. */

	group.imr_multiaddr.s_addr = inet_addr(IP_MCST);
	group.imr_interface.s_addr = inet_addr(IP_ADDR);

	if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
	{
		perror("Adding multicast group error");
		close(sd);
		exit(1);
	}
	else
		printf("Adding multicast group...OK.\n");

	/* Read from the socket. */
	datalen = sizeof(databuf);

	struct timeval tv;
	long ticks[CANTIDAD_ENVIOS];
	long cuenta=0;
	int cont;

	// Si existe el archivo lo piso para crear uno nuevo
	fptr = fopen("outputf.txt", "w+");
	fclose(fptr);

	for (cont=1; cont < CANTIDAD_ENVIOS; cont++)
	{
		if(read(sd, databuf, datalen) < 0)
		{
			perror("Reading datagram message error");
			close(sd);
			exit(1);
		}
		else
		{
			cuenta++;
			gettimeofday(&tv, NULL);
			ticks[cont]= ((tv.tv_sec * 1000000 + tv.tv_usec));
			if(IMPRIMIR == 1)
			{
				//printf("Reading N %ld...OK.\n", cuenta);
				printf("Mensaje enviado por el servidor: \"%s\"\n", databuf);
				printf( "Mensaje numero %ld, \t ticks%ld \n", cuenta, ticks[cont]);
			}
		}
	}
	
	//Genero el LOG
	fptr = fopen("outputf.txt", "a+");
	for (cont=1; cont < CANTIDAD_ENVIOS; cont++)
	{
		fprintf(fptr, "%s,%ld\n",databuf, ticks[cont]);
	}
	fclose(fptr);

	printf("Recividos %d mensajes\n", cont);

	return 0;
}