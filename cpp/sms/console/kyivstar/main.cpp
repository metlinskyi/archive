#include <iostream.h>
#include <stdio.h>
#include <winsock2.h>


bool Send(char*,char*);

void observer(char * msg)
{
	char* buffer;
	long  size=strlen(msg);	

	buffer=new char[size+1];
	strcpy(buffer,msg);
	cout <<buffer<<endl;

	delete[] buffer;
}

void main()
{
	observer("press any key...");
	getchar();

	if (!Send("6829158","TEST"))
	{
		observer("send error...");
	}

	getchar();
}

bool Send(char* phone,char* msg)
{
	WSADATA			wsaData;
	SOCKET			sock;
	SOCKADDR_IN		addr;
	hostent*		hp; 



	char buffer[1024];
	char request[512];
	char content_length[32];
	char command[256];
	int code=2265;
	int len=156;

	WSAStartup(MAKEWORD(2,2), &wsaData);

	sock = socket (AF_INET ,SOCK_STREAM,0);
	if(sock==INVALID_SOCKET) return false;

	hp = gethostbyname("193.41.60.108");
	if (hp==NULL)  return false;

	memcpy(&(addr.sin_addr),hp->h_addr,hp->h_length);
	addr.sin_family=hp->h_addrtype;
	addr.sin_port= htons(80);

	if (connect(sock,(struct sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR) return false;

//REQUEST (LOAD):
	observer("---LOAD---\r\n"); 

	strcpy(request, "GET /ru/sms/ HTTP/1.0\r\n");
	strcat(request,	"Accept-Language: uk\r\n");
	strcat(request,	"Accept: */*\r\n");
	strcat(request,	"Accept-Encoding: gzip, deflate\r\n");   
	strcat(request,	"User-Agent: Mozilla/3.0 (Compatible);metlinskiy@ua.fm\r\n");
	strcat(request,	"Host: www.kyivstar.net\r\n");
	strcat(request,	"Connection: keep-alive\r\n");
	strcat(request,	"Cookie: 429496729=a8fd42b0d841431b68a6c1885181b749\r\n");
	strcat(request,	"\r\n");

	observer(request);

//->SEND:

	send(sock,request,strlen(request),0);
	recv(sock, buffer, 1024, 0);
	observer(buffer);

//END


//REQUEST (IMAGE):

	observer("---IMAGE---\r\n"); 

	strcpy(request, "GET /ru/sms/image.php HTTP/1.0\r\n");
	strcat(request,	"Accept: */*\r\n");
	strcat(request,	"Referer: http://www.kyivstar.net/ru/sms/\r\n");
	strcat(request,	"Accept-Language: uk\r\n");
	strcat(request,	"Accept-Encoding: gzip, deflate\r\n");   
	strcat(request,	"User-Agent: Mozilla/3.0 (Compatible);metlinskiy@ua.fm\r\n");
	strcat(request,	"Host: www.kyivstar.net\r\n");
	strcat(request,	"Connection: keep-alive\r\n");
	strcat(request,	"Cookie: 429496729=a8fd42b0d841431b68a6c1885181b749\r\n");
	strcat(request,	"\r\n");

	observer(request);

//->SEND:

	send(sock,request,strlen(request),0);
	recv(sock, buffer, 1024, 0);
	observer(buffer);



//END


//REQUEST (SEND):
	observer("---SEND---\r\n"); 

	wsprintf(command,"to_addr=%s&dcs=0&new_body=%s&len=%d&code_visual=%d&=\r\n",phone,msg,len,code);
	wsprintf(content_length,"Content-Length: %d\r\n",strlen(command));

	strcpy(request, "POST /ru/sms/send.php HTTP/1.0\r\n");
	strcat(request,	"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, */*\r\n");
	strcat(request,	"Referer: http://www.kyivstar.net/ru/sms/\r\n");
	strcat(request,	"Accept-Language: uk\r\n");
	strcat(request,	"Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(request,	"Accept-Encoding: gzip, deflate\r\n");
	strcat(request,	"User-Agent: Mozilla/3.0 (Compatible);metlinskiy@ua.fm\r\n");
	strcat(request,	"Host: www.kyivstar.net\r\n");
	strcat(request,	 content_length);
	strcat(request,	"Connection: keep-alive\r\n");
	strcat(request,	"Cache-Control: no-cache\r\n");
	strcat(request,	"Cookie: 429496729=a8fd42b0d841431b68a6c1885181b749\r\n");
	strcat(request,	"\r\n");
	strcat(request,	 command);

	observer(request);

//->SEND:

	send(sock,request,strlen(request),0);
	recv(sock, buffer, 512, 0);
	observer(buffer);

//END

//EXIT:

	closesocket(sock);

	return true;
}