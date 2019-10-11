#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>

//#include "main.h"
//

#define SERVER_MODE 1
#define CLIENT_MODE 2
#define ARG_ERROR 10
#define DEFAULT_PORT 54000
//#define EXAMPLE_IP "127.0.0.1"
#define DEFAULT_IP "239.0.0.1"



void ConfigurarEstructuraServidor (int n_ip, long port, struct sockaddr_in &addr)
{
	// Configuro la IP y el puerto
    //struct sockaddr_in addr;
    int size_addr = sizeof(addr);
    memset(&addr, 0, size_addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = n_ip;			// Cuidado! addr.sin_addr.s_addr = inet_addr(n_ip)
    addr.sin_port = htons(port);

    /*
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
    */
}

void WriteLog(char *msg )
{
	
	char	  szDateTime[40];
	struct tm systime;
	struct timeval tv;

	gettimeofday( &tv, NULL );
	localtime_r( &tv.tv_sec, &systime );

	sprintf( szDateTime, "%02d/%02d/%04d %02d:%02d:%02d",
						systime.tm_mday,
						systime.tm_mon + 1,
						systime.tm_year + 1900,
						systime.tm_hour,
						systime.tm_min,
						systime.tm_sec
						);
	sprintf( &szDateTime[ strlen( szDateTime ) ], ".%03ld\t", tv.tv_usec / 1000 );

	printf( "%s - %s\n", szDateTime, msg);
	
}

int UsageMsg( void )
{
	printf("Usage: udptester --<server or client> <[In sever case: use listen port here] [In client case: use ip and port from server here]> \n");
	printf("For example: udptester --server 54900\n");
	printf("For example: udptester --client 54900\n");
	exit( EXIT_FAILURE );
}

int GetServerOrClient( int argc, char** argv, struct sockaddr_in &params, int *sock )
{
	struct in_addr localInterface;
	int size_addr;

	if(argc > 2)
	{
		//Common parameter for server and client.
		//Here we say that use Internet family adresses.
		params.sin_family = AF_INET;

		
		if( !strcmp(argv[1], "--server" ) )
		{
			*sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (sock < 0) 
			{
				perror("opening datagram socket");
				exit(1);
			}

			// Configuro la IP y el puerto
		    size_addr = sizeof(params);
		    memset(&params, 0, size_addr);
		    params.sin_family = AF_INET;
		    params.sin_addr.s_addr = inet_addr(DEFAULT_IP);
		    params.sin_port = htons(DEFAULT_PORT);

			//groupSock.sin_family = AF_INET;
			//groupSock.sin_addr.s_addr = inet_addr("225.1.1.1");
			//groupSock.sin_port = htons(5555);

			/*
		    * Disable loopback so you do not receive your own datagrams.
			*/
		    char loopch=0;
		    int size_loopch = sizeof(loopch);
		    if (setsockopt(*sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0)
		    {
		      perror("setting IP_MULTICAST_LOOP:");
		      close(*sock);
		      exit(1);
	    	}

			localInterface.s_addr = inet_addr("9.5.1.1");
			if (setsockopt(*sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface,sizeof(localInterface)) < 0)
			{
				perror("setting local interface");
				exit(1);
			}
			return SERVER_MODE;
		}
		
		
		if( !strcmp(argv[1], "--client" ) )
		{
			
			return CLIENT_MODE;
		}
	}

	return ARG_ERROR;
}

void StartEchoServer(struct sockaddr_in &params)
{
	// 1 - Create socket
	int sock;

	if( ( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
	{
		//Error creating socket.
		//TODO: Log some info here.
		return;
	}

	// CODIGO JOSE -------------------------------------------------------------------------------------
	// Configuro la IP y el puerto
    struct sockaddr_in addr;
    int size_addr = sizeof(addr);
    memset(&addr, 0, size_addr);
    params.sin_family = AF_INET;
    params.sin_addr.s_addr = inet_addr(DEFAULT_IP);
    //addr.sin_port = htons(port);

    // Uso setsockopt() para solicitarle al kernel unirse a un multicast EXAMPLE_IP
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(DEFAULT_IP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq) ) < 0 )
    {
        perror("setsockopt");
        //return 1;
    }    

    // --------------------------------------------------------------------------------------------------


	//int val = 1;
	//setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&val , sizeof(int));

	// 2 - Bind the socket to the specified port. This associate the port to the socket.
	int retval = bind( sock, (struct sockaddr *) &params, sizeof ( struct sockaddr_in ));
	if( retval < 0 )
	{
		printf("%s", strerror(errno));
		//TODO: Log some info here.
		return;
	}

	// 3 - Check if really was associated
	unsigned int paramLen = sizeof ( params );
	if( getsockname( sock, (struct sockaddr *) &params, &paramLen ) < 0 )
	{
		//TODO: Log some info here.
		return;
	}

	char line[100];
	//sprintf(line, "Listen Port setted to: %d \t IP: %s", ntohs(params.sin_port), inet_ntop(AF_INET, &(params.sin_addr), str, INET_ADDRSTRLEN));
	sprintf(line, "Listen Port setted to: %d", ntohs(params.sin_port));
	WriteLog(line);
	//printf("Estoy vivo2\n");

	// 4 - Ready to listen messages!!!
	while( 1 )
	{
		struct sockaddr_in client;
		unsigned int client_address_size = sizeof (client);
		char buf[1024];
		
		memset(buf,0,sizeof(buf));

		//printf("Estoy vivo\n");

		//Warning: recvfrom is in blocking mode by default, waiting for a message
		if( recvfrom( sock, buf, sizeof (buf), 0, (struct sockaddr *)&client,
					  &client_address_size ) < 0 )
		{
			//TODO: Log some info here.
			printf("Cliente conectado");
			return;
		}


		if( sendto( sock, (const char *)buf, strlen(buf), 0, (struct sockaddr *) &client,
                client_address_size) < 0 )
		{
			//TODO: Log some info here.
			return;
		}
	}
}

void StartEchoClient(struct sockaddr_in &params)
{
	// 1 - Create socket a
	int sock;	

	if( ( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
	{
		//Error creating socket.
		//TODO: Log some info here.
		return;
	}

	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&params, sizeof(params)) < 0) 
	{
	    perror("setting local interface");
	    exit(1);
  	}

	int i = 0;

	// 4 - Ready
	while( 1 )
	{
		char buf[1024];

		sprintf(buf,"%s - [%d]","Echo message nro", i);

		WriteLog( buf );

		if( sendto( sock, (const char *)buf, strlen( buf ),	0, (struct sockaddr *) &params,	sizeof ( struct sockaddr_in )) < 0 )
		{
			//TODO: Log some info here.
			return;
		}

		//Warning: recvfrom is in blocking mode by default, waiting for a message
		struct sockaddr_in serverResponseAddress;
		unsigned int server_address_size = sizeof (serverResponseAddress);
		if( recvfrom( sock, buf, sizeof (buf ), 0, (struct sockaddr *)&serverResponseAddress, &server_address_size ) < 0 )
		{
			//TODO: Log some info here.
			return;
		}

		WriteLog( (char *) "Echo received" );

		i++;

		sleep(5);
	}
}

int main( int argc, char** argv )
{
	struct sockaddr_in addres_info;
	int sock;

	int mode = GetServerOrClient(argc, argv, addres_info, &sock);

	if( mode < 0 )
	{
		UsageMsg();
	}
	else if( mode == SERVER_MODE )
	{
		StartEchoServer(addres_info);
	}
	else
	{
		StartEchoClient(addres_info);
	}

	exit( EXIT_SUCCESS );
}
