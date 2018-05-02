#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")

#define STRICT
#define WIN32_LEAN_AND_MEAN

#define CLSNAME "CLASS 3D"  
#define APPNAME "3D SPACE"  

#include <windows.h>
#include "setup.h"
#include "render.h"


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
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
		case WM_CHAR:
			switch (wParam) 
			{
			case 27:			/* ESC key */
				if (Esc())
				{
					PostQuitMessage(0);
				}
				break;
			}
			return 0;

        case WM_ACTIVATEAPP:
            if((BOOL)wParam)
            {
				TimerStart();
            }
            else
            {
                TimerStop();
            }
        break;

		case WM_SETCURSOR:
         	SetCursor( NULL );
		return TRUE;

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

	if (!Setup(hInstance))	return 0;

	MSG			msg;
	HWND		hWnd;
	WNDCLASSEX	wcex;

	// Create the window class here and register it
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize        = sizeof(wcex);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = MsgProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = NULL;//LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm       = NULL;//LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor       = NULL;//LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = CLSNAME;
	if (!RegisterClassEx(&wcex))
	{
		return 0;
	}

	if (Info.bFullScreen)
	{
		// Create the application's FULLSCREEN
		hWnd = CreateWindow( wcex.lpszClassName, APPNAME, 
								WS_POPUP,0,0,Info.nWidth,Info.nHeight,
								NULL, NULL, wcex.hInstance, NULL );
	}
	else
	{
		// Create the application's window
		hWnd = CreateWindow( wcex.lpszClassName, APPNAME, 
								WS_OVERLAPPED, 
								(GetSystemMetrics(SM_CXSCREEN)- Info.nWidth)/2, 
								(GetSystemMetrics(SM_CYSCREEN)- Info.nHeight)/2, 
								Info.nWidth, Info.nHeight,NULL, NULL, wcex.hInstance, NULL );
	}

    if( NULL == hWnd )
		return 0;

	SetCursor( NULL );
	SetFocus( hWnd );
	SetForegroundWindow(hWnd);


	if(Install(hWnd,Info.nWidth,Info.nHeight,Info.bFullScreen))
	{
		// Show the window
		ShowWindow( hWnd, SW_NORMAL );
		UpdateWindow( hWnd );

		ZeroMemory(&msg, sizeof(MSG));
		while( WM_QUIT != msg.message  ) // Enter the message loop
		{
			if(PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				// Translate and dispatch the message
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				// Render a frame during idle time (no messages are waiting)
				Idle();
			}
		}
	}
	
	UnInstall();
    UnregisterClass( wcex.lpszClassName, wcex.hInstance );
    return 0;
}

