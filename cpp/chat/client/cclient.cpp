#pragma comment ( lib, "wsock32.lib" )

#include <winsock2.h>
#include <string.h>
#include "cclient.h"

CCLIENT::CCLIENT()
{

}

CCLIENT::~CCLIENT()
{
	WSACleanup(); 
}

bool CCLIENT::Connect(const char* host,unsigned long port )
{
	WSADATA			wsa; 
	SOCKADDR_IN		addr;
	hostent*		hp; 


	if (WSAStartup(MAKEWORD( 2,2 ),&wsa)==SOCKET_ERROR)
	{
		if (WSAStartup(MAKEWORD( 1,1 ),&wsa)==SOCKET_ERROR)
		{
			return false;
		}
	}

	char	buffer[256];


	if ((host==NULL)||(port<0))
	{
		return false;
	}

	sock = socket (AF_INET ,SOCK_STREAM,0);
	if(sock==INVALID_SOCKET)
	{
		return  false;
	}



	hp = gethostbyname(host);
	if (hp==NULL)
	{
		return false;
	}

	memcpy(&(addr.sin_addr),hp->h_addr,hp->h_length);
	addr.sin_family=hp->h_addrtype;
	addr.sin_port= htons(port);

	if (connect(sock,(struct sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR)
	{
		return false;
	}


	return true;
}

void CCLIENT::Disconnect()
{	
	if(sock!=INVALID_SOCKET )
	{
		closesocket(sock);
	}
}

void  CCLIENT::SendMsg(const char* msg)
{
	send(sock,msg,strlen(msg)+1,0);
}

void  CCLIENT::RecvMsg(char* msg,int size)
{
	recv(sock,msg,size,0);
}

