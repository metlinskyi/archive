#ifndef _MAIN_H
#define _MAIN_H

#define CLASSNAME			"Chat Class"		//Arbitrary Names used to identify the window
#define WINDOWNAME			"Chat Window"
#define DMS_TIP				"Chat"
#define DMS_UIEVENT			"Chat Event"
#define	DEBUG				0;

#include <windows.h>
#include <shellapi.h>

class CMAIN
{
public:

    CMAIN() {};
    virtual ~CMAIN() { delete server;};

    int WinMain(HINSTANCE, HINSTANCE, PWSTR, int);
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

	HWND GetWnd();

	bool CreateServer(unsigned short port,int count);
	void ActionServer();
	friend void Error(LPCTSTR Error);

protected:
    BOOL InitInstance(HINSTANCE hInstance);
    BOOL InitApplication(HINSTANCE hInstance);
    void ExitApplication();
    void WideToByte(PWSTR pwzInput, PSTR pszOutput, WORD wStrLength);

    HWND		m_hWnd;              
    HINSTANCE	m_hInst;
	HICON		m_hIcon;
    UINT		m_uUIEvent;            
    BOOL		m_bDialogActive;

    NOTIFYICONDATA nid;

private:

	cserver* server;

};


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgChat(HWND, UINT, WPARAM, LPARAM);

#endif  //_MAIN.H