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
#include <math.h>
#include "defines.h"


void crear_datos (int);
int open_multicast_server();
int ch_enviados();							//Funcion que analiza el paquete recivido y devuelve un nibble con '1' donde se envia un canal
int send_msj(char*, int);
char * crear_encabezado(char*, char*);


struct in_addr localInterface;
struct sockaddr_in groupSock;
int sd;
char datos[MSJ_LENGTH - HEADER_SIZE - sizeof(FIN_TRAMA)];

FILE* fptr;



int main (int argc, char *argv[ ])
{
	/* Send a message to the multicast group specified by the*/
	/* groupSock sockaddr structure. */
	/*int datalen = 1024;*/
	struct timeval tv;
	long ticks;
	int i=0;
	int cont=0;
	char timestamp[16];
	char header[HEADER_SIZE];
	char buffer[MSJ_LENGTH] = "\0";
	char *posicion;

	open_multicast_server();
	crear_datos(DATA_LENGTH);

	fptr = fopen("send.txt", "w+");
	fclose(fptr);
	
	//Envio de datos
	while(1)
	{
		gettimeofday(&tv, NULL);
		ticks= ((tv.tv_sec * 1000000 + tv.tv_usec));
		sprintf(timestamp, "%ld", ticks);
		strcat(buffer, timestamp);
		strcat(buffer, "|");
		strcat(buffer, crear_encabezado(datos, header));
		strcat(buffer, "|");
		strcat(buffer, datos);
		strcat(buffer, FIN_TRAMA);


		/*
		if(sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
		{
			perror("Sending datagram message error");
		}
		else
		{
			i++;
			strcpy(buffer,"\0");
			
			//Impresion de cantidad de mensajes enviados (para ver si el servidor esta vivo)
			if( CANTIDAD_ENVIOS >= 1000)
			{
				if((i%(CANTIDAD_ENVIOS/10)) == 0)
				{
					printf("Mensaje n: %d de %d enviado\n",i, CANTIDAD_ENVIOS);
				}
			}

			if(i >= CANTIDAD_ENVIOS)
			{
				strcpy(buffer,FIN_TRANSM);
				if(sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
				{
					perror("Sending datagram message error");
				}
				printf("Fin de envios, enviados %d mensajes considerando el mensaje de fin\n",i+1);
				break;
			}
			usleep(DEMORA_ENVIO);
		}
		*/
		
		if (send_msj(buffer,sd) != 0)
		{
			i++;

			fptr = fopen("send.txt", "a+");
			fprintf(fptr, "%s\n", buffer);
			fclose(fptr);

			strcpy(buffer,"\0");
			
			//Impresion de cantidad de mensajes enviados (para ver si el servidor esta vivo)
			if( CANTIDAD_ENVIOS >= 1000)
			{
				if((i%(CANTIDAD_ENVIOS/10)) == 0)
				{
					printf("Mensaje n: %d de %d enviado\n",i, CANTIDAD_ENVIOS);
				}
			}

			if(i >= CANTIDAD_ENVIOS)
			{
				strcpy(buffer,FIN_TRANSM);
				if(sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
				{
					perror("Sending datagram message error");
				}
				printf("Fin de envios, enviados %d mensajes considerando el mensaje de fin\n",i+1);
				break;
			}
			usleep(DEMORA_ENVIO);
		}
		

	}	
	/* Try the re-read from the socket if the loopback is not disable
	if(read(sd, buffer, sizeof(buffer)) < 0)
	{
	perror("Reading datagram message error\n");
	close(sd);
	exit(1);
	}
	else
	{
	printf("Reading datagram message from client...OK\n");
	printf("The message is: %s\n", buffer);
	}

	*/

	printf("Fin del programa\n");
	return 0;
}

int send_msj (char* msj, int soc)
{	
	if(sendto(soc, msj, strlen(msj), 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
	{
		perror("Sending datagram message error");
	}
	else	
		return 1;
}

//Funcion para crear el venctor de datos a enviar
void crear_datos (int cant_datos)
{
	int i;
	for(i = 0; i < cant_datos; i++)
	{
		datos[i] = (i % 10) + 48;
	}
	*(datos+i) = '\0';
}

int ch_enviados()							//Funcion que analiza el paquete recivido y devuelve un nibble con '1' donde se envia un canal
{
	int ch;

	ch = 7;

	return ch;
}

char * crear_encabezado(char* mensaje, char* encabezado)
{
	/*Analizar si no es mejor dejar el timestamp en vez del cont_msj*/

	char log_msj[8];			//Envio la cantidad de caracteres del mensaje sin contar los encabezados
	char ch_env[8];				//Envia los canales que estan en el mensaje actuar
	char msj_id[8];				//Un identificador del numero de mensaje enviado
	static uint cont_msj = 0;

	//char encabezado[strlen(log_msj) + strlen(ch_env) + strlen(msj_id) + 4];
	/*
	if (((encabezado = malloc(sizeof(char) * (sizeof(log_msj) + sizeof(ch_env) + sizeof(msj_id) + 4)))) == NULL)
       return (NULL);
	*/

	sprintf(log_msj, "%ld", strlen(mensaje));		//Verifico el largo del string
	sprintf(ch_env, "%d", ch_enviados());					//Consigo los canales enviados *Harcodeados actualmente*
	sprintf(msj_id, "%d", cont_msj);			//Numero de identificacion del mensaje

	cont_msj++;
	cont_msj = cont_msj % 10000;			//Establezco un limite para el contador de mensajes

	strcpy(encabezado, "\0");
	strcat(encabezado, log_msj);
	strcat(encabezado, "|");
	strcat(encabezado, ch_env);
	strcat(encabezado, "|");
	strcat(encabezado, msj_id);

	return encabezado;
}


int open_multicast_server()
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
	groupSock.sin_addr.s_addr = inet_addr(IP_MCST);
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

	return sd;
}
