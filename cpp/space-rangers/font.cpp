//------------------------------------------------------
//	Файл:		FONT.CPP
//	Описание:	Функции для работы со шрифтами
//------------------------------------------------------
#include <windows.h>
#include <windowsx.h>

#include "main.h"

HFONT hFpsFont=NULL,hScoreFont=NULL;

//--------------------------------------------------------
// Инициализация: создание требуемых шрифтов
//--------------------------------------------------------
BOOL InitFonts()
{
	hFpsFont=CreateFont(20,0,0,0,FW_EXTRABOLD,FALSE,FALSE,FALSE,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,VARIABLE_PITCH,"Times New Roman");
	if (hFpsFont==NULL)
	{
		ErrorHandle(hMainWnd,"Times New Roman font creation");
		return (FALSE);
	}

	
	hScoreFont=CreateFont(24,0,0,0,FW_EXTRABOLD,FALSE,FALSE,FALSE,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,VARIABLE_PITCH,"Courier New");
	if (hScoreFont==NULL)
	{
		ErrorHandle(hMainWnd,"Courier New font creation");
		return (FALSE);
	}

	return (TRUE);
}

//--------------------------------------------------------
// Удаление созданных шрифтов
//--------------------------------------------------------
void RemoveFonts()
{
	DeleteFont(hFpsFont);
	DeleteFont(hScoreFont);
}

//--------------------------------------------------------
// Вывод надписи с тенью
//--------------------------------------------------------
void ShadowTextOut(HDC hdc, BYTE font, int x,int y,LPCSTR string)
{
	HFONT hTempFont;
	switch (font)
	{
	case FPS_FONT:
		hTempFont=SelectFont(hdc,hFpsFont);

		SetBkMode(hdc,TRANSPARENT);
	//	SetTextColor(hdc,RGB(130,130,130));
	//	TextOut(hdc,x+2,y+2,string,strlen(string));

		SetTextColor(hdc,RGB(255,255,255));
		TextOut(hdc,x,y,string,strlen(string));
		break;

	case SCORE_FONT:
		hTempFont=SelectFont(hdc,hScoreFont);

		SetBkMode(hdc,TRANSPARENT);
		SetTextColor(hdc,RGB(140,140,140));
		TextOut(hdc,x+3,y+3,string,strlen(string));

		SetTextColor(hdc,RGB(255,255,255));
		TextOut(hdc,x,y,string,strlen(string));
		break;

	}
	SelectFont(hdc, hTempFont);
}


