#include "server.h"
#include "main.h"
#include "resource.h"

CMAIN	main;

HWND		hwnd_error;

void Error(LPCTSTR Error)
{
	char buffer[255];
	wsprintf(buffer,"Îøèáêà: %s",Error); 
	MessageBox(hwnd_error, buffer,"class CNODE",MB_OK | MB_ICONSTOP);
}

	
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    return main.WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}


int CMAIN::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

    if (!InitApplication(hInstance))
    {
        OutputDebugString( "Could not initialize application\n" );
        return FALSE;
    }

    if (!InitInstance(hInstance))
    {
        OutputDebugString( "Could not initialize instance\n" );
        return FALSE;
    }

	if(!CreateServer(10000,10))
	{
        OutputDebugString( "Could not create server\n" );
        return FALSE;
	}

    while (GetMessage(&msg, NULL, 0, 0))
    {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }
    return (msg.wParam);
}

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

BOOL CMAIN::InitInstance(HINSTANCE hInstance)
{
    m_hInst = hInstance;
    m_hWnd = CreateWindow(CLASSNAME,WINDOWNAME,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,CW_USEDEFAULT,0,NULL,NULL,hInstance, NULL);
    if (!m_hWnd)
    {
        OutputDebugString( "Could not create window\n" );
        return FALSE;
    }

	// Build the icon in the tray
	m_hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_TRAY));

    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = m_hWnd;
    nid.hIcon = m_hIcon;
    nid.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
    strcpy( nid.szTip, DMS_TIP);

    // Establish a message ID for communication from the tray icon to the target window
    m_uUIEvent = RegisterWindowMessage( DMS_UIEVENT );
    nid.uCallbackMessage = m_uUIEvent;
    Shell_NotifyIcon( NIM_ADD, &nid); 

    return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return main.WndProc(hWnd, message, wParam, lParam);
}

LRESULT CMAIN::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == m_uUIEvent)
    {
        if (lParam == WM_LBUTTONDBLCLK || lParam == WM_LBUTTONDOWN)
        {
            if( !m_bDialogActive )
            {
                m_bDialogActive = TRUE;
                DialogBox(m_hInst,  (LPCTSTR)IDD_CHAT, hWnd, (DLGPROC)DlgChat);
                m_bDialogActive = FALSE;
            }
        }
        return TRUE;
    }

    switch (message)
    {
		case WM_TIMER:

			return (0L);
        case WM_DESTROY:
            ExitApplication();
            break;

        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return TRUE;
}

void CMAIN::ExitApplication()
{
	NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = m_hWnd;
    Shell_NotifyIcon( NIM_DELETE, &nid); 

	DestroyIcon(m_hIcon);

    PostQuitMessage(0);
}

void CMAIN::WideToByte(PWSTR pwzInput, PSTR pszOutput, WORD wStrLength)
{
    WideCharToMultiByte( CP_ACP, 0, pwzInput, wStrLength, pszOutput, wStrLength, NULL, NULL);
}

HWND CMAIN::GetWnd()
{
	return m_hWnd; 
}

void observer (char*)
{

}

LRESULT CALLBACK DlgChat(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_MOUSEMOVE:

			break;

		case WM_LBUTTONDOWN:
     
			break;
		case WM_LBUTTONUP:
 
			break;
        case WM_INITDIALOG:
			hwnd_error=hWnd;

            return TRUE;

        case WM_COMMAND:
			if (LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hWnd, TRUE);
                return TRUE;
            }

            break;
    }
    return FALSE;
}

//-------------------------------- CHAT ---------------------------------------------------

bool CMAIN::CreateServer(unsigned short port,int count)
{
	server=new cserver;
	return server->create(port,count);
}

void CMAIN::ActionServer()
{
	server->action();
}