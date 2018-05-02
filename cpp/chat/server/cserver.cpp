
#include "cserver.h"
#include <winbase.h.>

CSERVER::CSERVER()
{

}

CSERVER::~CSERVER()
{
	WSACleanup(); 
}

bool CSERVER::Start(char* host, unsigned short port)
{

	if (WSAStartup(MAKEWORD( 2,2 ),&wsa)==SOCKET_ERROR)
	{
		if (WSAStartup(MAKEWORD( 1,1 ),&wsa)==SOCKET_ERROR)
		{
			return false;
		}
	}


	if(host==NULL )	{return false; }

	sock = socket (AF_INET ,SOCK_STREAM,0); // TCP
	if(sock==INVALID_SOCKET)
	{
		return  false;
	}

	addr.sin_family			=	AF_INET;
	addr.sin_addr.s_addr	=	inet_addr(host);
	addr.sin_port			=	htons(port);	

	
	if(bind(sock,(LPSOCKADDR)&addr, sizeof(struct sockaddr)))
	{
		return  false;
	}

	if(listen(sock, 1)) 
	{
		return  false;
	}

	return true;
}


bool CSERVER::Chat()
{
	int		ret;
	char	type[2]; 
	char	Msg[256];
	char	String[280];

	char	Greeting[] = "Welcom to Console Chat\r\n"; 
	char*	pChat=new char[strlen(Greeting)+1];
	char*	pBuffer;
	char*	pMsg;

	int		Size=0;

	SYSTEMTIME	Time;

	wsprintf(pChat,"%s",Greeting);

	while(true)
	{
		SOCKET client=accept(sock,0,0);

		while(true)
		{
			ret=recv(client,Msg,256, 0);
			if ((ret== SOCKET_ERROR)||(ret == 0)) break;

			
			GetSystemTime(&Time);

			wsprintf(String,"%d:%d:%d  %s",Time.wHour,Time.wMinute,Time.wSecond,Msg); 
			observer(String);


            pBuffer= NULL; 	
			pBuffer= new char[Size+284];
			strcpy(pBuffer,String);
			strcat(pBuffer,"\r\n");
			strcat(pBuffer,pChat);
			strcat(pBuffer,"\r\n");

			pChat=pBuffer;


            Size=strlen(pChat);
			send(client,(const char*)&Size,sizeof(int),0);// send size message

            send(client,pChat,Size,0); // send message


			closesocket(client);

		}

		shutdown(client,1);
		closesocket(client);
	}
}

int CSERVER::SendMsg(SOCKET socket,const char* msg )
{
	return send(socket,msg,strlen(msg),0);
}


