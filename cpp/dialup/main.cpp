#include <windows.h>
#include <ras.h>
#include <raserror.h>
#include <iostream.h>
#include <string.h>


#define fname		"list.ip"
#define cmd_exit	"exit"
#define cmd_dial	"dial-up"
#define cmd_hang	"hang-up"
#define cmd_loop	"loop-up"
#define cmd_load	"load"
#define cmd_save	"save"
#define cmd_yes		"y"
#define cmd_no		"n"
#define MAX_LOOP	10

struct INFO
{
	char	phone[16];
	char	name[32];
	char	password[32];
};

struct DATA
{
	char ip[16];
};

struct NODE 
{
	NODE *	next;
	NODE *	prev;
	DATA    data;
};

void	error(const char* error);

bool	DialUp(LPTSTR PhoneNumber,LPTSTR UserName,LPTSTR Password);
bool	IsInternetConnected ();
bool	HangUp();
bool	LoopUp(INFO inf);
void	GetLocalIP(char *result);
void	SetInfo(LPTSTR PhoneNumber,LPTSTR UserName,LPTSTR Password);

bool	Next();
bool	Prev();
void	Begin();
void	End();

void	Load();
void	Save();
void	Clear();
void	AddNewIP(char * new_ip);

INFO	info;
NODE*	lpNode;
NODE*	lpHead;

char 	cmd[256];

int main()
{

	char    get_ip[16];

	cout<<"Welcom To Dial-Up++"<<endl;
	cout<<cmd_dial<<endl;
	cout<<cmd_hang<<endl;

	lpNode=new NODE;
	lpNode->next=NULL;
	lpNode->prev=NULL;
	strcpy(lpNode->data.ip,"127.0.0.1");

	Load();

	do
	{
		cout<<lpNode->data.ip<<endl;
	}
	while(Next());

	while (true)
	{
		cout<<"# ";	cin>> cmd;
		strlwr(cmd);

		if (strcmp(cmd,cmd_exit)==0)	{ Save(); break;}
		if (strcmp(cmd,cmd_hang)==0)	{ HangUp(); continue;}
		if (strcmp(cmd,cmd_save)==0)	{ Save();continue;}
		if (strcmp(cmd,cmd_load)==0)	{ Load(); continue;}

		if (strcmp(cmd,cmd_dial)==0)
		{ 
			if (DialUp(info.phone,info.name,info.password))	
			{
				GetLocalIP((char*)&get_ip);
				cout<<"ip:"<<get_ip<<endl;
				cout<<"connect..."<<endl;
			}
			else 
			{
				cout<<"error:RasDial()"<<endl;
			}
			continue;
		}


		if (strcmp(cmd,cmd_loop)==0)
		{ 
			if(LoopUp(info))
			{
				GetLocalIP((char*)&get_ip);
				cout<<"connect..."<<endl;
			}
			else
			{
				cout<<"error:LoopUP()"<<endl;
			}
			continue;
		}

		cout<<"unknown command"<<endl;
	}


	delete lpNode;
	return 0;
}

void error(const char* error)
{
	char buffer[256];

	strcpy(buffer,"error:");
	strcat(buffer, error);

}


bool DialUp(LPTSTR PhoneNumber,LPTSTR UserName,LPTSTR Password)
{
	RASDIALEXTENSIONS	DialExt;
    RASDIALPARAMS		rdParams;
	HRASCONN			hRasConn = NULL;
	char				szBuf[256];


	DialExt.dwSize=	sizeof(RASDIALEXTENSIONS);
	DialExt.dwfOptions	=RDEOPT_UsePrefixSuffix;
	DialExt.hwndParent	=NULL;
	DialExt.reserved	=NULL;


    rdParams.dwSize = sizeof(RASDIALPARAMS);
	rdParams.szEntryName[0] = '\0';
	lstrcpy( rdParams.szPhoneNumber, PhoneNumber );
	rdParams.szCallbackNumber[0] = '\0';
	lstrcpy( rdParams.szUserName, UserName );
	lstrcpy( rdParams.szPassword, Password );
	rdParams.szDomain[0] = '\0';


	DWORD dwRet = RasDial(NULL, NULL, &rdParams, 0L, NULL, &hRasConn );
	if ( dwRet == 0 )  return true;


	if ( RasGetErrorString( (UINT)dwRet, (LPSTR)szBuf, 256 ) != 0 )	 wsprintf( (LPSTR)szBuf, "Undefined RAS Dial Error (%ld).", dwRet );

	RasHangUp( hRasConn );
	 //MessageBox( (LPSTR)szBuf, "Error", MB_OK | MB_ICONSTOP );
	 return false;
}

bool  HangUp()
{
	 RASCONN ras[20];
	 DWORD  dSize, dNumber;
	 char  szBuf[256];

	 ras[0].dwSize = sizeof( RASCONN );
	 dSize = sizeof( ras );   // Get active RAS - Connection
	 DWORD  dwRet = RasEnumConnections( ras, &dSize, &dNumber );
	 if ( dwRet != 0 )
	 {
	  if ( RasGetErrorString( (UINT)dwRet, (LPSTR)szBuf, 256 ) != 0 )
	   wsprintf( (LPSTR)szBuf, "Undefined RAS Enum Connections error (%ld).", dwRet);
	  //MessageBox( (LPSTR)szBuf, "RasHangUp", MB_OK | MB_ICONSTOP );
	  return false;
	 }
	 bool bOK = true;
	 for( DWORD dCount = 0;  dCount < dNumber;  dCount++ )
	 {    // Hang up that connection
	  HRASCONN hRasConn = ras[dCount].hrasconn;
	  DWORD dwRet = RasHangUp( hRasConn );
	  if ( dwRet != 0 )
	  {
	   char  szBuf[256];
	   if ( RasGetErrorString( (UINT)dwRet, (LPSTR)szBuf, 256 ) != 0 )
		wsprintf( (LPSTR)szBuf, "Undefined RAS HangUp Error (%ld).", dwRet );
	  // MessageBox( (LPSTR)szBuf, "RasHangUp", MB_OK | MB_ICONSTOP );
	   bOK = false;
	  }
	 }
	 return bOK;
}

bool IsInternetConnected ()
{
	DWORD	nRasDevices = 20;
	DWORD	nSize = 20;
	RASCONN rasconnected [10];  // Ras Connected struct

	rasconnected[0].dwSize = sizeof(RASCONN);
	RasEnumConnections( rasconnected, &nSize, &nRasDevices );

    return ( BOOL (nRasDevices) );
}

void GetLocalIP(char *result)
{
     WSADATA WSAData;

     WSAStartup(MAKEWORD(1,1), &WSAData);

	 char dot[6];
	 int iResult;
	 int i = 0;
	 u_long *ppIpNO;
	 u_long *pIpNO;
	 HOSTENT FAR *lphostent;
	 u_long ipHO;
	 unsigned char binIp[4];
	 int iterations = 0;

	 //Get local host name and crudely validate
	 char szHostName[100];
	 *result = 0;

	 iResult = gethostname(szHostName, sizeof(szHostName));
	// printf("%d %s",iResult,szHostName);
	 if ((iResult != 0) || (lstrcmp(szHostName, "")==0))
	  return;

	 //Lok up this host info via supplied name
	 lphostent = gethostbyname(szHostName);
	 if (lphostent == NULL)
	  return;
	 //Retreive first entry (might have multiple connects)
	 do
	 {
	  iterations++;
	  ppIpNO = (u_long *)lphostent->h_addr_list;
	  if (ppIpNO+i == NULL)
	   return;
	  pIpNO = ((u_long *)*(ppIpNO+i));
	  if (pIpNO == NULL)
	   return;

	 //convert back to host order, since SOCKADDR_IN expects that
	  //MessageBox(NULL,"z","x",MB_OK);
	  ipHO = ntohl(*pIpNO);

	  binIp[0] = (BYTE)((ipHO & 0xff000000) >> 24);
	  itoa(binIp[0], dot, 10);
	  strcat(result,dot);
	  binIp[1] = (BYTE)((ipHO & 0x00ff0000) >> 16);
	  itoa(binIp[1], dot, 10);
	  strcat(result, "."); strcat(result, dot);
	  binIp[2] = (BYTE)((ipHO & 0x0000ff00) >> 8);
	  itoa(binIp[2], dot, 10);
	  strcat(result, "."); strcat(result, dot);
	  binIp[3] = (BYTE)(ipHO & 0x000000ff);
	  itoa(binIp[3], dot, 10);
	  strcat(result,"."); strcat(result, dot);
	  //strcat(result,"\r\n");
	  i++;
	 } while ((pIpNO != NULL) && (iterations < 6));
	 WSACleanup();
		  PostQuitMessage(0);
	 return;
}


void Clear()
{

}

void Load()
{
	HANDLE		hFile;
	DWORD		bRead;
	DWORD		fSize;
	DWORD		sum=0;
	DATA		dt;


	hFile=CreateFile((LPCSTR)fname,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		error("CreateFile()");
		return;
	}

	fSize=GetFileSize(hFile,NULL);

	if(fSize!=0)
	{
		for(int i=0;i<fSize/16;i++ )
		{
			ReadFile(hFile,&dt,sizeof(DATA),&bRead,NULL);
			AddNewIP((char*)&dt.ip);
		}
	}

/*
		for(int i=0;i<strlen(buffer);i++)
		{
			if(buffer[i]=='\r' )
			{
				AddNewIP((char*)&old_ip);

				for (j=0;j<16;j++)
				{
					old_ip[j]='\0';
				}
				j=0;
			}
			else
			{
				if(buffer[i]!='\n')
				{
					old_ip[j]=buffer[i];
					j++;
				}
			}
		}
		delete buffer;
		
	}
*/	
	CloseHandle(hFile);
}

void Save()
{
	HANDLE		hFile;
	DWORD		bWrite;
	DATA		dt;
	char *		buffer=NULL;

	hFile=CreateFile((LPCSTR)&fname,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		error("CreateFile()");
		return;
	}


	Begin();

	do
	{
		dt=lpNode->data;
		if(strcmp(dt.ip,"127.0.0.1")!=0)
		{
			WriteFile(hFile,&dt,sizeof(DATA),&bWrite,NULL);
		}
	}
	while (Next());


	CloseHandle(hFile);
}

bool LoopUp(INFO inf)
{
	char	new_ip[16];
	bool	flag=false;
	DWORD	loop=0;

	while(true)
	{
		if(loop<MAX_LOOP)loop++; else return false;

		DialUp(inf.phone,inf.name,inf.password);
		GetLocalIP((char*)&new_ip);
		cout<<"ip:"<<new_ip<<endl;

		Begin();
		flag=true;

		do
		{
			if(strcmp(new_ip,lpNode->data.ip)==0)
			{
				HangUp();
				flag=false;
			}
		}
		while(Next());
		
		if(flag)
		{	
			AddNewIP(new_ip);
			Save();
			break;
		}
	}
	return true;
}

void SetInfo(LPTSTR PhoneNumber,LPTSTR UserName,LPTSTR Password)
{
	strcpy(info.phone,PhoneNumber);
	strcpy(info.name,UserName);
	strcpy(info.password,Password);
}


bool Next()
{
	if(lpNode->next!=NULL)
	{
		lpNode->next->prev=lpNode;
		lpNode=lpNode->next;
		return true;
	}
	return false;
}


bool Prev()
{
	if(lpNode->prev!=NULL)
	{
		lpNode->prev->next=lpNode;
		lpNode=lpNode->prev;
		return true;
	}
	return false;
}

void Begin()
{
	while (lpNode->prev!=NULL)
	{
		Prev();
	}
}

void End()
{
	while (lpNode->next!=NULL)
	{
		Next();
	}
}


void AddNewIP(char * new_ip)
{
	Begin();
	if(lpNode->prev==NULL)
	{
		lpNode->prev=new NODE;
		lpNode->prev->next=lpNode;
		lpNode->prev->prev=NULL;
		lpNode=lpNode->prev;
		ZeroMemory(&lpNode->data,sizeof(DATA));
		strcpy(lpNode->data.ip,new_ip);
	}
}