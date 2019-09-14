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
#define EXAMPLE_PORT 54000
//#define EXAMPLE_IP "127.0.0.1"
#define EXAMPLE_IP "239.0.0.1"



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
	printf("For example: udptester --client 192.168.0.100 54900\n");
	exit( EXIT_FAILURE );
}

int GetServerOrClient( int argc, char** argv, struct sockaddr_in &params )
{
	if(argc > 2)
	{
		//Common parameter for server and client.
		//Here we say that use Internet family adresses.
		params.sin_family = AF_INET;

		
		if( !strcmp(argv[1], "--server" ) )
		{
			params.sin_addr.s_addr = htonl(INADDR_ANY); //Here we say that accept any address.
			params.sin_port = htons( (unsigned short int) strtol(argv[2], NULL, 10) ); //Here we set the port that we use to listen.
			return SERVER_MODE;
		}
		
		/*
		if( !strcmp(argv[1], "--server" ) )
		{
			//params.sin_addr.s_addr = htonl(INADDR_ANY); //Here we say that accept any address.
			//params.sin_port = htons( (unsigned short int) strtol(argv[2], NULL, 10) ); //Here we set the port that we use to listen.
			
			char n_ip;
			long n_port;

			if( !strcmp(argv[2], "" ) )
			{
				n_ip = inet_addr(argv[2]);
				n_port = (unsigned short int) strtol(argv[3], NULL, 10);
			}
			else
			{
				n_ip = inet_addr(EXAMPLE_IP);
				n_port = EXAMPLE_PORT;
			}

			int size_addr = sizeof(addr);
			memset(&addr, 0, size_addr);
			params.sin_family = AF_INET;
			params.sin_addr.s_addr = n_ip;			// Cuidado! addr.sin_addr.s_addr = inet_addr(n_ip)
			params.sin_port = htons(n_port);

			return SERVER_MODE;
		}
		*/

		if( !strcmp(argv[1], "--client" ) )
		{
			/*
			params.sin_addr.s_addr = inet_addr(argv[2]); //Here we say that accept our client is going to connect to this address.
			params.sin_port = htons( (unsigned short int) strtol(argv[3], NULL, 10) ); //Here we set the port of the server that we are going to connect.
			return CLIENT_MODE;
			*/


			// CODIGO JOSE ------------------------------------------------------------------------------------------------
			params.sin_addr.s_addr = inet_addr(EXAMPLE_IP); //Here we say that accept our client is going to connect to this address.
			params.sin_port = htons( (unsigned short int) strtol(argv[2], NULL, 10) ); //Here we set the port of the server that we are going to connect.
			return CLIENT_MODE;
			// ------------------------------------------------------------------------------------------------------------
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
    params.sin_addr.s_addr = inet_addr(EXAMPLE_IP);
    //addr.sin_port = htons(port);

    // Uso setsockopt() para solicitarle al kernel unirse a un multicast EXAMPLE_IP
    //
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(EXAMPLE_IP);
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
	sprintf(line, "Listen Port setted to: %d", ntohs(params.sin_port));
	WriteLog(line);

	// 4 - Ready to listen messages!!!
	while( 1 )
	{
		struct sockaddr_in client;
		unsigned int client_address_size = sizeof (client);
		char buf[1024];
		
		memset(buf,0,sizeof(buf));

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
	int mode = GetServerOrClient(argc, argv, addres_info);

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
