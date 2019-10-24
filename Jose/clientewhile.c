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
   

int open_multicast_client();					//Establece la conecion multicast cliente
int get_timetag(char* , char*);					//Extrae el timetag del mensaje recivido
int get_header(char*, char*, char*, char*);		//Extrae el header del mensaje recivido
int get_data(char*, char*);						//Remueve los caracteres de fin de trama
int recv_msj(char*, int);						//Recive el msj desde el socket indicado

struct sockaddr_in localSock;
struct ip_mreq group;
int sd;

FILE * fptr;

int main(int argc, char *argv[])
{
	/* Read from the socket. */
	struct timeval tv;
	long ticks;
	int cuenta=0;
	char header[HEADER_SIZE];						//String donde almaceno los datos del Header para luego analizarlos
	char buffer[MSJ_LENGTH];						//Buffer de mensaje recibido
	char bufferp[MSJ_LENGTH];						//Buffer de mensaje recibido sin los caracteres de fin de trama
	char data[MSJ_LENGTH - HEADER_SIZE] = "\0";		//string donde se almacenan los datos
	char timetag[17];								//String donde almaceno el time tag
	
	// Si existe el archivo lo piso para crear uno nuevo
	fptr = fopen("recive.txt", "w+");
	fclose(fptr);
	fptr = fopen("output.txt", "w+");
	fclose(fptr);


	open_multicast_client();

	while (1)
	{
		if(read(sd, buffer, sizeof(buffer)) < 0)
		{
			perror("Reading datagram message error");
			close(sd);
			exit(1);
		}
		else
		{
			gettimeofday(&tv, NULL);
			ticks= ((tv.tv_sec * 1000000 + tv.tv_usec));
			cuenta++;

			fptr = fopen("recive.txt", "a+");
			fprintf(fptr, "%s\n",buffer);
			fclose(fptr);

			if(IMPRIMIR == 1)
			{
				printf("Reading N %d...OK.\n", cuenta);
				printf("Mensaje enviado por el servidor: \"%s\"\n", buffer);
				printf( "%d : %ld \n", cuenta, ticks);
			}

			get_data(buffer, bufferp);
			get_header(bufferp, timetag, header, data);
			
			
			fptr = fopen("output.txt", "a+");
			fprintf(fptr, "%s,%ld,%s,%s\n",timetag, ticks, header, data);
			fclose(fptr);
			
			if (strcmp(buffer,FIN_TRANSM) == 0)
			{
				printf("Fin de transmicion, recividos %d mensajes\n", cuenta);
				return 0;
			}
		}
	}
	return 0;
}


int get_header(char* buf, char* ttp, char* hep, char* dat)
{
	int i;
	int posp = 0;
	int posu = 0;
	int cont = 0;


	//Busco la posicion del primer pipe y guardo la informacion del timetag
	for(i=0; *(buf+i) != '|'; i++)
	{
		*(ttp+i) = *(buf+i);
	}
	*(ttp+i) = '\0';
	posp = i;

	//Busco la posicion del ultimo pipe
	//Revisar si esta busqueda no esta al pedo y queda fija por alguno de los defines
	for(i = sizeof(hep)+sizeof(ttp)+10; *(buf+i) != '|'; i--)
	{
		
	}
	posu = i;
	
	//Copio desde el primero hasta el ultimo caracter en el string header
	for(i = posp; i <= posu ; i++)
	{
		*(hep+cont) = *(buf+i);
		cont++;
	}
	*(hep+cont) = '\0';

	
	//Copio los datos
	for(i = posu ; i < (strlen(buf)- posu) ; i++)
	{
		*(dat+i-posu) = *(buf+i+1);
	}
	*(dat+i-posu) = '\0';

	return cont;
}


int get_timetag(char* data, char* ttp)
{
	int sizett = sizeof(*ttp);

	strncpy(ttp, data, 16);						//Copio los prieros 16 caracteres que son el timestamp
	strcat(ttp, "\0");

	return 0;
}

//Funcion que elimina los caracteres extra que aparecen al final de la transmicion
int get_data (char* origen, char* destino)
{
	int i;


	for (i = strlen(origen); i > HEADER_SIZE; i--)
	{
		if ( memcmp(origen+i,FIN_TRAMA, sizeof(FIN_TRAMA)) == 0 )
			//Encontre los caracteres de fin de trama
			break;
	}

	memcpy(destino,origen,i);
	strcat(destino, "\0");

	return 0;
}

int open_multicast_client ()
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
	
	//return sd;
}
