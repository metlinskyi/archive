#include <iostream.h>
#include <stdio.h>
#include "client.h"
#include <ras.h>
#include <raserror.h>

#define MSG_SIZE 950

cclient client(true);

char request[2048];
char content_length[32];
char command[1024];
char msg[MSG_SIZE];
char phone[]="2904509";

void observer(char * msg)
{
	char* buffer;
	long  size=strlen(msg);	

	buffer=new char[size+1];
	strcpy(buffer,msg);
	cout <<buffer<<endl;

	delete[] buffer;
}
 
BYTE rnd8( BYTE low, BYTE high )
{
    BYTE range = high - low;
    BYTE num = rand() % range;
    return( num + low );
}

bool InternetConnected ()
{
	DWORD	nRasDevices = 20;
	DWORD	nSize = 20;
	RASCONN rasconnected [10];  

	rasconnected[0].dwSize = sizeof(RASCONN);
	RasEnumConnections( rasconnected, &nSize, &nRasDevices );

    return ( BOOL (nRasDevices) );
}

void main()
{
	while(true)
	{
		if(!InternetConnected()) 
		{
			observer("Not find internet connected...");
			break;
		}
			 
		ZeroMemory(&request,2048);
		ZeroMemory(&command,1024);
		ZeroMemory(&content_length,32);
		ZeroMemory(&msg,MSG_SIZE);

		for(int i=0;i<MSG_SIZE;i++) msg[i]=(char)rnd8(65,90);

		wsprintf(command,"cmdSend=sent&Phone1=%s&network1=JEANS&Message=%s&=\r\n",phone,msg);
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

		observer(request);
		observer("press any key...");
		getchar();

		client._connect("212.113.36.204",80);
		client._sendex(request,strlen(request));
		client.stop();

		Sleep(0);

		break;
	}
	getchar();
}