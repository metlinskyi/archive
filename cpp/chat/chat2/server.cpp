#include "server.h"

cserver::cserver()
{
	debug=false;
	init();
}

cserver::cserver(bool status)
{
	debug=status;
	init();
}

cserver::~cserver()
{
	stop();
	
	if(debug) observer("server:destroy");
	WSACleanup(); 
}

bool cserver::init()
{
	if (WSAStartup(MAKEWORD( 2,2 ),&wsaData)==SOCKET_ERROR)
	{
		observer("error:startup 2.2");

		if (WSAStartup(MAKEWORD( 1,1 ),&wsaData)==SOCKET_ERROR)
		{
			observer("error:startup 1.1");
			return false;
		}
		else
		{
			if(debug) observer("init:startup 1.1");
		}
	}
	else
	{
		if(debug) observer("init:startup 2.2");
	}

	return true;
}

bool cserver::create(unsigned short port,int count)
{

	char buffer[256];


	if(count<1)
	{
		observer("error:max user(0)");
		return false;
	}
	max_user = count;

	if(debug)
	{
		wsprintf(buffer,"create:max user %d",max_user);
		observer(buffer);
	}



	sock = socket (AF_INET ,SOCK_STREAM,0);
	if(sock==INVALID_SOCKET)
	{
		observer("error:invalid socket");
		return  false;
	}

	if(debug) observer("create:socket");


	addr.sin_family			=	AF_INET;
	addr.sin_addr.s_addr	=	htonl(INADDR_ANY);
	addr.sin_port			=	htons(port);	


	
	if(bind(sock,(LPSOCKADDR)&addr, sizeof(struct sockaddr)))
	{
		observer("error:bind");
		return  false;
	}

	if(debug) observer("create:bind");




	if(listen(sock, max_user)) 
	{
		observer("error:listen");		
		return  false;
	}

	if(debug) observer("create:listen");


	return true;
}

void cserver::stop()
{
	closesocket(sock);
}


void cserver::action()
{
	char	type[2]; 
	char	buffer[128];
	//SOCKET	client_sock; 

	while(true)
	{
		SOCKET	client_sock=accept(sock,0,0);

		if(debug) observer("action:user accept");


		while(true)
		{
			int i= recv(client_sock, buffer, 128, 0);

			if ((i== SOCKET_ERROR)||(i == 0)) break;

			observer(buffer);


			strcpy(buffer,"Welcom!");
			send(client_sock, buffer, 128, 0);
		}
		shutdown(client_sock,1);
		closesocket(client_sock);
		observer("user disconnect");
	}
}

