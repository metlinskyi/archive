#include "header.h"
#include <winsock2.h>

int Operator(char * lp)
{
	//const char name[]="Jeans (+380 50)";
	//const char name[]="UMC (+380 50 or 044)";
	//const char name[]="Golden Telecom (+380 39)";
	const char name[]="WellCOM Telecom (+380 68)";
	//const char name[]="Kyivstar GSM (+380 67)";


	strcpy(lp,name);

	return 1004;
}

bool Send(char* phone,char* msg)
{
	WSADATA			wsaData;
	SOCKET			sock;
	SOCKADDR_IN		addr;
	hostent*		hp; 

//REQUEST:

	char request[512];
	char content_length[32];
	char command[512];


	wsprintf(command,"cmdSend=sent&Message=%s&network1=WC&Phone1=%s&=\r\n",msg,phone);
	wsprintf(content_length,"Content-Length: %d\r\n",strlen(command));



	strcpy(request, "POST /ukr/sms.php? HTTP/1.0\r\n");
	strcat(request,	"Host: www.jeans.com.ua\r\n");
	strcat(request,	"User-Agent: Mozilla/3.0 (Compatible);metlinskiy@ua.fm\r\n");
	strcat(request,	"Accept: Text/*, */*\r\n");
	strcat(request,	"Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(request,	"Referer:\r\n");
	strcat(request,	"Connection: keep-alive\r\n");
	strcat(request,	 content_length);
	strcat(request,	"\r\n");
	strcat(request,	 command);


//SEND:

	WSAStartup(MAKEWORD(2,2), &wsaData);

	sock = socket (AF_INET ,SOCK_STREAM,0);
	if(sock==INVALID_SOCKET) return false;

	hp = gethostbyname("212.113.36.204");
	if (hp==NULL)  return false;

	memcpy(&(addr.sin_addr),hp->h_addr,hp->h_length);
	addr.sin_family=hp->h_addrtype;
	addr.sin_port= htons(80);

	if (connect(sock,(struct sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR) return false;
	send(sock,request,strlen(request),0);
	closesocket(sock);


	return true;
}


