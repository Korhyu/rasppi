#include <sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

struct in_addr localInterface;
struct sockaddr_in groupSock;
int sd;

#define IP_ADDR "192.168.0.7"
#define MSJ_LENGTH 1024
#define MODO 1							//0 para enviar sin demora, 1 para enviar con demora
#define CANTIDAD_ENVIOS 100				//Cantidad de mensajes enviados


char databuf[MSJ_LENGTH] = "Mensaje de prueba Multicast";
int datalen = sizeof(databuf);
  

int main (int argc, char *argv[ ])
{

	/* Create a datagram socket on which to send. */
	sd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sd < 0)
	{
	    perror("Opening datagram socket error");
	    exit(1);
	}

	else
	    printf("Opening the datagram socket...OK.\n");

	/* Initialize the group sockaddr structure with a */
	/* group address of 225.1.1.1 and port 5555. */

	memset((char *) &groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
	groupSock.sin_port = htons(4321);

	/* Disable loopback so you do not receive your own datagrams.
	{
		char loopch = 0;
		if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0)
			{
			perror("Setting IP_MULTICAST_LOOP error");
			close(sd);
			exit(1);
			}

		else
			printf("Disabling the loopback...OK.\n");
	}
	*/
    
	/* Set local interface for outbound multicast datagrams. */
	/* The IP address specified must be associated with a local, */
	/* multicast capable interface. */
	localInterface.s_addr = inet_addr(IP_ADDR);
	if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
	{
	    perror("Setting local interface error");
	    exit(1);
	}

	else
	    printf("Setting the local interface...OK\n");

	/* Send a message to the multicast group specified by the*/
	/* groupSock sockaddr structure. */
	/*int datalen = 1024;*/
	struct timeval tv;
	long ticks;
	int cuenta=0;
	int cont=0;
	while(1)
	{
		gettimeofday(&tv, NULL);
		ticks= ((tv.tv_sec * 1000000 + tv.tv_usec));
		sprintf(databuf, "%ld", ticks);
		if(sendto(sd, databuf, sizeof(databuf), 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
		{
			perror("Sending datagram message error");
		}

		else
		{
	    	cuenta++;
		    if(MODO)
		    	printf("Enviando mensaje numero N %d ...OK\n", cuenta);	
		    else
		    	cont++;
		}


		ticks= ((tv.tv_sec * 1000000 + tv.tv_usec));
		printf( "%d : %ld \n", cuenta, ticks);

	    if(MODO)
	    {
			usleep(500000);
	    }
	    else
	    {
	    	usleep(2000000);
	    	if(cont = CANTIDAD_ENVIOS)
	    	{
	    		printf("Enviados %d mensajes\nFin del programa\n", cont);
	    		return 0;
	    	}
	    }
	}	
	/* Try the re-read from the socket if the loopback is not disable
	if(read(sd, databuf, sizeof(databuf)) < 0)
	{
	perror("Reading datagram message error\n");
	close(sd);
	exit(1);
	}
	else
	{
	printf("Reading datagram message from client...OK\n");
	printf("The message is: %s\n", databuf);
	}

	*/

	return 0;

}