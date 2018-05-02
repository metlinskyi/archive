#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "main.h"
#include "resource.h"


LPDIRECTDRAW7			pDD=NULL;
LPDIRECTDRAWSURFACE7	pPrimarySurface=NULL;//первичная поверхность
LPDIRECTDRAWSURFACE7	pBackBuffer=NULL;//вторичная поверхность
LPDIRECTDRAWPALETTE		pDDPal=NULL;
LPDIRECTDRAWCLIPPER		pClipper=NULL;			

LPDIRECTDRAWSURFACE7	pRED=NULL;
LPDIRECTDRAWSURFACE7	pGREEN=NULL;
LPDIRECTDRAWSURFACE7	pBLUE=NULL;
LPDIRECTDRAWSURFACE7	pMouse=NULL;
LPDIRECTDRAWSURFACE7	pPlus=NULL;
LPDIRECTDRAWSURFACE7	pMinus=NULL;
LPDIRECTDRAWSURFACE7	pButton=NULL;

DDCOLORKEY				ddColorKey;

SurfaceLocation			fSurfLoc=BOTH;

LPDIRECTDRAWSURFACE7 CreateSurface(int x, int y);
BOOL	CreateWorkSurfaces();
BOOL	CreateGameSurfaces();
BOOL	PrepareSurfaces();
BOOL	LoadBMP(LPDIRECTDRAWSURFACE7 pSurface, char* filename);
BOOL	LoadBMPinEXE(LPDIRECTDRAWSURFACE7 lpdds, LPSTR lpstrResID);
BOOL	CopyBitmapToYUVSurface(LPDIRECTDRAWSURFACE7 lpDDSurf, HBITMAP hbm);

int		rndInt( int low, int high );
double	rndDouble( double low, double high );
void	InitSin_CosTables(void);
void	OutStr(int x, int y, LPCTSTR text);
void	InitObject(OBJECT * obj,int x, int y, int w,int h);
void	DrawObject(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj);
void	DrawObjectSRC(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj);
void 	DrawHole();
void	DrawControl();
bool	Crossing(OBJECT *obj, int x, int y);

OBJECT	mouse;
OBJECT	points[MAX_POINT];
OBJECT	screen;
OBJECT	fps;
OBJECT	plus[32];
OBJECT	minus[32];

RGB		font_color;
RECT	rPic;
	
char	buffer[256];

double	SinTable[360];
double	CosTable[360];

int		index;
int		z=0;
int		gravity=1;
float	fz1=0;
float	fz2=0;
float	tan_x=0;
float	tan_y=0;
float	tx=0;
float	ty=0;
float	ry=0;
float	rx=0;

BYTE	CurrentPoint=1;
BYTE	MaxPoint=4;
BYTE	Delay=0;
BYTE	var=0;
//----------------------------------------------------
// функция вызываемая в случае ошибки
//----------------------------------------------------
void ErrorHandle(HWND hwnd, LPCTSTR szError)
{
	char szErrorMessage[255];

	RemoveDirectDraw();
	ShowWindow(hwnd,SW_HIDE);
	wsprintf(szErrorMessage,"Произошла внутренняя ошибка.\n%s.",szError);	
	MessageBox(hwnd, szErrorMessage, AppName, MB_OK|MB_ICONWARNING);
	DestroyWindow(hwnd);   
}
//------------------------------------------------------
// инициализирует все объекты, связанные с DirectDraw
//------------------------------------------------------
BOOL	InitDirectDraw ()
{
	HRESULT hRet;
	//создание интерфейса DirectX	
	hRet=DirectDrawCreateEx(NULL, (VOID**)&pDD, IID_IDirectDraw7, NULL);
	if (FAILED(hRet))
	{
		ErrorHandle(hMainWnd,"Direct Draw Create");
		return (FALSE);
	}
	//установка уровня кооперации с устройством
	hRet=pDD->SetCooperativeLevel(hMainWnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);
	if (FAILED(hRet))
	{
		ErrorHandle(hMainWnd,"неможем скоопирироватся");
		return (FALSE);
	}

	hRet=pDD->SetDisplayMode(MAX_WIDTH, MAX_HEIGHT, COLOR_DEPTH,0,0);
	if (FAILED(hRet))
	{
		ErrorHandle(hMainWnd,"Ваш монитор нам не подходит!!!");				
		return (FALSE);
	}

	if (!CreateWorkSurfaces())//создание рабочих поверхностей
	{
		ErrorHandle(hMainWnd,"Create Work Surfaces");				
		return (FALSE);
	}

	if (!CreateGameSurfaces())//создание вспомогательных поверхностей
	{
		ErrorHandle(hMainWnd,"Create Game Surfaces");				
		return (FALSE);
	}

	if (!PrepareSurfaces())//загрузка данных в поверхность
	{
		ErrorHandle(hMainWnd,"Вы удалили файл pal.dat, верните его на место!!!");				
		return (FALSE);
	}
	return (TRUE);
}
//----------------------------------------------------
// Очищает выбранную поверхность
//----------------------------------------------------
BOOL ClearSurface(LPDIRECTDRAWSURFACE7 pSurface)
{
	DDSURFACEDESC2 ddSurfaceDesc;
	HRESULT hRet;

	ZeroMemory(&ddSurfaceDesc,sizeof(ddSurfaceDesc));
	ddSurfaceDesc.dwSize=sizeof(ddSurfaceDesc);

	hRet=pSurface->Lock(NULL, &ddSurfaceDesc, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL);
	if(FAILED(hRet))
	{
		return (FALSE);
	}

	UINT surfaceWidth=ddSurfaceDesc.lPitch;
	UINT surfaceHeight=ddSurfaceDesc.dwHeight;

	char *buf=(char*)ddSurfaceDesc.lpSurface;
	ZeroMemory(buf,surfaceWidth*surfaceHeight);
	pSurface->Unlock(NULL);
	return (TRUE);
}
//------------------------------------------------------
// Создаёт поверхность с выбранной шириной и высотой
//------------------------------------------------------
LPDIRECTDRAWSURFACE7 CreateSurface(int x, int y)
{
	LPDIRECTDRAWSURFACE7 pDDSurface;
	DDSURFACEDESC2 ddSurfaceDesc;
	HRESULT hRet;
	
	if ((fSurfLoc==BOTH) || (fSurfLoc==VIDEO))
	{
		ZeroMemory(&ddSurfaceDesc,sizeof(ddSurfaceDesc));
		ddSurfaceDesc.dwSize=sizeof(ddSurfaceDesc);
		ddSurfaceDesc.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
		ddSurfaceDesc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddSurfaceDesc.dwHeight=y;
		ddSurfaceDesc.dwWidth=x;
		
		hRet=pDD->CreateSurface(&ddSurfaceDesc,&pDDSurface,NULL);
		if(SUCCEEDED(hRet))
			return (pDDSurface);
		else if (fSurfLoc==VIDEO)
			return (NULL);
	}

	if ((fSurfLoc==BOTH) || (fSurfLoc==SYSTEM))
	{
		ZeroMemory(&ddSurfaceDesc,sizeof(ddSurfaceDesc));
		ddSurfaceDesc.dwSize=sizeof(ddSurfaceDesc);
		ddSurfaceDesc.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
		ddSurfaceDesc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
		ddSurfaceDesc.dwHeight=y;
		ddSurfaceDesc.dwWidth=x;

		hRet=pDD->CreateSurface(&ddSurfaceDesc,&pDDSurface,NULL);
		if(SUCCEEDED(hRet))
			return (pDDSurface);
		else if (fSurfLoc==SYSTEM)
			return (NULL);
	}
	return (NULL);
}
//--------------------------------------------------------
// Создаёт палитру DirectDraw на основе файла "PAL.BMP"
//--------------------------------------------------------
LPDIRECTDRAWPALETTE CreateDirectDrawPalette(LPDIRECTDRAW7 pDD)
{
	LPDIRECTDRAWPALETTE pDirectDrawPal;
	PALETTEENTRY palEntries[256];
	HRESULT hRet;
	LPRGBQUAD pColorTable;
	UINT uMemNeed=sizeof(RGBQUAD)*256;
	DWORD nBytesRead;

	HANDLE hFile=CreateFile("pal.dat",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		pDirectDrawPal=NULL;
		return (pDirectDrawPal);
	}
	
	pColorTable= (LPRGBQUAD)malloc(uMemNeed);
	SetFilePointer(hFile,sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER),NULL,FILE_BEGIN);
	ReadFile(hFile, (LPVOID)pColorTable,uMemNeed, &nBytesRead,NULL);
	CloseHandle(hFile);
		
	for (int x=0;x<256;++x)
	{
		palEntries[x].peRed=pColorTable[x].rgbRed;
		palEntries[x].peBlue=pColorTable[x].rgbBlue;
		palEntries[x].peGreen=pColorTable[x].rgbGreen;
	}
	
	hRet=pDD->CreatePalette(DDPCAPS_8BIT|DDPCAPS_ALLOW256,palEntries,&pDirectDrawPal,NULL);
	if (FAILED(hRet))
		pDirectDrawPal=NULL;

	free(pColorTable);
	
	return (pDirectDrawPal);
}
//--------------------------------------------------------
// Загружает выбранный файл на выбранную поверхность
//--------------------------------------------------------
BOOL LoadBMP(LPDIRECTDRAWSURFACE7 pSurface, char* filename)
{
	HRESULT hRet;
	DDSURFACEDESC2 ddSurfaceDesc;
	
	BYTE* pBmp;
	DWORD dwBmpSize;
	DWORD dwFileLength;
	DWORD nBytesRead;
	LONG  lX,lY;
	BYTE magicX=0;
	
	BYTE* pPixels;
	BYTE* pSurfBits;

	HANDLE hFile=CreateFile(filename,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	
	if (hFile==INVALID_HANDLE_VALUE)
		return (FALSE);
		
	dwFileLength=GetFileSize (hFile, NULL) ;
	dwBmpSize=dwFileLength-sizeof(BITMAPFILEHEADER);

	pBmp= (BYTE*) malloc(dwBmpSize);
	SetFilePointer(hFile,sizeof(BITMAPFILEHEADER),NULL,FILE_BEGIN);
	ReadFile(hFile, (LPVOID)pBmp, dwBmpSize, &nBytesRead,NULL);
	CloseHandle(hFile);

	pPixels=pBmp+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256;
	lX=((BITMAPINFOHEADER*)pBmp)->biWidth;
	lY=((BITMAPINFOHEADER*)pBmp)->biHeight;

	ZeroMemory(&ddSurfaceDesc,sizeof(ddSurfaceDesc));
	ddSurfaceDesc.dwSize=sizeof(ddSurfaceDesc);

	hRet=pSurface->Lock(NULL, &ddSurfaceDesc, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL);
	if(FAILED(hRet))
	{
		return (FALSE);
	}

	pSurfBits=(BYTE*)ddSurfaceDesc.lpSurface;

	while (((lX+magicX)%4)!=0)	magicX++;
	
	pPixels=pPixels + (lY-1)*(lX+magicX);
			
	for (LONG i=0;i<lY;++i)
	{
		CopyMemory(pSurfBits,pPixels,lX);
		pSurfBits+=ddSurfaceDesc.lPitch;
		pPixels-=lX+magicX;
	}
	
	pSurface->Unlock(NULL);
	free(pBmp);
	
	return (TRUE);
}
//--------------------------------------------------------
// Загружает выбранный файл на выбранную поверхность из RESOURCE
//--------------------------------------------------------
BOOL LoadBMPinEXE(LPDIRECTDRAWSURFACE7 lpdds, LPSTR lpstrResID)
{
    HBITMAP			hbm;
    HDC				hdcImage= NULL;
    HDC				hdcSurf = NULL;
    BOOL			bRetVal = FALSE;
    HRESULT			ddrval;
    DDSURFACEDESC2	ddsd;
    
    if (!lpdds) return FALSE;
    
    INIT_DIRECTDRAW_STRUCT(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    ddrval = lpdds->GetSurfaceDesc(&ddsd);
    if (FAILED(ddrval))
        goto Exit;
    
    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(lpstrResID), IMAGE_BITMAP, ddsd.dwWidth, ddsd.dwHeight, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        goto Exit;
    
    if (ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC)
    {
        if (!CopyBitmapToYUVSurface(lpdds, hbm))
            goto Exit;        
    }
    else  //Looks like we're just using a standard RGB surface format, let GDI do the work.
    {
        hdcImage = CreateCompatibleDC(NULL);
        SelectObject(hdcImage, hbm);
        
        ddrval = lpdds->GetDC(&hdcSurf);
        if (FAILED(ddrval))
            goto Exit;
        
        if (BitBlt(hdcSurf, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, 0, 0, SRCCOPY) == FALSE)
            goto Exit;
    }
    
    bRetVal = TRUE;
    
Exit:
    if (hdcSurf)
        lpdds->ReleaseDC(hdcSurf);
    if (hdcImage)
        DeleteDC(hdcImage);
    if(hbm)
        DeleteObject(hbm);
    
    return bRetVal;
}
//--------------------------------------------------------
// Копирование HBITMAP на поверхность
//--------------------------------------------------------
BOOL CopyBitmapToYUVSurface(LPDIRECTDRAWSURFACE7 lpDDSurf, HBITMAP hbm)
{
    HDC                 hdcImage;
    HRESULT             ddrval;
    DDSURFACEDESC2      ddsd;
    DWORD               x, y, dwWidth, dwHeight;
    LONG                lPitch;
    LPBYTE              pSurf;
    DWORD               dwBytesInRow;
    COLORREF            color;
    BYTE                R,G,B, Y0,Y1,U,V;
    BOOL                bRet = FALSE;
    
    if (hbm == NULL || lpDDSurf == NULL)
        return FALSE;
    
    hdcImage = CreateCompatibleDC(NULL);
    SelectObject(hdcImage, hbm);
    
    INIT_DIRECTDRAW_STRUCT(ddsd);
    ddrval=lpDDSurf->Lock( NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);
    if (FAILED(ddrval))
        goto CleanUp;
    
    dwWidth=ddsd.dwWidth;
    dwHeight=ddsd.dwHeight;
    lPitch=ddsd.lPitch;
    pSurf=(LPBYTE)ddsd.lpSurface;
    dwBytesInRow=ddsd.dwWidth*2;
    
    for(y=0; y<dwHeight; y++)
    {
        for(x=0; x<dwWidth; x+=2)
        {
            color=GetPixel(hdcImage, x,y);
            R=GetRValue(color);
            G=GetGValue(color);
            B=GetBValue(color);
            Y0= (BYTE)(0.29*R + 0.59*G + 0.14*B);
            U= (BYTE)(128.0 - 0.14*R - 0.29*G + 0.43*B);
            
            color=GetPixel(hdcImage, x+1,y);
            R=GetRValue(color);
            G=GetGValue(color);
            B=GetBValue(color);
            Y1= (BYTE)(0.29*R + 0.57*G + 0.14*B);
            V= (BYTE)(128.0 + 0.36*R - 0.29*G - 0.07*B);
            
            switch (ddsd.ddpfPixelFormat.dwFourCC)
            {
            case MAKEFOURCC('Y','U','Y','2'): 
                *(pSurf++) = Y0;
                *(pSurf++) = U;
                *(pSurf++) = Y1;
                *(pSurf++) = V;
                break;
            case MAKEFOURCC('U','Y','V','Y'): 
                *(pSurf++) = U;
                *(pSurf++) = Y0;
                *(pSurf++) = V;
                *(pSurf++) = Y1;
                break;
            }                       
        }
        pSurf+=(lPitch-dwBytesInRow);
    }
    
    lpDDSurf->Unlock(NULL);     
    
CleanUp:
    if(hdcImage)
        DeleteDC(hdcImage);
    
    return TRUE;
}

//------------------------------------------------------
// Создаёт все рабочии поверхности, требуемые программе
//------------------------------------------------------
BOOL CreateWorkSurfaces()
{
	DDSURFACEDESC2 ddSurfaceDesc;
	DDSCAPS2 ddsCaps;
	HDC hdc;
	HRESULT hRet;

	ZeroMemory(&ddSurfaceDesc,sizeof(ddSurfaceDesc));
	ddSurfaceDesc.dwSize=sizeof(ddSurfaceDesc);
	ddSurfaceDesc.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
	ddSurfaceDesc.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX;
	ddSurfaceDesc.dwBackBufferCount=1;

	hRet=pDD->CreateSurface(&ddSurfaceDesc,&pPrimarySurface,NULL);
	if (FAILED(hRet))
		return (FALSE);
	
	ZeroMemory(&ddsCaps, sizeof(ddsCaps));
	ddsCaps.dwCaps=DDSCAPS_BACKBUFFER;
	hRet=pPrimarySurface->GetAttachedSurface(&ddsCaps,&pBackBuffer);
	if(FAILED(hRet))
		return (FALSE);

	ClearSurface(pBackBuffer);
	if ((pBackBuffer->GetDC(&hdc)) == DD_OK)
	{
		pBackBuffer->ReleaseDC(hdc);
	}
	pPrimarySurface->Flip(NULL,DDFLIP_WAIT);


	hRet=pDD->CreateClipper(NULL,&pClipper,NULL);
	if (FAILED(hRet))
		return (FALSE);

	hRet=pClipper->SetHWnd(NULL,hMainWnd);
	if (FAILED(hRet))
		return (FALSE);

	hRet=pPrimarySurface->SetClipper(pClipper);
	if (FAILED(hRet))
		return (FALSE);

	return (TRUE);
}
//------------------------------------------------------
// Освобождает все объекты, связанные с DirectDraw
//------------------------------------------------------
void RemoveDirectDraw()
{
	if (pDD!=NULL)
	{
		SAFE_RELEASE(pPrimarySurface);
		SAFE_RELEASE(pClipper);
		SAFE_RELEASE(pDDPal);
		SAFE_RELEASE(pMouse);
		SAFE_RELEASE(pPlus);
		SAFE_RELEASE(pMinus);
		SAFE_RELEASE(pRED);	
		SAFE_RELEASE(pGREEN);	
		SAFE_RELEASE(pBLUE);
		
		pDD->Release();
		pDD=NULL;
	}
}
//------------------------------------------------------
// Создаёт все игровые поверхности, требуемые программе    GAMES  Surfaces!!!!!
//------------------------------------------------------
BOOL CreateGameSurfaces()
{
	pRED=CreateSurface(points[1].w,points[1].h);
	if (!pRED)
		return (FALSE);

	pGREEN=CreateSurface(points[1].w,points[1].h);
	if (!pGREEN)
		return (FALSE);

	pBLUE=CreateSurface(points[1].w,points[1].h);
	if (!pBLUE)
		return (FALSE);

	pMouse=CreateSurface(mouse.w,mouse.h);
	if (!pMouse)
		return (FALSE);

	pPlus=CreateSurface(plus[0].w,plus[0].h);
	if (!pPlus)
		return (FALSE);

	pMinus=CreateSurface(minus[0].w,minus[0].h);
	if (!pMinus)
		return (FALSE);

	ddColorKey.dwColorSpaceLowValue=TRASPARENT_COLOR;
	ddColorKey.dwColorSpaceHighValue=TRASPARENT_COLOR;

	pMouse->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);

	return (TRUE);
}
//--------------------------------------------------------
// Очищает поверхности и заполняет их правильными данными
//--------------------------------------------------------
BOOL PrepareSurfaces()
{
	pDDPal=CreateDirectDrawPalette(pDD);
	if (pDDPal==NULL)
		return (FALSE);

	pPrimarySurface->SetPalette(pDDPal);

	if (!LoadBMPinEXE(pMouse,		MAKEINTRESOURCE(IDB_CURSOR)))	return (FALSE);
	if (!LoadBMPinEXE(pPlus,		MAKEINTRESOURCE(IDB_PLUS)))		return (FALSE);
	if (!LoadBMPinEXE(pMinus,		MAKEINTRESOURCE(IDB_MINUS)))	return (FALSE);
	if (!LoadBMPinEXE(pRED,			MAKEINTRESOURCE(IDB_RED)))		return (FALSE);
	if (!LoadBMPinEXE(pGREEN,		MAKEINTRESOURCE(IDB_GREEN)))	return (FALSE);
	if (!LoadBMPinEXE(pBLUE,		MAKEINTRESOURCE(IDB_BLUE)))		return (FALSE);
	return (TRUE);
}
//--------------------------------------------------------
// Проверяем поверхности перед выводом очередного кадра
//--------------------------------------------------------
void PrepareFrame()
{
	if (pPrimarySurface->IsLost())
	{
		pPrimarySurface->Restore();
		pBackBuffer->Restore();

		if (pRED->IsLost())
		{
			pRED->Restore();
			LoadBMPinEXE(pRED,MAKEINTRESOURCE(IDB_RED));
		}

		if (pGREEN->IsLost())
		{
			pGREEN->Restore();
			LoadBMPinEXE(pGREEN,MAKEINTRESOURCE(IDB_GREEN));
		}

		if (pBLUE->IsLost())
		{
			pBLUE->Restore();
			LoadBMPinEXE(pBLUE,MAKEINTRESOURCE(IDB_BLUE));
		}

		if (pMouse->IsLost())
		{
			pMouse->Restore();
			LoadBMPinEXE(pMouse,MAKEINTRESOURCE(IDB_CURSOR));
		}

		if (pPlus->IsLost())
		{
			pPlus->Restore();
			LoadBMPinEXE(pPlus,MAKEINTRESOURCE(IDB_PLUS));
		}
		
		if (pMinus->IsLost())
		{
			pMinus->Restore();
			LoadBMPinEXE(pMinus,MAKEINTRESOURCE(IDB_MINUS));
		}		
	}
}


//-------------------------------------- GAMES SCRIPT ---------------------------------------------------------
//-------------------------------------- GAMES SCRIPT ---------------------------------------------------------
//-------------------------------------- GAMES SCRIPT ---------------------------------------------------------.
int rndInt( int low, int high )
{
    int range = high - low;
    int num = rand() % range;
    return( num + low );
}

double rndDouble( double low, double high )
{
    double range = high - low;
    double num = range * (double)rand()/(double)RAND_MAX;
    return( num + low );
}

void InitSin_CosTables(void)
{
	int angle;
    for(angle = -1;angle < 361;angle++)
    {
		SinTable[angle]=sin((double)angle* 3.14 / 180);
		CosTable[angle]=cos((double)angle* 3.14 / 180);
    }
} 



//--------------------------------------------------------
// Расчитывает новое значение частоты смены кадров
//--------------------------------------------------------
void UpdateFPS()
{
	static const long lPeriod=10;
	static long lFrameNumber=0;
	static DWORD dwPrevTime;
	static DWORD dwCurTime;
	static int nFps=0;

	lFrameNumber+=1;
	if (lFrameNumber==lPeriod)
	{
		dwPrevTime=dwCurTime;
		dwCurTime=timeGetTime();

		double fSeconds=double(dwCurTime-dwPrevTime)/(double)1000;
		nFps=(int)((double)lFrameNumber/fSeconds);

		lFrameNumber=0;

		wsprintf(fps.name,"%d fps",nFps);
	}
	OutStr(fps.x,fps.y,fps.name);
}

void OutStr(int x, int y, LPCTSTR text)
{
    HDC		hdc;
    RECT	rc;
    SIZE	size;

    if (pBackBuffer->GetDC(&hdc) == DD_OK)
    {
        SetBkColor(hdc, RGB(0, 0, 00));
        SetTextColor(hdc, RGB(font_color.red, font_color.green,font_color.blue));
        GetClientRect(hMainWnd, &rc);
        GetTextExtentPoint(hdc, text, lstrlen(text), &size);
        TextOut(hdc, x, y, text, lstrlen(text));

        pBackBuffer->ReleaseDC(hdc);
    }
}
// ----------------------------------- STAR (END) -----------------------------------------------
//
// ----------------------------------- DRAW FRAME (BEGIN) ---------------------------------------
void DrawFrame()//отрисовка кадра
{
	Sleep(Delay);

	PrepareFrame();
	ClearSurface(pBackBuffer);

	DrawHole();
	DrawControl();
	DrawObjectSRC(pMouse,&mouse);

	UpdateFPS();
	pPrimarySurface->Flip(NULL,DDFLIP_WAIT);
}

void Press(bool sp, bool f1,bool f2)
{
	if (sp) 
	{

	}

	if (f1) 
	{

	}
	if (f2) 
	{

	}
}

// ----------------------------------- OBJECT (BEGIN) ----------------------------------------
void InitObject(OBJECT * obj,int x, int y, int w,int h)
{
	obj->x=		x;
	obj->y=		y;
	obj->h=		h;
	obj->w=		w;
	obj->cx=	w/2;
	obj->cy=	h/2;
	obj->visible=false;
	SetRect(&obj->pic,0,0,w,h);
}

void DrawObject(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj)
{
	pBackBuffer->BltFast(obj->x,obj->y,lpdds,&obj->pic,DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
}

void DrawObjectSRC(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj)
{
	pBackBuffer->BltFast(obj->x,obj->y,lpdds,&obj->pic,DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
}

bool Crossing(OBJECT *obj, int x, int y)//пересечение точки с прямоугольником
{	
	if ((x>obj->x) && (x< obj->x+obj->w) && (y>obj->y) && (y< obj->y+obj->h )) return true; else return false;
}
//--------------------------------------------------------------
//отрисовка интерфейса
//--------------------------------------------------------------
void DrawControl()
{
	int i;
	for(i=0;i<MAX_CONTROL;i++)	
	{
		font_color.blue=000;
		font_color.red=100;
		font_color.green=100;

		OutStr(minus[i].x+20,minus[i].y-15,minus[i].name);
		DrawObject(pMinus,&minus[i]);

		font_color.blue=200;
		font_color.red=200;
		font_color.green=200;

			switch(i)
			{
				case CONTROL_MAX_POINT:	
					wsprintf(buffer,"%d",MaxPoint);
					OutStr(minus[i].x+21,minus[i].y,buffer);	
					break;
				case CONTROL_CURRENT:
					wsprintf(buffer,"%d",CurrentPoint);
					OutStr(minus[i].x+21,minus[i].y,buffer);	
					break;
				case CONTROL_LOOP:	
					wsprintf(buffer,"%d",points[CurrentPoint].loop);					
					OutStr(minus[i].x+21,minus[i].y,buffer);
					break;	
				case CONTROL_A:	
					wsprintf(buffer,"%d",points[CurrentPoint].a);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;				
				case CONTROL_B:		
					wsprintf(buffer,"%d",points[CurrentPoint].b);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;
				case CONTROL_A_K:
					wsprintf(buffer,"%d",points[CurrentPoint].ka);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;
				case CONTROL_B_K:	
					wsprintf(buffer,"%d",points[CurrentPoint].kb);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;
				case CONTROL_HI_R:		
					wsprintf(buffer,"%d",points[CurrentPoint].radius_hi);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;
				case CONTROL_LOW_R:		
					wsprintf(buffer,"%d",points[CurrentPoint].radius_low);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;	
				case CONTROL_ROUND_MAX:	
					wsprintf(buffer,"%d",points[CurrentPoint].round_max);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;						
				case CONTROL_ROUND_MIN:	
					wsprintf(buffer,"%d",points[CurrentPoint].round_min);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;	
				case CONTROL_COLOR:		
					wsprintf(buffer,"%d",points[CurrentPoint].color);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;	
				case CONTROL_OSX:	
					wsprintf(buffer,"%d",points[1].osx);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;	
				case CONTROL_OSY:		
					wsprintf(buffer,"%d",points[1].osy);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;
				case CONTROL_TANX:	
					wsprintf(buffer,"%d",points[1].tanx);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;	
				case CONTROL_TANY:		
					wsprintf(buffer,"%d",points[1].tany);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;
				case CONTROL_VAR:		
					wsprintf(buffer,"%d",var);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;
				case CONTROL_FPS:		
					wsprintf(buffer,"%d",Delay);					
					OutStr(minus[i].x+21,minus[i].y,buffer);					
					break;
			}
	}
	for(i=0;i<MAX_CONTROL;i++)	DrawObject(pPlus,&plus[i]);

	font_color.blue=100;
	font_color.red=100;
	font_color.green=100;

	OutStr(MAX_WIDTH-100,MAX_HEIGHT-20,"Metlinskiy R.A.");
}
//--------------------------------------------------------------
//обраюотка мышинных событий
//--------------------------------------------------------------
void MoveMouse(int x,int y, int scroll,bool lb, bool rb,bool cb)
{
	if (lb) 
	{
		for(int i=0;i<MAX_CONTROL;i++)
		{
			if(Crossing(&plus[i],x,y))
			{
				switch(i)
				{
					case CONTROL_MAX_POINT:	if(MaxPoint<4)MaxPoint++;	break;
					case CONTROL_CURRENT:	if(CurrentPoint<4)CurrentPoint++;break;
					case CONTROL_LOOP:		points[CurrentPoint].loop++;break;	
					case CONTROL_A:			points[CurrentPoint].a++;	break;				
					case CONTROL_B:			points[CurrentPoint].b++;	break;		
					case CONTROL_A_K:		points[CurrentPoint].ka++;	break;			
					case CONTROL_B_K:		points[CurrentPoint].kb++;	break;	
					case CONTROL_HI_R:		points[CurrentPoint].radius_hi++;break;		
					case CONTROL_LOW_R:		points[CurrentPoint].radius_low++;break;		
					case CONTROL_ROUND_MAX:	if(points[CurrentPoint].round_max<360)points[CurrentPoint].round_max++;break;		
					case CONTROL_ROUND_MIN:	if(points[CurrentPoint].round_max<360)points[CurrentPoint].round_min++;break;
					case CONTROL_COLOR:		if(points[CurrentPoint].color<3)points[CurrentPoint].color++;break;	
					case CONTROL_OSX:		points[CurrentPoint].osx+=0.001f;	break;	
					case CONTROL_OSY:		points[CurrentPoint].osy+=0.001f;	break;
					case CONTROL_TANX:		points[CurrentPoint].tanx+=0.001f;	break;	
					case CONTROL_TANY:		points[CurrentPoint].tany+=0.001f;	break;	
					case CONTROL_VAR:		if(var<20)var++;	 else var=0;		break;
					case CONTROL_FPS:		if(Delay<50)Delay++;	break;							
				}
			}
			if(Crossing(&minus[i],x,y))
			{
				switch(i)
				{
					case CONTROL_MAX_POINT:	if(MaxPoint>0)MaxPoint--;	break;
					case CONTROL_CURRENT:	if(CurrentPoint>0)CurrentPoint--;break;
					case CONTROL_LOOP:		points[CurrentPoint].loop--;break;	
					case CONTROL_A:			points[CurrentPoint].a--;break;					
					case CONTROL_B:			points[CurrentPoint].b--;break;			
					case CONTROL_A_K:		points[CurrentPoint].ka--;	break;			
					case CONTROL_B_K:		if(points[CurrentPoint].kb>0)points[CurrentPoint].kb--;	break;	
					case CONTROL_HI_R:		points[CurrentPoint].radius_hi--;break;		
					case CONTROL_LOW_R:		points[CurrentPoint].radius_low--;break;		
					case CONTROL_ROUND_MAX:	if(points[CurrentPoint].round_max>1) points[CurrentPoint].round_max--;break;		
					case CONTROL_ROUND_MIN:	if(points[CurrentPoint].round_min>0) points[CurrentPoint].round_min--;break;
					case CONTROL_COLOR:		if(points[CurrentPoint].color>0)points[CurrentPoint].color--;	break;
					case CONTROL_OSX:		points[CurrentPoint].osx-=0.001f;	break;	
					case CONTROL_OSY:		points[CurrentPoint].osy-=0.001f;	break;	
					case CONTROL_TANX:		points[CurrentPoint].tanx-=0.001f;	break;	
					case CONTROL_TANY:		points[CurrentPoint].tany-=0.001f;	break;	
					case CONTROL_VAR:		if(var>0)var--;		 else var=20; break;	
					case CONTROL_FPS:		if(Delay>0)Delay--;	break;	
				}
			}
		}

	}
	mouse.x=x;
	mouse.y=y;
}


void DrawHole()
{
	fz1+=	points[1].osy;
	fz2+=	points[1].osx;
	tan_x+= points[1].tanx;
	tan_y+= points[1].tany;

	for(index=1;index<MaxPoint+1;index++)
	{
		for(int i=0;i<points[index].loop;i++)
		{
			switch(var)
			{
case 1:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*cos(fz1);//вращение
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a]*sin(fz2);
break;
case 2:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*cos(fz1);
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a]*cos(points[index].a);
break;
case 3:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*cos(points[index].b);
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a];
break;
case 4:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*tan(points[index].b);
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a]*cos(points[index].b);
break;
case 5:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*tan(points[index].b*cos(fz1));
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a]*cos(points[index].b*sin(fz2));
break;
case 6:
			points[index].x=(int)(screen.cx + points[index].b * SinTable[points[index].a])*cos(points[index].b)*cos(fz1);
			points[index].y=(int)(screen.cy - points[index].b * CosTable[points[index].a])*tan(points[index].b)*sin(fz2);
break;
case 7:
			points[index].x=(int)(screen.cx + points[index].b * SinTable[points[index].a])*atan(points[index].b)*cos(fz1);
			points[index].y=(int)(screen.cy - points[index].b * CosTable[points[index].a])*tan(points[index].b)*sin(fz2);
break;
case 8:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*atan(points[index].a)*cos(fz1);
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a]*tan(points[index].a)*sin(fz2);
break;
case 9:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*tan(SinTable[points[index].a]);
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a];
break;
case 10:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*(cos(SinTable[points[index].a])/sin(SinTable[points[index].a]));
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a];
break;
case 11:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a];
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a]*atan(SinTable[points[index].a]);
break;
case 12:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a];
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a]*asin(SinTable[points[index].a]);
break;
case 13:
			points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]*tan(fz1);
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a]*atan(fz2);
break;
case 14:
			points[index].x=(int)screen.cx + points[index].b * (SinTable[points[index].a]*log(points[index].a))/16;
			points[index].y=(int)screen.cy - points[index].b * (CosTable[points[index].a]*sqrt(points[index].a))/16;
break;
case 15:
			points[index].x=(int)screen.cx + points[index].b / (SinTable[points[index].a])/2;
			points[index].y=(int)screen.cy - points[index].b / (CosTable[points[index].a])/2;
break;
case 16:
			points[index].x=(int)screen.cx + points[index].b * cos(points[index].a)/sin(points[index].a);
			points[index].y=(int)screen.cy - points[index].b * sin(points[index].a);
break;
case 17:
			points[index].x=(int)screen.cx + points[index].b * tan(points[index].a);
			points[index].y=(int)screen.cy - points[index].b * sin(points[index].a);
break;
case 18:
			points[index].x=(int)screen.cx + points[index].b / cos(points[index].a)/sin(points[index].a);
			points[index].y=(int)screen.cy - points[index].b / sin(points[index].a);
break;
case 19:
			points[index].x=(int)screen.cx + points[index].b * sin(points[index].a);
			points[index].y=(int)screen.cy - atan(points[index].b) * tan(points[index].a)/sin(points[index].a);
break;
case 20:
			points[index].x=(int)screen.cx + tan(points[index].b) * SinTable[points[index].a];
			points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a];
break;
default:
				points[index].x=(int)screen.cx + points[index].b * SinTable[points[index].a]; //базовая
				points[index].y=(int)screen.cy - points[index].b * CosTable[points[index].a];
			}

			points[index].a-=points[index].ka;
			points[index].b-=points[index].kb;

			if(points[index].a<points[index].round_min) points[index].a=points[index].round_max;
			if(points[index].a>points[index].round_max) points[index].a=points[index].round_min;
			if(points[index].b<points[index].radius_low) points[index].b=points[index].radius_hi;

			switch(points[index].color)
			{
				case COLOR_RED:
					DrawObject(pRED,&points[index]);
					break;
				case COLOR_GREEN:
					DrawObject(pGREEN,&points[index]);
					break;

				case COLOR_BLUE:
					DrawObject(pBLUE,&points[index]);
					break;
			}
		}
	}
}

void InitEditor()
{
	int id;
	InitSin_CosTables();
	InitObject(&mouse,MAX_WIDTH/2,MAX_HEIGHT/2,24,28);
	InitObject(&screen,0,0,MAX_WIDTH,MAX_HEIGHT);
	InitObject(&fps,screen.cx-25,5,50,30);

	for(id=0;id<MAX_CONTROL	;id++) InitObject(&plus[id],100,id*30+30,20,15);
	for(id=0;id<MAX_CONTROL	;id++) InitObject(&minus[id],5,id*30+30,20,15);

	fz1=0;
	fz2=5;
	tan_x=0.001f;
	tan_y=0.0f;
	var=0;

id=0;
	strcpy(minus[id].name,"Max Point");
	strcpy(plus[id].name,"Max Point");
id++;
	strcpy(minus[id].name,"Current");
	strcpy(plus[id].name,"Current");
id++;
	strcpy(minus[id].name,"Loop");
	strcpy(plus[id].name,"Loop");
id++;
	strcpy(minus[id].name,"Angle A");
	strcpy(plus[id].name,"Angle A");
id++;
	strcpy(minus[id].name,"Current B");
	strcpy(plus[id].name,"Current B");
id++;
	strcpy(minus[id].name,"Inc A ");
	strcpy(plus[id].name,"Inc A");
id++;
	strcpy(minus[id].name,"Inc B");
	strcpy(plus[id].name,"Inc B");
id++;
	strcpy(minus[id].name,"Hi r");
	strcpy(plus[id].name,"Hi r");
id++;
	strcpy(minus[id].name,"Low r");
	strcpy(plus[id].name,"Low r");
id++;
	strcpy(minus[id].name,"Round MAX");
	strcpy(plus[id].name,"Round MAX");
id++;
	strcpy(minus[id].name,"Round MIN");
	strcpy(plus[id].name,"Round MIN");
id++;
	strcpy(minus[id].name,"Color");
	strcpy(plus[id].name,"Color");
id++;
	strcpy(minus[id].name,"Rotation X");
	strcpy(plus[id].name,"Rotation X");
id++;
	strcpy(minus[id].name,"Rotation Y");
	strcpy(plus[id].name,"Rotation Y");
id++;
	strcpy(minus[id].name,"tan(X)");
	strcpy(plus[id].name,"tan(X)");
id++;
	strcpy(minus[id].name,"tan(Y)");
	strcpy(plus[id].name,"tan(Y)");
id++;
	strcpy(minus[id].name,"Variation");
	strcpy(plus[id].name,"Variation");
id++;
	strcpy(minus[id].name,"Delay");
	strcpy(plus[id].name,"Delay");

	id=1;
	InitObject(&points[id],0,0,4,4);
	points[id].radius_low=-252;
	points[id].radius_hi=106;
	points[id].a=0;
	points[id].b=0;
	points[id].ka=1;
	points[id].kb=3;
	points[id].loop=400;
	points[id].speed=1;
	points[id].round_max=360;
	points[id].round_min=0;
	points[id].color=COLOR_RED;
	points[id].osx=0.0f;
	points[id].osy=0.0f;
	points[id].tanx=0.0f;
	points[id].tany=0.0f;

	id=2;
	InitObject(&points[id],0,0,4,4);
	points[id].radius_low=-118;
	points[id].radius_hi=240;
	points[id].a=45;
	points[id].b=0;
	points[id].ka=1;
	points[id].kb=3;
	points[id].loop=500;
	points[id].speed=1;
	points[id].round_max=360;
	points[id].round_min=0;
	points[id].color=COLOR_BLUE;
	points[id].osx=0.0f;
	points[id].osy=0.0f;
	points[id].tanx=0.0f;
	points[id].tany=0.0f;

	id=3;
	InitObject(&points[id],0,0,4,4);
	points[id].radius_low=274;
	points[id].radius_hi=333;
	points[id].a=90;
	points[id].b=0;
	points[id].ka=-1;
	points[id].kb=1;
	points[id].loop=450;
	points[id].speed=1;
	points[id].round_max=360;
	points[id].round_min=0;
	points[id].color=COLOR_GREEN;
	points[id].osx=0.0f;
	points[id].osy=0.0f;
	points[id].tanx=0.0f;
	points[id].tany=0.0f;

	id=4;
	InitObject(&points[id],0,0,4,4);
	points[id].radius_low=271;
	points[id].radius_hi=330;
	points[id].a=135;
	points[id].b=0;
	points[id].ka=-1;
	points[id].kb=1;
	points[id].loop=400;
	points[id].speed=1;
	points[id].round_max=360;
	points[id].round_min=0;
	points[id].color=COLOR_GREEN;
	points[id].osx=0.0f;
	points[id].osy=0.0f;
	points[id].tanx=0.0f;
	points[id].tany=0.0f;
}

