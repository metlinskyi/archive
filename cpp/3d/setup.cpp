//---------------------------------------------------------------
//	Файл:		CONNECT.CPP
//	Описание:	Получение от пользователя параметров игры
//---------------------------------------------------------------
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdio.h>
#include "setup.h"
#include "res3d.h"

#pragma comment ( lib, "version.lib" )

SETUPINFO Info;

//-----------------------------------------------------------------------------
// External function-prototypes
//-----------------------------------------------------------------------------
extern HRESULT GetDXVersion(DWORD* pdwDirectXVersion,TCHAR* strDirectXVersion,int cchDirectXVersion );


BOOL CALLBACK SetupDlg(HWND, UINT, WPARAM, LPARAM);


HWND hRenderer;//Дескриптор списка A.P.I.
HWND hResolution;//Дескриптор списка разрещений

void StrMultiLine(HWND hWnd,int index,char* msg)
{
	char*	buffer;
	char*	tmpBuffer;
	int		OldLength;
	int		NewLength;

	HWND hwnd   =GetDlgItem(hWnd,index);

	OldLength	= GetWindowTextLength(hwnd);
	NewLength	= OldLength+(int)strlen(msg)+5;
	
 	buffer	= new char[NewLength];
	tmpBuffer= new char[OldLength+1];

	GetWindowText(hwnd,tmpBuffer,OldLength+1);
		
	strcpy(buffer,tmpBuffer); 
	strcat(buffer,"\r\n");
	strcat(buffer,msg);

	SetWindowText(hwnd,buffer);


	delete[] buffer;
	delete[] tmpBuffer;

	int k=Edit_GetLineCount(hwnd);
	for (int i=0;i<k;i++) SendMessage(hwnd,EM_LINESCROLL ,0,k);
}

//----------------------------------------------------
// Функция для получения настроек от пользователя
//----------------------------------------------------
bool Setup(HINSTANCE hInst)
{
	
	//Заполнение структуры cDesc для вызова диалогового окна
	ZeroMemory(&Info,sizeof(SETUPINFO));

    DWORD		dwDirectXVersion = 0;
    TCHAR		strDirectXVersion[10];
/*
	//Инициализация элементов управления
	INITCOMMONCONTROLSEX iccex;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_BAR_CLASSES ;
	InitCommonControlsEx(&iccex);
*/
	OSVERSIONINFO info;

	info.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);

	if(GetVersionEx(&info))
	{

	}


	switch(info.dwPlatformId)
	{
		case VER_PLATFORM_WIN32s:
			_tcsncpy( Info.strWindows, TEXT("OS: Windows 32s"), STR_SIZE );
			 // strResult[STR_SIZE-1] = 0;
		break;

		case VER_PLATFORM_WIN32_WINDOWS:

			if((info.dwMajorVersion==4) && (info.dwMinorVersion==0))
			{
				_tcsncpy(Info.strWindows, TEXT("OS: Windows 95"), STR_SIZE );
			}

				if((info.dwMajorVersion==4) && (info.dwMinorVersion==10))
			{
				_tcsncpy(Info.strWindows, TEXT("OS: Windows 9x"), STR_SIZE );
			}

		break;

		case VER_PLATFORM_WIN32_NT:
				
			//_sntprintf( Info.strWindows,STR_SIZE, TEXT("OS: Windows %d"), info.dwMajorVersion );

			if((info.dwMajorVersion==4) && (info.dwMinorVersion==0))
			{
				_tcsncpy(Info.strWindows, TEXT("OS: Windows NT"), STR_SIZE );
			}

			if((info.dwMajorVersion==5) && (info.dwMinorVersion==0))
			{
				_tcsncpy(Info.strWindows, TEXT("OS: Windows 2000"), STR_SIZE );
			}

			if((info.dwMajorVersion==5) && (info.dwMinorVersion==1))
			{
				_tcsncpy(Info.strWindows, TEXT("OS: Windows XP"), STR_SIZE );
			}

			if((info.dwMajorVersion==5) && (info.dwMinorVersion==2))
			{
				_tcsncpy(Info.strWindows, TEXT("OS: Windows 2003"), STR_SIZE );
			}

		break;

		default:

			_tcsncpy( Info.strWindows, TEXT("OS: unknow"), STR_SIZE );
			 // strResult[STR_SIZE-1] = 0;
			break;
	}

    if( SUCCEEDED(GetDXVersion( &dwDirectXVersion, strDirectXVersion, 10)))
    {
        if( dwDirectXVersion > 0 )
			_sntprintf( Info.strDXVer,STR_SIZE, TEXT("DX: %s"), strDirectXVersion );

        else
			_tcsncpy( Info.strDXVer, TEXT("DX: not installed"),STR_SIZE );
        //strResult[STR_SIZE-1] = 0;
    }
    else
    {
		_sntprintf( Info.strDXVer,STR_SIZE, TEXT("DX: unknown"));
       // strResult[STR_SIZE-1] = 0;
    }


	Info.bMMX=IsProcessorFeaturePresent(PF_MMX_INSTRUCTIONS_AVAILABLE);		//MMX:
	Info.bSSE1=IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE);	//SSE:
	Info.bSSE2=IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE_DOUBLE);	//SSE2:
	Info.b3DNOW=IsProcessorFeaturePresent(PF_3DNOW_INSTRUCTIONS_AVAILABLE); //3DNOW:


	_tcsncpy( Info.strProcessor, TEXT("CPU:"), STR_SIZE );

	//Диалог выбора настроек
	if (!DialogBox(hInst,MAKEINTRESOURCE(IDD_SETUP),NULL,SetupDlg))
		return (FALSE);

	return (TRUE);
}

//-------------------------------------------------------
// Диалоговая процедура окна "Подключение к Master Mind"
//-------------------------------------------------------
BOOL CALLBACK SetupDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:

		CheckDlgButton(hwnd,IDC_FULLSCREEN,BST_UNCHECKED);
		Info.bFullScreen=false;

		hRenderer = GetDlgItem(hwnd,IDC_RENDERER);
		hResolution = GetDlgItem(hwnd,IDC_RESOLUTION);

       	ComboBox_AddString(hRenderer,"Soft");
       	ComboBox_AddString(hRenderer,"OpenGL");
       	ComboBox_AddString(hRenderer,"Direct3D9");

       	ComboBox_AddString(hResolution,"648x480");
       	ComboBox_AddString(hResolution,"800x600");
       	ComboBox_AddString(hResolution,"1024x768");
       	ComboBox_AddString(hResolution,"1280x1024");

		ComboBox_SetCurSel(hRenderer, 2);
		ComboBox_SetCurSel(hResolution, 2);


		StrMultiLine(hwnd,IDC_INFO,Info.strWindows);
		StrMultiLine(hwnd,IDC_INFO,Info.strDXVer);
		StrMultiLine(hwnd,IDC_INFO,Info.strProcessor);

		if(Info.bMMX)
			StrMultiLine(hwnd,IDC_INFO,"MMX: available");
		else
			StrMultiLine(hwnd,IDC_INFO,"MMX: not available");

		if(Info.bSSE1)
			StrMultiLine(hwnd,IDC_INFO,"SSE: available");
		else
			StrMultiLine(hwnd,IDC_INFO,"SSE: not available");

		if(Info.bSSE2)
			StrMultiLine(hwnd,IDC_INFO,"SSE2: available");
		else
			StrMultiLine(hwnd,IDC_INFO,"SSE2: not available");

		if(Info.b3DNOW)
			StrMultiLine(hwnd,IDC_INFO,"3DNOW: available");
		else
			StrMultiLine(hwnd,IDC_INFO,"3DNOW: not available");


		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDC_FULLSCREEN:

			break;

			case IDOK: // Нажата кнопка ОК

				switch ( ComboBox_GetCurSel(hResolution))
				{
					case 0:
						Info.nHeight=480;
						Info.nWidth=640;
						break;

					case 1:
						Info.nHeight=600;
						Info.nWidth=800;			
						break;

					case 2:			
						Info.nHeight=768;
						Info.nWidth=1024;
						break;

					case 3:
						Info.nHeight=1024;
						Info.nWidth=1280;
						break;

				}

				if (BST_CHECKED==IsDlgButtonChecked(hwnd,IDC_FULLSCREEN))
				{
					Info.bFullScreen=true;
				}
				else
				{
					Info.bFullScreen=false;
				}


     
				EndDialog(hwnd,TRUE);
			return (TRUE);
		}

		break;
	case WM_CLOSE:
		EndDialog(hwnd,FALSE);
		return (TRUE);
					
	}
	return (FALSE);
}
