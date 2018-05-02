#include "main.h"
#include "resource.h"


typedef int (*OperatorType)(char*);
typedef bool (*SendType)(char*,char*);

void Dispath(int left, int right,char* msg);


CMAIN		main(true);
HWND		hwnd_error;
HMODULE     hOperatorDLL;
//FARPROC		pOperator; 
//FARPROC		pSend; 
DWORD		dwThreadId;

bool		start=false;

OperatorType pOperator;
SendType pSend;
 
char tmp[256];

void Error(LPCTSTR Error)
{
	char buffer[255];
	wsprintf(buffer,"Ошибка: %s",Error); 
	MessageBox(hwnd_error, buffer,STR_WND,MB_OK | MB_ICONSTOP);
}

bool LoadPlugin(char * name);
bool InternetConnected ();

//-------------------------------------------------------------------------------	
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    return main.WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
//-------------------------------------------------------------------------------
int CMAIN::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	MSG						msg;

// LINK:comctl32.lib
/*

	INITCOMMONCONTROLSEX	iccex;

	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_BAR_CLASSES ;
	InitCommonControlsEx(&iccex);

*/
	GetCurrentDirectory(MAX_PATH,current_dir);
	strcat(current_dir,"\\");


	if (!dlg)
	{
		if (!InitApplication(hInstance))
		{
			Error( "Инициализаций приложения...\n" );
			return FALSE;
		}

		if (!InitInstance(hInstance))
		{
			Error( "Инициализаций...\n" );
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
/*
		if (!CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Dispath,NULL,0,&dwThreadId))
		{	
			Error("Сеанс рассылки несоздан...");
		}

*/		while(!appDone)
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
        Error( "Невозможно создать окно...\n" );
        return FALSE;
    }

	// Build the icon in the tray
	hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MAIN));

    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = m_hWnd;
    nid.hIcon = hIcon;
    nid.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
    strcpy( nid.szTip, DMS_TIP);

    // Establish a message ID for communication from the tray icon to the target window
    m_uUIEvent = RegisterWindowMessage( DMS_UIEVENT );
    nid.uCallbackMessage = m_uUIEvent;
    Shell_NotifyIcon( NIM_ADD, &nid); 

	interval =15000;

	SetTimer(m_hWnd,TMR_ID, interval,NULL);

	return TRUE;
}

//-------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return main.WndProc(hWnd, message, wParam, lParam);
}

//-------------------------------------------------------------------------------
void CMAIN::WideToByte(PWSTR pwzInput, PSTR pszOutput, WORD wStrLength)
{
    WideCharToMultiByte( CP_ACP, 0, pwzInput, wStrLength, pszOutput, wStrLength, NULL, NULL);
}
//-------------------------------------------------------------------------------
void CMAIN::ExitApplication()		
{
	KillTimer(m_hWnd,TMR_ID);

    //Remove DMShell icon from the tray
	NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = m_hWnd;
    Shell_NotifyIcon( NIM_DELETE, &nid); 

	DestroyIcon(hIcon);

	PostQuitMessage(0);
}
//------------------------ GET CMAIN ---------------------------------------

HWND		CMAIN::GetWnd()				{	return m_hWnd; }
HWND		CMAIN::GetDlg()				{	return m_hDlg; }
HICON		CMAIN::GetIcon()			{   return m_hIcon;}
HINSTANCE	CMAIN::GetInst()			{	return m_hInst;}
char*		CMAIN::GetDir()				{	return  current_dir;}


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

long CMAIN::ListCmBoxDir(HWND hwnd,char* dir, char * ext)
{
	WIN32_FIND_DATA		ffd;
	HANDLE				hFile;
	bool				fFile=true;
	long				count=0;
	char				mask[8];

	strcpy(mask,"*.");
	strcat(mask,ext);

	hFile=FindFirstFile(mask,&ffd);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		while(fFile)
		{
			ComboBox_AddString(hwnd,ffd.cFileName);
			count++;
			fFile=(bool)FindNextFile(hFile,&ffd);
		}
		FindClose(hFile);
	}

	return  count;
}
int CMAIN::RndInt( int low, int high )
{
    int range = high - low;
    int num = rand() % range;
    return( num + low );
}


//****************************** ИНТЕРФЕЙС *************************************
//-------------------------------------------------------------------------------
LRESULT CMAIN::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == m_uUIEvent)
    {
        if (lParam == WM_LBUTTONDBLCLK || lParam == WM_LBUTTONDOWN)
        {
            if( !m_bAboutBoxActive )
            {
                m_bAboutBoxActive = TRUE;
                DialogBox(m_hInst,  (LPCTSTR)IDD_MAIN, hWnd, (DLGPROC)DlgProc);
                m_bAboutBoxActive = FALSE;
            }
        }
        return TRUE;
    }

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
//------------------------------------------------------------------------------
//							Обработка сообщений окна IDD_MAIN
//------------------------------------------------------------------------------
BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
		case WM_CLOSE:
			FreeLibrary(hOperatorDLL);
			main.ExitApplication();

			break;
        case WM_INITDIALOG:

			hwnd_error = hWnd;

			SendMessage(hWnd,WM_SETICON, ICON_BIG,(LPARAM) main.GetIcon());
			SendMessage(hWnd,WM_SETICON, ICON_SMALL,(LPARAM) main.GetIcon());
			SetWindowText(hWnd,STR_WND);

			if(LoadPlugin("jeans.dll"))
			{
				pOperator(tmp);
				ComboBox_AddString(GetDlgItem(hWnd, IDC_OPERATOR),tmp);
				ComboBox_SetCurSel(GetDlgItem(hWnd, IDC_OPERATOR), 0);

			}
			else
			{
				Error("Оператор ненайден...");
			}


			main.SetStr(hWnd,IDC_MSG,"Pozvoni+mne+8050290450");

				
		    return TRUE;


        case WM_COMMAND:
			switch (LOWORD(wParam))
			{

			case IDC_START:

				if (start)
				{
					start=false;
					main.SetStr(hWnd,IDC_START,"Старт");
				}
				else
				{
					start=true;
					main.SetStr(hWnd,IDC_START,"Стоп");
					Dispath(
						main.GetInt(hWnd,IDC_FROM), 
						main.GetInt(hWnd,IDC_TO),
						main.GetStr(hWnd,IDC_MSG));

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
				

				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
            break;
		return (TRUE);
    }
    return (FALSE);
}


bool InternetConnected ()
{
	DWORD	nRasDevices = 20;
	DWORD	nSize = 20;
	RASCONN rasconnected [10];  // Ras Connected struct

	rasconnected[0].dwSize = sizeof(RASCONN);
	RasEnumConnections( rasconnected, &nSize, &nRasDevices );

    return ( BOOL (nRasDevices) );
}


bool LoadPlugin(char * name)
{
	hOperatorDLL=LoadLibrary(name);
	if (hOperatorDLL==NULL)
	{
		return false;
	}

	pOperator=(OperatorType)GetProcAddress(hOperatorDLL,"Operator");
	if (pOperator==NULL) 
	{
		FreeLibrary(hOperatorDLL);
		return false;
	}

	pSend=(SendType)GetProcAddress(hOperatorDLL,"Send");
	if (pSend==NULL) 
	{
		FreeLibrary(hOperatorDLL);
		return false;
	}

	return true;
}

void Dispath(int left, int right,char* msg)
{
	//for (unsigned int phone=2888729;phone<2904509;phone++)
//	{
		int phone =2888729;

		char buffer[32];

		wsprintf(buffer,"%d",phone); 
		pSend(buffer,msg);

//	}

}