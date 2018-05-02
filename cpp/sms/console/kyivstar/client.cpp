#include "client.h"

cclient::cclient()
{
	debug=false;
	init();
}

cclient::cclient(bool status)
{
	debug=status;
	init();
}

cclient::~cclient()
{
	stop();

	if(debug) observer("client:destroy");

	WSACleanup(); 
}

bool cclient::init()
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

	hp=NULL;

	return true;
}


bool cclient::_connect(const char* host,unsigned short port)
{

	char			buffer[256];


	if ((host==NULL)||(port<0))
	{
		observer("error:invalid host name ");
		return false;
	}

	if(debug)
	{
		wsprintf(buffer,"connect:%s(%d)",host,port);
		observer(buffer);
	}


	
	sock = socket (AF_INET ,SOCK_STREAM,0);
	if(sock==INVALID_SOCKET)
	{
		observer("error:invalid socket");
		return  false;
	}

	if(debug) observer("create:socket");



	hp = gethostbyname(host);
	if (hp==NULL)
	{
		observer("error:get host name");
		return false;
	}

	if(debug) observer("connect:get host name");


	memcpy(&(addr.sin_addr),hp->h_addr,hp->h_length);
	addr.sin_family=hp->h_addrtype;
	addr.sin_port= htons(port);

	if (connect(sock,(struct sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR)
	{
		observer("error:connect");
		return false;
	}

	if(debug) observer("connect:get host name");

	return true;
}

void cclient::stop()
{
	closesocket(sock);
}


void cclient::_send(char data)
{
	send(sock,&data,sizeof(BYTE),0);
}

char cclient::_recv()
{
	char buffer;
	recv(sock, &buffer, sizeof(BYTE), 0);
	return buffer;
}


void cclient::_sendex(char* data, int size)
{
	/*
	send(sock,&bstart,sizeof(char),0);
	send(sock,(const char*)&size,sizeof(DWORD),0);
	*/
	send(sock,data,size,0);
}

void cclient::_recvex(char* data, int size)
{
	char* buffer=new char[size];
	recv(sock, buffer, size, 0);
	memcpy(data,buffer,size);
	delete[] buffer;
}