#ifndef _MAIN_H
#define _MAIN_H

#define CLASSNAME			"Рассылка .0 Class"		//Arbitrary Names used to identify the window
#define WINDOWNAME			"Рассылка 1.0 Window"
#define DMS_TIP				"Рассылка 1.0"
#define DMS_UIEVENT			"Рассылка 1.0 Event"
#define STR_WND				"Рассылка 1.0"
#define TMR_ID			1

#define RUN_KEY		(TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
#define	SOFT_KEY	"Dispather"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <ras.h>
#include <raserror.h>
#include <shellapi.h>
#include <winreg.h>

class CMAIN
{

public:

	CMAIN()				{	dlg=false;}
	CMAIN(bool flag)	{	dlg=flag;}
    virtual ~CMAIN()	{};

    int WinMain(HINSTANCE, HINSTANCE, PWSTR, int);
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);


	HWND			GetWnd();
	HWND			GetDlg();
	HICON			GetIcon();
	HINSTANCE		GetInst();
	char*			GetStr(HWND,unsigned long);
	unsigned int	GetInt(HWND,unsigned long);
	float			GetFlt(HWND hwnd,unsigned long index);
	char*			GetDir();

	void SetStr(HWND,unsigned long,char*);
	void SetInt(HWND,unsigned long,unsigned int);
	void SetFlt(HWND hwnd,unsigned long index, float digit);

	void ExitApplication();

	bool ListBoxVoid(HWND,int);
	void ListBoxDelete(HWND hwnd,int index, char* dir);
	void ListBoxDir(HWND,int,char*,char*);

	friend void Error(LPCTSTR Error);

	long ListCmBoxDir(HWND hwnd,char* dir, char * ext);

	void LoadFile(char * path,void * data);
	void SaveFile(char * path, void * data,DWORD size);
	DWORD GetFSize(char* path);
	int RndInt( int low, int high );
protected:

    BOOL InitInstance(HINSTANCE hInstance);
    BOOL InitApplication(HINSTANCE hInstance);

    void WideToByte(PWSTR pwzInput, PSTR pszOutput, WORD wStrLength);

    HWND		m_hWnd;
	HWND		m_hDlg;
	HICON		m_hIcon;
    HINSTANCE	m_hInst;
    UINT		m_uUIEvent;            // the registered message sent from the popup
    BOOL		m_bAboutBoxActive;

private:
	bool dlg;
	char current_dir[MAX_PATH];

	NOTIFYICONDATA nid;
	HICON		hIcon;
	BYTE		id_icon;

	unsigned int interval;

};

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK AboutProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


#endif  //_MAIN.H