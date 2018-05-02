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

HWND	hMainWnd;//глобальный дискриптор окна
BOOL	bActive;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wndClass;
	HWND hWnd;
	MSG msg;
//заполенния структуры окна
	wndClass.cbSize	      =sizeof(wndClass);
	wndClass.style        =CS_HREDRAW|CS_VREDRAW;
	wndClass.lpfnWndProc  =WndProc;
	wndClass.cbClsExtra   =0;
	wndClass.cbWndExtra   =0;
	wndClass.hInstance    =hInst;
	wndClass.hIcon        =LoadIcon(hInst,MAKEINTRESOURCE(IDI_MAIN));
	wndClass.hIconSm      =LoadIcon(hInst,MAKEINTRESOURCE(IDI_MAIN));
	wndClass.hCursor      =LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground=GetStockBrush(BLACK_BRUSH);
	wndClass.lpszMenuName =NULL;
	wndClass.lpszClassName=ClassName;
	RegisterClassEx(&wndClass);
//создания окна
	hWnd=CreateWindowEx(0, ClassName, AppName, WS_POPUP,0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),	NULL, NULL, hInst, NULL);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	hMainWnd=hWnd;

	InitEditor();//инициализация редактора

	if (!InitDirectDraw())//инициализация компонента Direct Draw
		return (FALSE);

	if (!InitDirectInput())//инициализация компонента Direct Input
		return (FALSE);

	while (TRUE)//выборка сообщения
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
//оконная функция
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		HANDLE_MSG(hWnd,WM_DESTROY,DX_OnDestroy);
		HANDLE_MSG(hWnd,WM_ACTIVATE,DX_OnActivate);

		default:
			return DefWindowProc(hWnd,msg,wParam,lParam);
	}
}
//обработка сообщений:
void DX_OnDestroy(HWND hwnd)
{
	RemoveDirectInput();
	RemoveDirectDraw();
	PostQuitMessage(0);
}

void DX_OnIdle(HWND hwnd)
{
	ProcessMouse();
	ProcessKeyboard();
	DrawFrame();
}

void DX_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
	if (state==WA_INACTIVE)
		bActive=FALSE;
	else 
		bActive=TRUE;

	AccessMouse(bActive);
	AccessKeyboard(bActive);
}


