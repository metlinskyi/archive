#ifndef _MAIN_H
#define _MAIN_H

#define CLASSNAME			"Dolbilka Class"		
#define WINDOWNAME			"Dolbilka Window"
#define DMS_TIP				"Dolbilka"
#define DMS_UIEVENT			"Dolbilka Event"
#define MAIN_ICON			MAKEINTRESOURCE(IDI_ICON1)
#define MAIN_DIALOG			(LPCTSTR)IDD_MAIN
#define DEF_START			"START"
#define DEF_STOP			"STOP"
#define DEF_EXE				"NET"
								

#include <windows.h>
#include <shellapi.h>

class CMAIN
{

public:

    CMAIN() {m_bDolbilka=FALSE;};
    virtual ~CMAIN() {};

    int WinMain(HINSTANCE, HINSTANCE, PWSTR, int);
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
	HWND GetWnd();
	HWND GetWndNet();
	HANDLE GetThread();
	void ExitApplication();


	BOOL			m_bDolbilka;

protected:

    BOOL InitInstance(HINSTANCE hInstance);
    BOOL InitApplication(HINSTANCE hInstance);

    void WideToByte(PWSTR pwzInput, PSTR pszOutput, WORD wStrLength);

    HWND			m_hWnd;
	HWND			m_hNet;
	HICON			m_hIcon;
    HINSTANCE		m_hInst; 
	HANDLE			m_hThread;
	UINT			m_uUIEvent;
	BOOL			m_bDialogActive;
	DWORD			m_dwThreadID;



	NOTIFYICONDATA	nid;
};

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI  SendMsg(PVOID pvoid);
#endif  //_MAIN.H