#ifndef _MAIN_H
#define _MAIN_H

#define CLASSNAME			"Cryption Class"		//Arbitrary Names used to identify the window
#define WINDOWNAME			"Cryption Window"
#define DMS_TIP				"Cryption"
#define DMS_UIEVENT			"Cryption Event"
#define STR_WND				"Cryption"

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
	char*			GetCurDir();

	void SetStr(HWND,unsigned long,char*);
	void SetInt(HWND,unsigned long,unsigned int);
	void SetFlt(HWND hwnd,unsigned long index, float digit);

	void ExitApplication();

	bool ListBoxVoid(HWND,int);
	void ListBoxDelete(HWND hwnd,int index, char* dir);
	void ListBoxDir(HWND,int,char*,char*);

	friend void Error(LPCTSTR Error);

	long ListCmBoxDir(HWND hwnd,char* dir, char * ext);

	void LoadFile(char *,void *);
	void SaveFile(char *,void *,DWORD size);
	DWORD GetFSize(char*);

	char* current_dir;

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

void* EncryptionMIX(void * data,DWORD size, BYTE key);
void* DecryptionMIX(void * data,DWORD size, BYTE key);

void* Encryption(void * data,DWORD size, BYTE key);
void* Decryption(void * data,DWORD size ,BYTE key);

void* EncryptionXOR(void * data,DWORD size, char* password);
void* DecryptionXOR(void * data,DWORD size ,char* password);

void* EncryptionViginer(void * data,DWORD size, char* password);
void* DecryptionViginer(void * data,DWORD size, char* password);


#endif  //_MAIN.H