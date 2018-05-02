#include "main.h"
#include "resource.h"

CMAIN		main(true);
HWND		hwnd_error;
BYTE*		f;
char		fname[MAX_PATH];
DWORD		sz;
bool		rule;
char		mode[32];

void Error(LPCTSTR Error)
{
	char buffer[255];
	wsprintf(buffer,"Ошибка: %s",Error); 
	MessageBox(hwnd_error, buffer,"class CNODE",MB_OK | MB_ICONSTOP);
}
//-------------------------------------------------------------------------------	
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    return main.WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
//-------------------------------------------------------------------------------
int CMAIN::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	MSG						msg;
/*
	INITCOMMONCONTROLSEX	iccex;

	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_BAR_CLASSES ;
	InitCommonControlsEx(&iccex);
*/

	current_dir=new char[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,current_dir);

	if (!dlg)
	{
		if (!InitApplication(hInstance))
		{
			OutputDebugString( "Could not initialize application\n" );
			return FALSE;
		}

		if (!InitInstance(hInstance))
		{
			OutputDebugString( "Could not initialize instance\n" );
			return FALSE;
		}

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (msg.wParam);
	}
	else
	{
		BOOL	appDone=FALSE;

		m_hIcon=	LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
		m_hDlg=		CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);

		while(!appDone)
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(!IsDialogMessage(GetWnd(), &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if(msg.message==WM_QUIT)
				{
					appDone=TRUE;
					break;
				}
			}
		}
		DestroyIcon(m_hIcon);
		return (msg.wParam);
	}
}
//-------------------------------------------------------------------------------
BOOL CMAIN::InitApplication(HINSTANCE hInstance)
{
    HWND hWnd = FindWindow (CLASSNAME, WINDOWNAME);
    if (hWnd)
	{
        return FALSE;
	}

    WNDCLASSEX  wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = ::WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASSNAME;


    return RegisterClassEx(&wc);
}
//-------------------------------------------------------------------------------
BOOL CMAIN::InitInstance(HINSTANCE hInstance)
{
    m_hInst = hInstance;
    m_hWnd = CreateWindow(CLASSNAME,WINDOWNAME,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,CW_USEDEFAULT,0,NULL,NULL,hInstance, NULL);
    if (!m_hWnd)
    {
        OutputDebugString( "Could not create window\n" );
        return FALSE;
    }

	return TRUE;
}
//-------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return main.WndProc(hWnd, message, wParam, lParam);
}
//-------------------------------------------------------------------------------
LRESULT CMAIN::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_CREATE:
				
			break;
		case WM_TIMER:
			break;

        case WM_DESTROY:
            ExitApplication();
            break;

        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return TRUE;
}
//-------------------------------------------------------------------------------
void CMAIN::WideToByte(PWSTR pwzInput, PSTR pszOutput, WORD wStrLength)
{
    WideCharToMultiByte( CP_ACP, 0, pwzInput, wStrLength, pszOutput, wStrLength, NULL, NULL);
}
//-------------------------------------------------------------------------------
void CMAIN::ExitApplication()		
{
	PostQuitMessage(0);
}
//------------------------ GET CMAIN ---------------------------------------

HWND		CMAIN::GetWnd()				{	return m_hWnd; }
HWND		CMAIN::GetDlg()				{	return m_hDlg; }
HICON		CMAIN::GetIcon()			{   return m_hIcon;}
HINSTANCE	CMAIN::GetInst()			{	return m_hInst;}

char* CMAIN::GetStr(HWND hwnd,unsigned long index)
{
	char* buffer;

	buffer=new char[GetWindowTextLength(hwnd)];
	GetWindowText(GetDlgItem(hwnd,index),buffer,GetWindowTextLength(hwnd));
	return buffer;
}

unsigned int CMAIN::GetInt(HWND hwnd,unsigned long index)
{
	return GetDlgItemInt(hwnd,index,NULL,TRUE);
}

float CMAIN::GetFlt(HWND hwnd,unsigned long index)
{
	return NULL;
}

//------------------------ SET CMAIN ---------------------------------------

void CMAIN::SetStr(HWND hwnd,unsigned long index, char* string)
{
	SetWindowText(GetDlgItem(hwnd,index),string);
}

void CMAIN::SetInt(HWND hwnd,unsigned long index, unsigned int digit)
{
	SetDlgItemInt(hwnd,index,digit,TRUE);	
}

void CMAIN::SetFlt(HWND hwnd,unsigned long index, float digit)
{
		
}
//------------------------------------------------------------------------
//							List Box
//
bool CMAIN::ListBoxVoid(HWND hwnd, int index)
{
	if (ListBox_GetCount(GetDlgItem(hwnd,index))>0)
	{
		EnableWindow(GetDlgItem(hwnd, index), true);
		return false;
	}
	else
	{
		EnableWindow(GetDlgItem(hwnd, index), false);
		return true;
	}
}
//------------------------------------------------------------------------------
void CMAIN::ListBoxDir(HWND hwnd,int index,char* dir, char* ext)
{
	char*	buffer;
	buffer=new char[(strlen(dir)+strlen(ext)+2)]; 

	strcpy(buffer,dir);
	strcat(buffer,"\\");
	strcat(buffer,ext);

	ListBox_ResetContent(GetDlgItem(hwnd,index));
	ListBox_Dir(GetDlgItem(hwnd,index),NULL,buffer);
	ListBox_SetCurSel(GetDlgItem(hwnd,index),0);

	delete[] buffer;
}
//------------------------------------------------------------------------------
void CMAIN::ListBoxDelete(HWND hwnd,int index, char* dir)
{
	char*	buffer;
	DWORD	size=ListBox_GetTextLen( GetDlgItem(hwnd,index) ,ListBox_GetCurSel( GetDlgItem(hwnd,index) ));

	buffer	= new char[size+1];
	ListBox_GetText(GetDlgItem( hwnd,index),ListBox_GetCurSel(GetDlgItem( hwnd,index)), buffer);

	delete[] buffer;
}
//------------------------------------------------------------------------------
void MultiLine(HWND hwnd,char* msg)
{
	char*	buffer;
	char*	tmpBuffer;
	int		OldLength;
	int		NewLength;

	OldLength	= GetWindowTextLength(hwnd);
	NewLength	= OldLength+strlen(msg)+5;
	
	buffer	= new char[NewLength];
	tmpBuffer= new char[OldLength+1];

	GetWindowText(hwnd,tmpBuffer,OldLength+1);
		
	strcpy(buffer,tmpBuffer); 
	strcat(buffer,"\r\n");
	strcat(buffer,msg);

	SetWindowText(hwnd,buffer);


	delete[] buffer;
	delete[] tmpBuffer;

	int k=Edit_GetLineCount(hwnd);
	for (int i=0;i<k;i++) SendMessage(hwnd,EM_LINESCROLL ,0,k);
}

void MultiLineAdd(HWND hwnd,char* msg)
{
	char*	buffer;
	char*	tmpBuffer;
	int		OldLength;
	int		NewLength;

	OldLength	= GetWindowTextLength(hwnd);
	NewLength	= OldLength+strlen(msg)+5;
	
	buffer	= new char[NewLength];
	tmpBuffer= new char[OldLength+1];

	GetWindowText(hwnd,tmpBuffer,OldLength+1);
		
	strcpy(buffer,tmpBuffer); 
	strcat(buffer,msg);

	SetWindowText(hwnd,buffer);


	delete[] buffer;
	delete[] tmpBuffer;

	int k=Edit_GetLineCount(hwnd);
	for (int i=0;i<k;i++) SendMessage(hwnd,EM_LINESCROLL ,0,k);
}



long CMAIN::ListCmBoxDir(HWND hwnd,char* dir, char * ext)
{
	WIN32_FIND_DATA		ffd;
	HANDLE				hFile;
	bool				fFile=true;
	long				count=0;
	char				mask[5];

	strcpy(mask,"*");
	strcat(mask,ext);
	
	SetCurrentDirectory(dir);



	hFile=FindFirstFile(mask,&ffd);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		while(fFile)
		{
			ComboBox_AddString(hwnd,ffd.cFileName);
			count++;
			//fFile=FindNextFile(hFile,&ffd);
		}
		FindClose(hFile);
	}

	//SetCurrentDirectory(engine.GetDirHome());
	return  count;
}


void CMAIN::LoadFile(char * path,void * data)
{
	HANDLE		hFile;
	DWORD		bRead;

	hFile=CreateFile((LPCSTR)path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		Error("в функций CreateFile() невозможно создать дескриптор файл!");
	}
	ReadFile(hFile,data,GetFileSize(hFile,NULL),&bRead,NULL);
	
	CloseHandle(hFile);
}

void CMAIN::SaveFile(char * path, void * data,DWORD size)
{
	HANDLE		hFile;
	DWORD		bWrite;

	hFile=CreateFile((LPCSTR)path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		Error("в функций CreateFile() невозможно записать временный файл!");
	}

	WriteFile(hFile,data,size,&bWrite,NULL);

	CloseHandle(hFile);
}

DWORD CMAIN::GetFSize(char* path)
{
	HANDLE		hFile;
	DWORD		size;

	hFile=CreateFile((LPCSTR)path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		Error("в функций CreateFile() невозможно создать дескриптор файл!");
	}
	size= GetFileSize(hFile,NULL);
	CloseHandle(hFile);
	return size;
}

char* CMAIN::GetCurDir()
{
	return current_dir;
}
//****************************** ИНТЕРФЕЙС *************************************
//------------------------------------------------------------------------------
//							Обработка сообщений окна IDD_MAIN
//------------------------------------------------------------------------------


BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_DESTROY:
			delete[] f;

			break;
		case WM_CLOSE:
			main.ExitApplication();
			break;
        case WM_INITDIALOG:

			hwnd_error = hWnd;

			SendMessage(hWnd,WM_SETICON, ICON_BIG,(LPARAM) main.GetIcon());
			SendMessage(hWnd,WM_SETICON, ICON_SMALL,(LPARAM) main.GetIcon());
			SetWindowText(hWnd,STR_WND);


			ComboBox_AddString(GetDlgItem(hWnd,IDC_MODE),"побитовое сложение xor");

			ComboBox_AddString(GetDlgItem(hWnd,IDC_MODE),"перестановка");

			ComboBox_AddString(GetDlgItem(hWnd,IDC_MODE),"метод Вижинера");

			ComboBox_SetCurSel(GetDlgItem(hWnd,IDC_MODE), 0);


            return TRUE;

		case WM_ACTIVATE:
			if (wParam=WA_ACTIVE)
			{
				main.ListBoxDir(hWnd,IDC_DIR,main.GetCurDir(),"*.*");
				main.ListBoxVoid(hWnd,IDC_DIR);
				SetFocus(GetDlgItem(hWnd,IDC_CODE));
				return (TRUE);
			}
			break;



        case WM_COMMAND:
			switch (LOWORD(wParam))
			{

				case IDC_CODE:

					switch(ComboBox_GetCurSel(GetDlgItem(hWnd,IDC_MODE)))
					{

						case 0://xor

							if(rule)
							{
								//Encryption(f,s, main.GetInt(hWnd,IDC_PASSWORD));
								EncryptionXOR(f,sz, main.GetStr(hWnd,IDC_PASSWORD));
								main.SetStr(hWnd,IDC_SOURCE,(char*)f);
								main.SetStr(hWnd,IDC_CODE,"Encryption");

								rule=false;
							}
							else
							{
								//Decryption(f,s, main.GetInt(hWnd,IDC_PASSWORD));
								DecryptionXOR(f,sz, main.GetStr(hWnd,IDC_PASSWORD));
								main.SetStr(hWnd,IDC_SOURCE,(char*)f);
								main.SetStr(hWnd,IDC_CODE,"Decryption");
								rule=true;
							}
						break;


						case 1://перестановка

							if(rule)
							{
								//Encryption(f,s, main.GetStr(hWnd,IDC_PASSWORD));
	
								EncryptionMIX(f,sz, main.GetInt(hWnd,IDC_PASSWORD));
								main.SetStr(hWnd,IDC_SOURCE,(char*)f);
								main.SetStr(hWnd,IDC_CODE,"Encryption");
								rule=false;
							}
							else
							{
								//Decryption(f,s, main.GetStr(hWnd,IDC_PASSWORD));
								EncryptionMIX(f,sz, main.GetInt(hWnd,IDC_PASSWORD));
								main.SetStr(hWnd,IDC_SOURCE,(char*)f);
								main.SetStr(hWnd,IDC_CODE,"Decryption");
								rule=true;
							}
						break;

						
						case 2://виженера

							if(rule)
							{
								DecryptionViginer(f,sz, main.GetStr(hWnd,IDC_PASSWORD));

								main.SetStr(hWnd,IDC_SOURCE,(char*)f);
								main.SetStr(hWnd,IDC_CODE,"Encryption");
								rule=false;
							}
							else
							{
								EncryptionViginer(f,sz, main.GetStr(hWnd,IDC_PASSWORD));

								main.SetStr(hWnd,IDC_SOURCE,(char*)f);
								main.SetStr(hWnd,IDC_CODE,"Decryption");
								rule=true;
							}
						break;

					}

					break;


				case IDC_DIR:

					ListBox_GetText(GetDlgItem( hWnd,IDC_DIR),ListBox_GetCurSel(GetDlgItem( hWnd,IDC_DIR)), fname);

					delete[] f;
					sz=main.GetFSize(fname);
					f=new BYTE[sz];
					main.LoadFile(fname,f);
					main.SetStr(hWnd,IDC_SOURCE,(char*)f);
					if(sz>0)
					{
						EnableWindow(GetDlgItem(hWnd, IDC_CODE), true);
					}
					else
					{
						EnableWindow(GetDlgItem(hWnd, IDC_CODE), false);
					}
					break;


				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
            break;
		return (TRUE);
    }
    return (FALSE);
}


//------------------------------------------------------------------------------
//							Обработка сообщений окна IDD_ABOUT
//------------------------------------------------------------------------------
BOOL CALLBACK AboutProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
		case WM_CLOSE:
			EndDialog(hWnd, LOWORD(wParam));
			return (TRUE);

        case WM_INITDIALOG:

			hwnd_error = hWnd;

            return TRUE;

        case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				
				case IDOK:
					EndDialog(hWnd, LOWORD(wParam));
					break;

				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
            break;
		return (TRUE);
    }
    return (FALSE);
}


///-----------------------------------------------------------------------------------------------

void* Encryption(void * data,DWORD size, BYTE key)
{
	BYTE *	lpData=NULL;

	lpData=new BYTE[size];
	if(!lpData)
	{
		return NULL;
	}

	lpData=(BYTE*)data;

	for(DWORD i=0; i<size; i++)
	{
		lpData[i]^=key;
	}

	return lpData;
}

void* Decryption(void * data,DWORD size ,BYTE key)
{
	BYTE *	lpData=NULL;

	lpData=new BYTE[size];
	if(!lpData)
	{
		return NULL;
	}

	lpData=(BYTE*)data;

	for(DWORD i=0; i<size; i++)
	{
		lpData[i]^=key;
	}
	return lpData;
}


///-----------------------------------------------------------------------------------------------

void* EncryptionXOR(void * data,DWORD size, char * password)
{
	BYTE* 	lpData=NULL;
	BYTE*	lpPass=NULL;
	char*   buffer=NULL;
	DWORD	szPass;
	DWORD	j=0;

	lpData=(BYTE*)data;
	if(!lpData)
	{
		return NULL;
	}

	szPass=strlen(password);
	buffer=new char[szPass+1];
	strcpy(buffer,password);

	lpPass=(BYTE*)buffer;
	if(!lpPass)
	{
		return NULL;
	}

	for(DWORD i=0; i<size; i++)
	{
		j++;
		lpData[i]^=lpPass[j];

		if(j==szPass)j=0;
	}
	return lpData;
}

void* DecryptionXOR(void * data,DWORD size ,char* password)
{
	BYTE* 	lpData=NULL;
	BYTE*	lpPass=NULL;
	char*   buffer=NULL;
	DWORD	szPass;
	DWORD	j=0;

	lpData=(BYTE*)data;
	if(!lpData)
	{
		return NULL;
	}

	szPass=strlen(password);
	buffer=new char[szPass+1];
	strcpy(buffer,password);

	lpPass=(BYTE*)buffer;
	if(!lpPass)
	{
		return NULL;
	}

	for(DWORD i=0; i<size; i++)
	{
		j++;
		lpData[i]^=lpPass[j];

		if(j==szPass)j=0;
	}
	return lpData;
}

///-----------------------------------------------------------------------------------------------

void* EncryptionMIX(void * data,DWORD size, BYTE key)
{

	BYTE* 	lpData=NULL;
	BYTE	tmp[256];
	BYTE	mix[256];
	int		i=0;
	int		j=0;
	BYTE	_key=key;
	DWORD	whole=(DWORD)(size/_key)*_key;
	DWORD	rest=size-whole;
	bool	exit=true;




	lpData=(BYTE*)data;
	if(!lpData)
	{
		return NULL;
	}

	while(exit)
	{
		if(j<whole)
		{
			if(i<_key)
			{	tmp[i]=lpData[j];
				i++;
				j++;
			}
			else
			{	
				for(i=0;i<_key;i++) 
				{
					mix[i]=tmp[(_key-i)-1];
				}

				for(i=0;i<_key;i++ )
				{
					lpData[((j+i)-_key)]=mix[i];
				}
				i=0;
			}
		}
		else
		{
			/*
			if(i<rest)
			{	tmp[i]=lpData[j];
				i++;
				j++;
			}
			else
			{
				for(i=0 ;i<rest;i++)mix[i]=tmp[rest-i];

				for(i=0;i<rest;i++ )lpData[(j+i)-rest]=tmp[i];

			}
			*/
				exit=false;
		}
	}
	return lpData;
}

void* DecryptionMIX(void * data,DWORD size, BYTE key)
{

	BYTE* 	lpData=NULL;
	BYTE	tmp[256];
	BYTE	mix[256];
	int		i=key;
	int		j=0;
	DWORD	whole=(DWORD)(size/key)*key;
	DWORD	rest=size-whole;
	bool	exit=true;


	lpData=(BYTE*)data;
	if(!lpData)
	{
		return NULL;
	}

	while(exit)
	{
		if(j<whole)
		{
			if(i<key)
			{	tmp[i]=lpData[j];
				i++;
				j++;
			}
			else
			{	
				for(i=0;i<key;i++)mix[i]=tmp[key-i];

				for(i=0;i<key;i++ )
				{
					lpData[(j+i)-key]=tmp[i];
				}
				i=0;
			}
		}
		else
		{
			/*
			if(i<rest)
			{	tmp[i]=lpData[j];
				i++;
				j++;
			}
			else
			{
				for(i=0 ;i<rest;i++)mix[i]=tmp[rest-i];

				for(i=0;i<rest;i++ )lpData[(j+i)-rest]=tmp[i];

			}
			*/
				exit=false;
		}
	}
	return lpData;
}


///-----------------------------------------------------------------------------------------------
void* EncryptionViginer(void * data,DWORD size, char* password)
{
	BYTE* 	lpData=NULL;
	BYTE*	lpPass=NULL;
	char*   buffer=NULL;
	DWORD	szPass;
	DWORD	j=0;
	BYTE	n=256;

	lpData=(BYTE*)data;
	if(!lpData)
	{
		return NULL;
	}

	szPass=strlen(password);
	buffer=new char[szPass+1];
	strcpy(buffer,password);

	lpPass=(BYTE*)buffer;
	if(!lpPass)
	{
		return NULL;
	}

	for(DWORD i=0; i<size; i++)
	{
		j++;

		lpData[i]+=lpPass[j];
		lpData[i]%=256;

		if(j==szPass)j=0;
	}
	return lpData;
}

void* DecryptionViginer(void * data,DWORD size, char* password)
{
	BYTE* 	lpData=NULL;
	BYTE*	lpPass=NULL;
	char*   buffer=NULL;
	DWORD	szPass;
	DWORD	j=0;
	BYTE	n=256;

	lpData=(BYTE*)data;
	if(!lpData)
	{
		return NULL;
	}

	szPass=strlen(password);
	buffer=new char[szPass+1];
	strcpy(buffer,password);

	lpPass=(BYTE*)buffer;
	if(!lpPass)
	{
		return NULL;
	}

	for(DWORD i=0; i<size; i++)
	{
		j++;

		lpData[i]-=lpPass[j];
		lpData[i]%=256;

		if(j==szPass)j=0;
	}
	return lpData;
}