#define STRICT
#define WIN32_LEAN_AND_MEAN 
#include <windows.h> 
#include <windowsx.h>
#include <mmsystem.h>

#include "main.h"
#include "resource.h"

void DX_OnDestroy(HWND hwnd);
void DX_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized);
void DX_OnIdle(HWND hwnd);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND	hMainWnd;
BOOL	bActive;
char	ver[8]="v0.3a";	
char	current_dir[MAX_PATH];

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wndClass;	HWND hWnd;	MSG msg;
	wndClass.cbSize	      =sizeof(wndClass);
	wndClass.style        =CS_HREDRAW|CS_VREDRAW;
	wndClass.lpfnWndProc  =WndProc;
	wndClass.cbClsExtra   =0;
	wndClass.cbWndExtra   =0;
	wndClass.hInstance    =hInst;
	wndClass.hIcon        =LoadIcon(hInst,MAKEINTRESOURCE(IDI_MAIN));
	wndClass.hCursor      =LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground=GetStockBrush(BLACK_BRUSH);
	wndClass.lpszMenuName =NULL;
	wndClass.lpszClassName=ClassName;
	wndClass.hIconSm      =LoadIcon(hInst,MAKEINTRESOURCE(IDI_MAIN));
	RegisterClassEx(&wndClass);

	hWnd=CreateWindowEx(0, ClassName, AppName, WS_POPUP,0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),	NULL, NULL, hInst, NULL);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	hMainWnd=hWnd;

	GetCurrentDirectory(MAX_PATH,current_dir);
	MultiByteToWideChar(CP_ACP, 0, current_dir, -1, wszHomeDir, MAX_PATH);

	InitSin_CosTables();
	NewGame();
	HoleInit();
	InitStar();
	InitLogo();

	if(DEBUG) 
	{
		fUseSound=false;
		fUseMusic=false;
	}

	if (!InitDirectDraw())
		return (FALSE);

	if (!InitDirectInput())
		return (FALSE);

	if (fUseSound) 
	{
		if (!InitDirectSound())
		{
			RemoveDirectSound();
			fUseSound=false;
			//return (FALSE);
		}

		if (fUseMusic)
		{
			if (!InitDirectMusic())
			{
				RemoveDirectMusic();
				fUseMusic=false;
				//return (FALSE);
			}
			else
			{
				PlayMusic(MENU_MUSIC);
			}
		}
	}

	while (TRUE)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
				break;
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (bActive)
			DX_OnIdle(hWnd);
		else
			WaitMessage();
	}
	return (msg.wParam);
}
/////////////////////////////////////////////////
//	Оконная процедура
////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		HANDLE_MSG(hWnd,WM_DESTROY,DX_OnDestroy);
		HANDLE_MSG(hWnd,WM_ACTIVATE,DX_OnActivate);
		case MM_MCINOTIFY:
			if((fUseSound) && (fMusic==MCI) && (wParam==MCI_NOTIFY_SUCCESSFUL))
				mciSendString("play MUSIC from 0 notify",NULL,0,hWnd);

	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
}

/* Обработчики сообщений */

void DX_OnDestroy(HWND hwnd)
{
	if (fUseMusic) RemoveDirectMusic();
	if (fUseSound) RemoveDirectSound();
	RemoveDirectInput();
	RemoveDirectDraw();
	RemoveFonts();

	RemoveGame();
	PostQuitMessage(0);
}
//-----------------------------------------------------------------
void DX_OnIdle(HWND hwnd)
{
	if (bLogo)
	{
		ProcessMouse();
		ProcessKeyboard();
		Logo();
	}
	else
	{
		ProcessKeyboard();
		DrawFrame();
	}
}
//-----------------------------------------------------------------
void DX_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
	if (state==WA_INACTIVE)
		bActive=FALSE;
	else 
		bActive=TRUE;

	AccessMouse(bActive);
	AccessKeyboard(bActive);
}


