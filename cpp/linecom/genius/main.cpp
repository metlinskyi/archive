#include <windows.h>
#include <stdio.h>
#include <iostream.h>
#define   def_pass "85=2>285=6A6;646"	


void main()
{
	HANDLE		hFile;
	DWORD		bRead;

	char*		code_tab=NULL;
	char*		real_tab=NULL;

	int		 	real_cx=0;
	int			code_cx=0;
	int			pass_cx=0;

	DWORD		pass_size;
	DWORD		real_size;
	DWORD		code_size;

	char*		password;

	char		exit;
	char		pass_tmp[4];
	char		code_tmp[4];	

	ZeroMemory(pass_tmp,4);
	ZeroMemory(code_tmp,4);

// password

	pass_size=strlen(def_pass);
	password=new char[pass_size];
	strcpy(password,def_pass);

// real

	hFile=CreateFile((LPCSTR)"real.tab",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		return;
	}

	real_size = GetFileSize(hFile,NULL);
	real_tab=new char[real_size];

	ReadFile(hFile,real_tab,real_size,&bRead,NULL);
	CloseHandle(hFile);

// code

	hFile=CreateFile((LPCSTR)"code.tab",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		return;
	}

	code_size = GetFileSize(hFile,NULL);
	code_tab=new char[code_size];

	ReadFile(hFile,code_tab,code_size,&bRead,NULL);
	CloseHandle(hFile);

// encode

	pass_cx=0;

	while(pass_cx<pass_size)
	{
		pass_tmp[0]=password[pass_cx++];
		pass_tmp[1]=password[pass_cx++];

		real_cx=0;
		code_cx=0;
		while(code_cx<code_size)
		{
			code_tmp[0]=code_tab[code_cx++];
			code_tmp[1]=code_tab[code_cx++];

			//cout<<code_tmp<<"-"<<real_tab[real_cx]<<endl;
			
			//if((pass_tmp[0]==code_tmp[0])&&(pass_tmp[1]==code_tmp[1]))
			if(strcmp(code_tmp,pass_tmp)==0)
			{
				cout<<real_tab[real_cx];
			}
			real_cx++;
		}

	}

// exit

	cin>>exit;

	delete[] password; 
	delete[] real_tab;
	delete[] code_tab;
}
