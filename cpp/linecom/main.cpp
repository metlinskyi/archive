#include "main.h"
#include "resource.h"


CMAIN		main(true);
HWND		hwnd_error;

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
		m_hDlg=	CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);

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
	buffer=new char[(strlen(dir)+strlen(ext)+1)]; 

	strcpy(buffer,dir);
	strcat(buffer,"*");
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

//****************************** ИНТЕРФЕЙС *************************************
//------------------------------------------------------------------------------
//							Обработка сообщений окна IDD_MAIN
//------------------------------------------------------------------------------
BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_CLOSE:
			main.ExitApplication();
			break;
        case WM_INITDIALOG:

			hwnd_error = hWnd;

			SendMessage(hWnd,WM_SETICON, ICON_BIG,(LPARAM) main.GetIcon());
			SendMessage(hWnd,WM_SETICON, ICON_SMALL,(LPARAM) main.GetIcon());
			SetWindowText(hWnd,STR_WND);


            return TRUE;
        case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				
				case IDC_SEE: See(hWnd); break;

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


void See(HWND hwnd)
{
	HANDLE		hFile;
	DWORD		bRead;

	char*		code_tab=NULL;
	char*		real_tab=NULL;
	char*		fclient=NULL;


	DWORD	 	real_cx=0;
	DWORD		code_cx=0;
	DWORD		pass_cx=0;
	DWORD		resl_cx=0;
	DWORD		start_cx=0;
	DWORD		item_cx=0;
	DWORD		tag_cx=0;

	DWORD		pass_size;
	DWORD		real_size;
	DWORD		code_size;
	DWORD		file_size;

	char*		password;
	char		result[32];


	char		pass_tmp[4];
	char		code_tmp[4];
	char		tag[12];
	char*		lp;

	bool		stop=true;

	HMODULE		hModule=NULL;
    HRSRC		hResInfo;
    HGLOBAL		hResData;
    void*		pvRes;


	ZeroMemory(pass_tmp,4);
	ZeroMemory(code_tmp,4);
	ZeroMemory(result,32);
	ZeroMemory(tag,10);


// client.ini
			hFile=CreateFile((LPCSTR)"client.ini",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
			if(hFile==INVALID_HANDLE_VALUE)
			{
				Error("Not find file client.ini");
				return;
			}

			file_size = GetFileSize(hFile,NULL);
			fclient=new char[file_size];

			ReadFile(hFile,fclient,file_size,&bRead,NULL);
			CloseHandle(hFile);

			while(start_cx<file_size)
			{
				item_cx=start_cx;
				tag_cx=0;
				ZeroMemory(tag,10);
				while(tag_cx<9) tag[tag_cx++]=fclient[item_cx++];

				if (strcmp(def_tag,tag)==0)
				{
					lp=&fclient[item_cx];

					break;
				}
				start_cx++;
			}

			memcpy(result,lp,file_size-item_cx-2);

			delete[] fclient;

// password
			pass_size=strlen(result);
			password=new char[pass_size];
			strcpy(password,result);
			ZeroMemory(result,32);
	/*	
// real
			hFile=CreateFile((LPCSTR)"real.tab",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
			if(hFile==INVALID_HANDLE_VALUE)
			{
				Error("Not Open file real.tba");
				return;
			}

			real_size = GetFileSize(hFile,NULL);
			real_tab=new char[real_size];

			ReadFile(hFile,real_tab,real_size,&bRead,NULL);
			CloseHandle(hFile);

*/		
			hResInfo = FindResource(hModule, "IDR_REAL", "TAB"); 
			hResData = LoadResource(hModule, hResInfo) ;
			pvRes = LockResource(hResData);			 

			char* lpsz=(char*)hResData;
			real_tab=new char[96];
			memcpy(real_tab,hResData,96);



// code
			hFile=CreateFile((LPCSTR)"code.tab",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
			if(hFile==INVALID_HANDLE_VALUE)
			{
				Error("Not Open file code.tba");
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

					if(strcmp(code_tmp,pass_tmp)==0)
					{
						 result[resl_cx++]=real_tab[real_cx];
	 
					}
					real_cx++;
				}

			}
// exit
			main.SetStr(hwnd,IDC_PASSWORD,result);

			delete[] password; 
			delete[] real_tab;
			delete[] code_tab;
}
