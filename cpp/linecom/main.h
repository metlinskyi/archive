#ifndef _MAIN_H
#define _MAIN_H

#define CLASSNAME			"I see Class"		//Arbitrary Names used to identify the window
#define WINDOWNAME			"I see Window"
#define DMS_TIP				"I see"
#define DMS_UIEVENT			"I see Event"
#define STR_WND				"I see"
#define def_tag "password="
#define def_pass "85=2>285=6A6;646"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>



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

	void SetStr(HWND,unsigned long,char*);
	void SetInt(HWND,unsigned long,unsigned int);
	void SetFlt(HWND hwnd,unsigned long index, float digit);

	void ExitApplication();

	bool ListBoxVoid(HWND,int);
	void ListBoxDelete(HWND hwnd,int index, char* dir);
	void ListBoxDir(HWND,int,char*,char*);

	friend void Error(LPCTSTR Error);

	long ListCmBoxDir(HWND hwnd,char* dir, char * ext);

protected:

    BOOL InitInstance(HINSTANCE hInstance);
    BOOL InitApplication(HINSTANCE hInstance);

    void WideToByte(PWSTR pwzInput, PSTR pszOutput, WORD wStrLength);

    HWND		m_hWnd;
	HWND		m_hDlg;
	HICON		m_hIcon;
    HINSTANCE	m_hInst;

private:
	bool dlg;

};

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL	CALLBACK AboutProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void See(HWND hwnd);

#endif  //_MAIN.H