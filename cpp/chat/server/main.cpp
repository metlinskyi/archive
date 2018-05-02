#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include "cserver.h"


CSERVER Server;

void observer(char* msg)
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
    if(!Server.Start("127.0.0.1",7777))
	{
		observer("Error: start server");
		return;
	}	

	if(!Server.Chat())
	{
		observer("Error: chat action");
		return;
	}
	
}