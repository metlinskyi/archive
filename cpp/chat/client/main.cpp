#include <windows.h>
#include <iostream.h>
#include <string.h>
#include "cclient.h"

#define cmd_exit	"exit"

CCLIENT		Client;

CONSOLE_SCREEN_BUFFER_INFO cinfo; 
COORD c;
DWORD written;
HANDLE hConsole;

void observer(const char* msg) // debuger messadge
{
	char* buffer;
	long  size=strlen(msg);	

	buffer=new char[size+1];
	strcpy(buffer,msg);
	cout <<buffer<<endl;

	delete[] buffer;
}

void ClearScreen()
{
	c.X = 0;
	c.Y = 0;
	GetConsoleScreenBufferInfo(hConsole, &cinfo);
	FillConsoleOutputCharacter(hConsole, ' ',cinfo.dwSize.X*cinfo.dwSize.Y, c, &written);
	FillConsoleOutputAttribute(hConsole, FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE, cinfo.dwSize.X*cinfo.dwSize.Y, c,&written);
}

void main(int argc, char* argv[])
{
	if (argv[1]==NULL || argv[2]==NULL) // check command line
	{
		observer("Sample: client 127.0.0.1 UserName");
		return;
	}

	char	cmd[128];
	char	pMsg[256];

	char*	pChat;
	char*	pBuffer;

	int		Size; 

	
	ZeroMemory(&cmd,128);

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	wsprintf(pMsg,"%s: connect",argv[2]);

	do
	{
		ClearScreen();

		if (!Client.Connect(argv[1],7777)) // connect to server
		{
			observer("Error: not connect");
			return;
		}

		Client.SendMsg(pMsg);		
		Client.RecvMsg((char*)&Size,sizeof(int));

		pChat= NULL;
		pChat= new char[Size];
		Client.RecvMsg(pChat,Size);

        //put to screen
		cinfo.dwCursorPosition.X=0;
		cinfo.dwCursorPosition.Y=1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cinfo.dwCursorPosition);	

		observer(pChat);
		delete[] pChat;

		cinfo.dwCursorPosition.X=0;
		cinfo.dwCursorPosition.Y=0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cinfo.dwCursorPosition);

		//command string
		cout<<"# ";	cin>> cmd;	
		wsprintf(pMsg,"%s:%s",argv[2],cmd);

	}while(strcmp(cmd,cmd_exit)!=0);//exit

	wsprintf(pMsg,"%s: disconnect",argv[2]);
    if (Client.Connect(argv[1],7777)) // connect to server
	{
		Client.SendMsg(pMsg);	
	}
	
	Client.Disconnect();
}
