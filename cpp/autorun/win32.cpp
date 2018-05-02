#pragma comment( lib, "version.lib" )

#define STRICT
#define WIN32_LEAN_AND_MEAN

#define CLSNAME "AUTORUN"  
#define APPNAME "AUTORUN"  

#define RUN_KEY		(TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
#define IE_KEY		(TEXT("Software\\Microsoft\\Internet Explorer"))
#define AUTORUN		"\\autorun.exe"
#define SETUP		"setup.exe"
#define IE			"\\ie\\ie6setup.exe"
#define VER			'6'

#include <windows.h>
#include <shellAPI.h>
#include <winreg.h>

bool GetRegValue(LPCTSTR key,unsigned char * value, DWORD size)
{
	HKEY	hRunKey = NULL;  

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, RUN_KEY, 0, KEY_QUERY_VALUE, &hRunKey ))
		return false;

	if(RegQueryValueEx(hRunKey, key, NULL, NULL,(BYTE*)value,&size)!= ERROR_SUCCESS)
		return false;

	RegCloseKey(hRunKey);
	return true;
}


bool SetRegValue(LPCTSTR key ,unsigned char * value, DWORD size)
{
	HKEY	hRunKey = NULL;  

	if (RegCreateKeyEx( HKEY_LOCAL_MACHINE, RUN_KEY, 0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hRunKey, NULL ))
		return false;

	RegSetValueEx(hRunKey, key, NULL,REG_SZ,value,size);// write
	RegCloseKey(hRunKey);
	return true;
}

bool DeleteKey(LPCTSTR key)
{
	HKEY	hRunKey = NULL;  

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, RUN_KEY, 0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hRunKey, NULL ))
		return false;

	RegDeleteValue(hRunKey,key); //delete
	RegCloseKey(hRunKey);
	return true;
}


//-----------------------------------------------------------------------------
// Name: Error()
// Desc: Error window's message 
//-----------------------------------------------------------------------------
void Error(LPCTSTR str)
{
	MessageBox(NULL, str, "Error", MB_OK);
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:

            PostQuitMessage( 0 );
        return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}
//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
    if (FindWindow (CLSNAME,APPNAME))  return 0;

	MSG			msg;
	HWND		hWnd;
	HWND		hWndSetup=NULL;
	WNDCLASSEX	wc;

	bool win9x=false;
	bool winXP=false;

    // Create a class structure for the target window
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = ::WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLSNAME;
	if (!RegisterClassEx(&wc))
		return 0;

    // Create the invisible target window
    hWnd = CreateWindow(CLSNAME, APPNAME, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        NULL, NULL, hInstance, NULL);

    if( NULL == hWnd )
		return 0;


	OSVERSIONINFO info;
	info.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	if(GetVersionEx(&info))
	{

	}

	switch(info.dwPlatformId)
	{
		case VER_PLATFORM_WIN32s:// Windows 32s

		break;

		case VER_PLATFORM_WIN32_WINDOWS:
			win9x=true;
		break;

		case VER_PLATFORM_WIN32_NT:
			winXP=true;
		break;

		default:
			if((info.dwMajorVersion>5) && (info.dwMinorVersion>2))
			{
				winXP=true;
			}
			break;
	}

	unsigned char	value[16];
	DWORD	size;

	char SetupDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,SetupDir);

	char fname[]=AUTORUN;
	char* path=new char[strlen(SetupDir)+strlen(fname)];
	strcpy(path,SetupDir);
	strcat(path,fname);

	SetRegValue("SETUP",(unsigned char*)path,strlen(path));

	hWndSetup = CreateWindowEx(WS_EX_LEFT,"SETUP","SETUP",WS_POPUP,0,0,600,600,NULL,NULL,	hInstance,NULL);

	if (win9x)
	{
		HKEY	hKey = NULL;  

		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, IE_KEY, 0,KEY_READ, &hKey))
		{

		}
		else
		{
			size = sizeof(value);
			if(RegQueryValueEx(hKey, "Version", NULL, NULL,(unsigned char*)&value,&size)!= ERROR_SUCCESS)
			{

			}

			if(value[0]<VER)
			{
				strcat(SetupDir,IE);
				ShellExecute(hWndSetup,"open",SetupDir,"/q",NULL,SW_SHOWNORMAL);
			}
			else
			{
				DeleteKey("SETUP");
				ShellExecute(hWndSetup,"open",SETUP,NULL,NULL,SW_SHOWNORMAL);
			}
		}
		RegCloseKey(hKey);
	}
	else
	{
		DeleteKey("SETUP");
		ShellExecute(hWndSetup,"open",SETUP,NULL,NULL,SW_SHOWNORMAL);
	}

/*
	// Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
*/	
    UnregisterClass( wc.lpszClassName, wc.hInstance );
    return 0;
}
