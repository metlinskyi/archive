#include "main.h"
#include "resource.h"

CMAIN Main;
	
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    return Main.WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
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
	
	m_hIcon=LoadIcon(hInstance,MAIN_ICON);

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
	
	m_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SendMsg,NULL,CREATE_SUSPENDED,&m_dwThreadID);
	if(!m_hThread)
	{
		return FALSE;
	}

    return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return Main.WndProc(hWnd, message, wParam, lParam);
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
                DialogBox(m_hInst,  MAIN_DIALOG, hWnd, (DLGPROC)DialogProc);
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

HWND CMAIN::GetWndNet()
{
	return m_hNet;
}

HANDLE CMAIN::GetThread()
{
	return m_hThread;
}

LRESULT CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:

            return TRUE;

		case WM_CLOSE:
			Main.ExitApplication();
			break;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_CONTROL )
            {
				if(Main.m_bDolbilka)
				{
					SetWindowText(GetDlgItem(hWnd, IDC_CONTROL),DEF_START);
					Main.m_bDolbilka=FALSE;
				}
				else
				{
					SetWindowText(GetDlgItem(hWnd, IDC_CONTROL),DEF_STOP);
					Main.m_bDolbilka=TRUE;
					ResumeThread(Main.GetThread());
				}
                return TRUE;
            }
            break;
    }
    return FALSE;
}

DWORD WINAPI SendMsg(PVOID pvoid)
{
	
	//ShellExecute(Main.GetWndNet(),NULL,"LOOP","RAMZES Test",NULL,NULL);

    while (Main.m_bDolbilka)
    {
		ShellExecute(Main.GetWndNet(),NULL,DEF_EXE,"SEND RAMZES Test",NULL,NULL);
    }
	return 0;
}