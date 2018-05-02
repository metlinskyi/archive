//---------------------------------------------------------
//	Файл:		DRAW.CPP
//	Описание:	Вывод графики средствами DirectDraw
//---------------------------------------------------------
#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "resource.h"
#include "object.h"

LPDIRECTDRAW7			pDD=NULL;
LPDIRECTDRAWSURFACE7	pPrimarySurface=NULL;
LPDIRECTDRAWSURFACE7	pBackBuffer=NULL;
LPDIRECTDRAWPALETTE		pDDPal=NULL;
LPDIRECTDRAWCLIPPER		pClipper=NULL;			
LPDIRECTDRAWSURFACE7	pShip=NULL;
LPDIRECTDRAWSURFACE7	pStarD=NULL;
LPDIRECTDRAWSURFACE7	pStarS=NULL;
LPDIRECTDRAWSURFACE7	pStarM=NULL;
LPDIRECTDRAWSURFACE7	pStarL=NULL;
LPDIRECTDRAWSURFACE7	pBullet=NULL;
LPDIRECTDRAWSURFACE7	pEnemiS=NULL;
LPDIRECTDRAWSURFACE7	pEnemiM=NULL;
LPDIRECTDRAWSURFACE7	pEnemiL=NULL;
LPDIRECTDRAWSURFACE7	pBossN=NULL;
LPDIRECTDRAWSURFACE7	pBossE=NULL;
LPDIRECTDRAWSURFACE7	pBossM=NULL;
LPDIRECTDRAWSURFACE7	pBossR=NULL;

LPDIRECTDRAWSURFACE7	pPlanetS=NULL;
LPDIRECTDRAWSURFACE7	pPlanetM=NULL;
LPDIRECTDRAWSURFACE7	pPlanetL=NULL;
LPDIRECTDRAWSURFACE7	pExplM=NULL;
LPDIRECTDRAWSURFACE7	pLogo=NULL;
LPDIRECTDRAWSURFACE7	pBrandName=NULL;
LPDIRECTDRAWSURFACE7	pFont=NULL;
LPDIRECTDRAWSURFACE7	pFPS=NULL;
LPDIRECTDRAWSURFACE7	pAccount=NULL;
LPDIRECTDRAWSURFACE7	pMouse=NULL;
DDCOLORKEY				ddColorKey;
PALETTEENTRY            pal[256];

SurfaceLocation			fSurfLoc=BOTH;


LPDIRECTDRAWSURFACE7 CreateSurface(int x, int y);
BOOL	CreateWorkSurfaces();
BOOL	CreateGameSurfaces();
BOOL	PrepareSurfaces();
BOOL	LoadBMP(LPDIRECTDRAWSURFACE7 pSurface, char* filename);
BOOL	LoadBMPinEXE(LPDIRECTDRAWSURFACE7 lpdds, LPSTR lpstrResID);
BOOL	CopyBitmapToYUVSurface(LPDIRECTDRAWSURFACE7 lpDDSurf, HBITMAP hbm);

HDC		hdc;
RECT	rc;

bool	InitShip(OBJECT * obj , BYTE tp);

void	OutStr(int x, int y, LPCTSTR text);
void	UpdateFPS();
void	UpdateAccount();
void	DisplayFrameRate( void );

void	InitObject(OBJECT * obj,int x, int y, int w,int h,int speed);
void	SetODZ(OBJECT *obj);
inline bool	Crossing(OBJECT *obj, int x, int y);
inline bool	Crossing(OBJECT *obj1, OBJECT *obj2);

void	DrawObjectNO(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj);
void	DrawObjectSRC(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj);
void	DrawObjectCutSRC(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj);

void	InitStar();
void 	PrepareStar();
void	DrawStar(LPDIRECTDRAWSURFACE7 lpdds, OBJECT * obj);
	
void	PrepareLogo();
void	HoleInit();
bool	BlackScreen();
void	AnimMenu(BYTE direct,BYTE &out);


void	InitBullet();
void    NewBullet(BYTE power);
void	NewBullet(OBJECT * obj,OBJECT * sh);
void	DeletBullet(OBJECT * obj ,int ind);
bool	NextBullet();
bool	PrevBullet();
void	AddObjToListBullet(OBJECT * obj);
void	EndBullet();
void	PointText(POINT_TEXT p[32]);
void	NewBoom(OBJECT *obj);

void	AddObjToList(OBJECT * obj);
void	AddObjToList(LIST *lp,OBJECT * obj);
bool	Next();
bool	Prev();
bool	Next(LPLIST);
bool	Prev(LPLIST);
void	Begin();
void	End();
void	Insert(OBJECT obj);
void	Delete(int);
BYTE	bscreen=0;


OBJECT	ship;
OBJECT	star_d[MAX_STAR_DEEP];
OBJECT	star_s[MAX_STAR_S];
OBJECT	star_m[MAX_STAR_M];
OBJECT	star_l[MAX_STAR_L];
OBJECT  enemi_s[MAX_ENEMI_S];
OBJECT  enemi_m[MAX_ENEMI_M];
OBJECT  enemi_l[MAX_ENEMI_L];
OBJECT  boss_n;
OBJECT  boss_e;
OBJECT  boss_m;
OBJECT  boss_r;
OBJECT  planet;
OBJECT  expl_m[32];
OBJECT	bullet[128];
OBJECT	logo;
OBJECT	bname[8];
OBJECT	cursor;
OBJECT	fps;
OBJECT	screen;
OBJECT	comet;
OBJECT	weapon_blaster;

POINT_TEXT	point[32];

RECT	rPic;

SIZE	size;
long	account=0;
	
char	buffer[256];

int		expl_id=0;
int		index=0;
int		reload=0;
int		g=0;
int		id=0;

BYTE	fasa=1;

DWORD                   dwFrameCount;
DWORD                   dwFrameTime;
DWORD                   dwFrames;
DWORD                   dwFramesLast;

bool	l=false;
bool	r=false;
bool	NotShoot=true;
bool	GameOver=false;
bool	MoveShipLock=false;
bool	MouseLock=false;
bool	bLogo=true;
bool	bMenu=true;
bool	bKliss=false;
bool	bExit=false;

int		delay=GAME_SLEEP;

float	inc;
float	dec;
int		k1;
int		k2;
int		r_hi;
int		r_low;
int		loop;
int		object_count;


BYTE	stream=0;
LEVEL	level;


LIST*	lpListObject;	
LIST*	lpHeadObject;
LIST*	lpList;	
LIST*	lpHead;
LIST*	lpListBullet;	
LIST*	lpHeadBullet;	
LIST*	lpTmp;	

bool	bDelObj=false;

double	SinTable[360];
double	CosTable[360];
//----------------------------------------------------
// Функция вызываемая в случае ошибки
//----------------------------------------------------
void ErrorHandle(HWND hwnd, LPCTSTR szError)
{
	char szErrorMessage[255];

	RemoveDirectDraw();
	ShowWindow(hwnd,SW_HIDE);
	wsprintf(szErrorMessage,"Фатальный глюк .\n%s.",szError);	
	MessageBox(hwnd, szErrorMessage, AppName, MB_OK|MB_ICONWARNING);
	DestroyWindow(hwnd);   
}

//------------------------------------------------------
// Инициализирует все объекты, связанные с DirectDraw
//------------------------------------------------------
BOOL	InitDirectDraw ()
{
	HRESULT hRet;
	
	hRet=DirectDrawCreateEx(NULL, (VOID**)&pDD, IID_IDirectDraw7, NULL);
	if (FAILED(hRet))
	{
		ErrorHandle(hMainWnd,"нехочет DirectDraw  создаватся!!!");
		return (FALSE);
	}

	hRet=pDD->SetCooperativeLevel(hMainWnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);
	if (FAILED(hRet))
	{
		ErrorHandle(hMainWnd,"в установке CooperativeLevel  - а фиг знает что это???");
		return (FALSE);
	}

	hRet=pDD->SetDisplayMode(MAX_WIDTH, MAX_HEIGHT, COLOR_DEPTH,0,0);
	if (FAILED(hRet))
	{
		ErrorHandle(hMainWnd,"Ваш монитор нам не подходит!");				
		return (FALSE);
	}

	if (!CreateWorkSurfaces())
	{
		ErrorHandle(hMainWnd,"Рабочие поверхности несозданы!");				
		return (FALSE);
	}

	if (!CreateGameSurfaces())
	{
		ErrorHandle(hMainWnd,"Игровые поверхности несозданы!");				
		return (FALSE);
	}

	if (!PrepareSurfaces())
	{
		ErrorHandle(hMainWnd,"Вы удалили файл pal.dat, верните его на место!");				
		return (FALSE);
	}

	if (pDDPal->GetEntries(0, 0, 256, pal) != DD_OK)
		return(FALSE);

	if (!InitShip(&ship,SHIP_TYPE_DEF))
	{
		ErrorHandle(hMainWnd,"Ошибка в инициализации pShip!");				
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
    
    if (!lpdds)
        return FALSE;
    
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
//-------------------------------------- GAMES SCRIPT ---------------------------------------------------------
//-------------------------------------- GAMES SCRIPT ---------------------------------------------------------
//-------------------------------------- GAMES SCRIPT ---------------------------------------------------------.

//------------------------------------------------------
// Освобождает все объекты, связанные с DirectDraw
//------------------------------------------------------
void RemoveDirectDraw()
{
	if (pDD!=NULL)
	{
		SAFE_RELEASE(pPrimarySurface);
		SAFE_RELEASE(pDDPal);
		SAFE_RELEASE(pStarD);
		SAFE_RELEASE(pStarS);
		SAFE_RELEASE(pStarM);	
		SAFE_RELEASE(pStarL);
		SAFE_RELEASE(pBullet);
		SAFE_RELEASE(pEnemiS);
		SAFE_RELEASE(pEnemiM);	
		SAFE_RELEASE(pEnemiL);
		SAFE_RELEASE(pPlanetS);
		SAFE_RELEASE(pPlanetM);	
		SAFE_RELEASE(pPlanetL);
		SAFE_RELEASE(pLogo);
		SAFE_RELEASE(pFPS);
		SAFE_RELEASE(pClipper);
		SAFE_RELEASE(pAccount);
		SAFE_RELEASE(pBrandName);
		SAFE_RELEASE(pShip);

		pDD->Release();
		pDD=NULL;
	}
}
//------------------------------------------------------
// Создаёт все игровые поверхности, требуемые программе    GAMES  Surfaces!!!!!
//------------------------------------------------------
BOOL CreateGameSurfaces()
{
	pLogo=CreateSurface(565,88);
	if (!pLogo)
		return (FALSE);

	pBrandName=CreateSurface(bname[7].w,bname[7].h);
	if (!pBrandName)
		return (FALSE);

	pMouse=CreateSurface(28,32);
	if (!pMouse)
		return (FALSE);

	pEnemiL=CreateSurface(enemi_l[1].w,enemi_l[1].h);
	if (!pEnemiL)
		return (FALSE);

	pEnemiM=CreateSurface(enemi_m[1].w,enemi_m[1].h);
	if (!pEnemiM)
		return (FALSE);

	pEnemiS=CreateSurface(enemi_s[1].w,enemi_s[1].h);
	if (!pEnemiS)
		return (FALSE);

	pStarD=CreateSurface(star_d[1].w,star_d[1].h);
	if (!pStarD)
		return (FALSE);

	pStarS=CreateSurface(star_s[1].w,star_s[1].h);
	if (!pStarS)
		return (FALSE);

	pStarM=CreateSurface(star_m[1].w,star_m[1].h);
	if (!pStarM)
		return (FALSE);

	pStarL=CreateSurface(star_l[1].w,star_l[1].h);
	if (!pStarL)
		return (FALSE);

	pBullet=CreateSurface(bullet[1].w,bullet[1].h);
	if (!pBullet)
		return (FALSE);

	pExplM=CreateSurface(expl_m[1].w,expl_m[1].h);
	if (!pExplM)
		return (FALSE);

	pPlanetL=CreateSurface(160,160);
	if (!pPlanetL)
		return (FALSE);

	pAccount=CreateSurface(ACCOUNT_WIDTH,ACCOUNT_HEIGHT);
	if (!pAccount)
		return (FALSE);

	pFPS=CreateSurface(fps.w,fps.h);
	if (!pFPS)
		return (FALSE);

	ddColorKey.dwColorSpaceLowValue=TRASPARENT_COLOR;
	ddColorKey.dwColorSpaceHighValue=TRASPARENT_COLOR;

	pBrandName->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
	pAccount->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
	pFPS->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
	pExplM->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
	pEnemiL->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
	pEnemiM->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
	pEnemiS->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
	pPlanetL->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
	pLogo->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);
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

	if (!LoadBMPinEXE(pMouse,		MAKEINTRESOURCE(IDB_CURSOR)))		return (FALSE);
	if (!LoadBMPinEXE(pLogo,		MAKEINTRESOURCE(IDB_LOGO)))			return (FALSE);
	if (!LoadBMPinEXE(pBrandName,	MAKEINTRESOURCE(IDB_LOGO_ALL)))		return (FALSE);
	if (!LoadBMPinEXE(pExplM,		MAKEINTRESOURCE(IDB_EXPL_M)))		return (FALSE);
	if (!LoadBMPinEXE(pEnemiL,		MAKEINTRESOURCE(IDB_ENEMI_L)))		return (FALSE);
	if (!LoadBMPinEXE(pEnemiM,		MAKEINTRESOURCE(IDB_ENEMI_M)))		return (FALSE);
	if (!LoadBMPinEXE(pEnemiS,		MAKEINTRESOURCE(IDB_ENEMI_S)))		return (FALSE);
	if (!LoadBMPinEXE(pBullet,		MAKEINTRESOURCE(IDB_BULLET_B)))		return (FALSE);
	if (!LoadBMPinEXE(pStarD,		MAKEINTRESOURCE(IDB_STAR_DEEP)))	return (FALSE);
	if (!LoadBMPinEXE(pStarL,		MAKEINTRESOURCE(IDB_STAR_L)))		return (FALSE);
	if (!LoadBMPinEXE(pStarM,		MAKEINTRESOURCE(IDB_STAR_M)))		return (FALSE);
	if (!LoadBMPinEXE(pStarS,		MAKEINTRESOURCE(IDB_STAR_S)))		return (FALSE);
	if (!LoadBMPinEXE(pPlanetL,		MAKEINTRESOURCE(IDB_PLANET_L)))		return (FALSE);

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

		PrepareStar();

		if (pExplM->IsLost())
		{
			pExplM->Restore();
			LoadBMPinEXE(pExplM,MAKEINTRESOURCE(IDB_EXPL_M));
		}
		
		if (pBullet->IsLost())
		{
			pBullet->Restore();
			LoadBMPinEXE(pBullet,MAKEINTRESOURCE(IDB_BULLET_B));
		}

		if (pEnemiL->IsLost())
		{
			pEnemiL->Restore();
			LoadBMPinEXE(pEnemiL,MAKEINTRESOURCE(IDB_ENEMI_L));
		}

		if (pEnemiM->IsLost())
		{
			pEnemiM->Restore();
			LoadBMPinEXE(pEnemiM,MAKEINTRESOURCE(IDB_ENEMI_M));
		}

		if (pEnemiS->IsLost())
		{
			pEnemiS->Restore();
			LoadBMPinEXE(pEnemiS,MAKEINTRESOURCE(IDB_ENEMI_S));
		}

		if (pPlanetL->IsLost())
		{
			pPlanetL->Restore();
			LoadBMPinEXE(pPlanetL,MAKEINTRESOURCE(IDB_PLANET_L));
		}

		if (pShip->IsLost())
		{
			switch (ship.a)
			{
				case SHIP_TYPE_M:
					pShip->Restore();
					LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_M));
					break;

				case SHIP_TYPE_H:
					pShip->Restore();
					LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_H));
					break;

				case SHIP_TYPE_G:
					pShip->Restore();
					LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_G));
					break;

				case SHIP_TYPE_F:
					pShip->Restore();
					LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_P));
					break;

				case SHIP_TYPE_P:
					pShip->Restore();
					LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_K));
					break;
			}
		}
	}
}


void PrepareLogo()
{
	if (pPrimarySurface->IsLost())
	{
		pPrimarySurface->Restore();
		pBackBuffer->Restore();

		if (pLogo->IsLost())
		{
			pLogo->Restore();
			LoadBMPinEXE(pLogo,MAKEINTRESOURCE(IDB_LOGO));
		}

		if (pBrandName->IsLost())
		{
			pBrandName->Restore();
			LoadBMPinEXE(pBrandName,MAKEINTRESOURCE(IDB_LOGO_ALL));
		}

		if (pMouse->IsLost())
		{
			pMouse->Restore();
			LoadBMPinEXE(pMouse,MAKEINTRESOURCE(IDB_CURSOR));
		}

		PrepareStar();
	}
}
//--------------------------------------------------------
// Расчитывает новое значение частоты смены кадров
//--------------------------------------------------------
void UpdateFPS()
{
/*	HDC hdcFPS;
	//char s[10];
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

		wsprintf(fps.fname,"%d fps ",nFps);

		/*
		ClearSurface(pFPS);
		if ((pFPS->GetDC(&hdcFPS)) == DD_OK)
		{
			ShadowTextOut(hdcFPS,FPS_FONT, fps.x,fps.y,s);
			pFPS->ReleaseDC(hdcFPS);
		}
	
	}
	if(fps.visible)	OutStr(fps.x,fps.y,fps.fname);
*/
	DisplayFrameRate();
}

void	UpdateAccount()
{
/*
	HDC hdcAccount;
	char s[10];

	wsprintf(s,"%d $",account);
	ClearSurface(pAccount);

	if ((pAccount->GetDC(&hdcAccount)) == DD_OK)
	{
		ShadowTextOut(hdcAccount,SCORE_FONT, 200,5,s);
		pAccount->ReleaseDC(hdcAccount);
	}
*/

	wsprintf(buffer,"@ %d",ship.hp);
	OutStr(5,5,buffer);

	wsprintf(buffer,"$ %d",account);
	OutStr(100,5,buffer);
	
	wsprintf(buffer,"# %d",level.length);
	OutStr(200,5,buffer);
}


void OutStr(int x, int y, LPCTSTR text)
{
    if (pBackBuffer->GetDC(&hdc) == DD_OK)
    {
        SetBkColor(hdc, RGB(0, 0, 00));
        SetTextColor(hdc, RGB(250, 250, 250));
        GetClientRect(hMainWnd, &rc);
        GetTextExtentPoint(hdc, text, lstrlen(text), &size);
        TextOut(hdc, x, y, text, lstrlen(text));

        pBackBuffer->ReleaseDC(hdc);
    }
}

//---------------------------------------- SHIP (BEGIN) ---------------------------------------

bool InitShip(OBJECT * obj, BYTE tp) // инициализация (только для pShip)
{
	int i;

	SAFE_RELEASE(pShip);

	switch (tp)
	{
		case SHIP_TYPE_M: 
			InitObject(obj,MAX_WIDTH/2,MAX_HEIGHT/2,40,44,SHIP_SPEED); 
			for (i=0;i<128;i++ )	
			{
				bullet[i].sx1=0;
				bullet[i].sy1=14;
				bullet[i].sx2=38;
				bullet[i].sy2=14;
			}
			obj->x-=obj->cx;
			obj->y-=obj->cy;
			obj->hp=obj->hp_=1000;
			obj->damage_min=200;
			obj->damage_max=250;
			obj->speed_f=0.01f;
			break;

		case SHIP_TYPE_H: 
			InitObject(obj,MAX_WIDTH/2,MAX_HEIGHT/2,44,28,SHIP_SPEED); 
			for (i=0;i<128;i++ )	
			{
				bullet[i].sx1=11;
				bullet[i].sy1=-6;
				bullet[i].sx2=31;
				bullet[i].sy2=-6;
			}
			obj->x-=obj->cx;
			obj->y-=obj->cy;
			obj->hp=obj->hp_=800;
			obj->damage_min=100;
			obj->damage_max=150;
			obj->speed_f=0.012f;
			break;


		case SHIP_TYPE_G: 
			InitObject(obj,MAX_WIDTH/2,MAX_HEIGHT/2,44,40,SHIP_SPEED);
			for (i=0;i<128;i++ )	
			{
				bullet[i].sx1=19;
				bullet[i].sy1=-12;
				bullet[i].sx2=24;
				bullet[i].sy2=-12;
			}
			obj->x-=obj->cx;
			obj->y-=obj->cy;
			obj->hp=obj->hp_=1000;
			obj->damage_min=100;
			obj->damage_max=200;
			obj->speed_f=0.01f;
			break;

		case SHIP_TYPE_F: 
			InitObject(obj,MAX_WIDTH/2,MAX_HEIGHT/2,44,40,SHIP_SPEED);
			for (i=0;i<128;i++ )	
			{
				bullet[i].sx1=20;
				bullet[i].sy1=-12;
				bullet[i].sx2=22;
				bullet[i].sy2=-12;
			}
			obj->x-=obj->cx;
			obj->y-=obj->cy;
			obj->hp=obj->hp_=1000;
			obj->damage_min=150;
			obj->damage_max=200;
			obj->speed_f=0.01f;
			break;

		case SHIP_TYPE_P: 
			InitObject(obj,MAX_WIDTH/2,MAX_HEIGHT/2,44,44,SHIP_SPEED); 
			for (i=0;i<128;i++ )	
			{
				bullet[i].sx1=0;
				bullet[i].sy1=0;
				bullet[i].sx2=42;
				bullet[i].sy2=0;
			}
			obj->x-=obj->cx;
			obj->y-=obj->cy;
			obj->hp=obj->hp_=1500;
			obj->damage_min=100;
			obj->damage_max=150;
			obj->speed_f=0.0095f;
			break;

		case SHIP_TYPE_K: 
			InitObject(obj,MAX_WIDTH/2,MAX_HEIGHT/2,52,40,SHIP_SPEED); 
			for (i=0;i<128;i++ )	
			{
				bullet[i].sx1=15;
				bullet[i].sy1=0;
				bullet[i].sx2=34;
				bullet[i].sy2=0;
			}
			obj->x-=obj->cx;
			obj->y-=obj->cy;
			obj->hp=obj->hp_=2000;
			obj->damage_min=100;
			obj->damage_max=500;
			obj->speed_f=0.01f;
			break;
	}

	obj->b=MAX_HEIGHT+500;

	pShip=CreateSurface(obj->w,obj->h);
	if (!pShip)
		return (FALSE);

	pShip->SetColorKey(DDCKEY_SRCBLT,&ddColorKey);

	switch (tp)
	{
		case SHIP_TYPE_M: if (!LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_M))) return (FALSE); break;
		case SHIP_TYPE_H: if (!LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_H))) return (FALSE); break;
		case SHIP_TYPE_G: if (!LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_G))) return (FALSE); break;
		case SHIP_TYPE_F: if (!LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_F))) return (FALSE); break;
		case SHIP_TYPE_P: if (!LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_P))) return (FALSE); break;
		case SHIP_TYPE_K: if (!LoadBMPinEXE(pShip,MAKEINTRESOURCE(IDB_SHIP_K))) return (FALSE); break;
	}

	return true;
}


void MoveShip(bool up,bool down, bool left, bool rigth,bool fire,bool accsel) //перемещение
{
	if (!MoveShipLock)
	{

		if (up && ship.y>ship.odz.top )	ship.y--;
		if (down && ship.y<ship.odz.bottom)	ship.y+=3;

		if (left)
		{
			if (ship.x>ship.odz.left) ship.x-=4;	
			l=true;
			r=false;
		}
		else
		{
			l=false;
		}

		if (rigth)	
		{
			if (ship.x<ship.odz.right)ship.x+=4;
			r=true;
			l=false;
		}
		else
		{
			r=false;
		}

		if (accsel)
		{
			if(g<MAX_ACCELERATION)g++;
		}
		else
		{
			if(g>0)g--;
		}

		if (fire)
		{
			if (reload>20)
			{
				//for(BYTE i=0;i<WEAPON_POWER;i++)	NewBullet(i);
				//NewBullet(0);
				NewBullet(&weapon_blaster,&ship);
			}
			else
			{
				reload++;
			}
		}
		else
		{
			reload=20;
		}
	}
}

void NewBullet(BYTE power)
{
	index++;
	if(index==128)index=0;
	
	bullet[index].x1=ship.x+bullet[index].sx1;
	bullet[index].y1=ship.y+bullet[index].sy1+bullet[index].h;
	bullet[index].x2=ship.x+bullet[index].sx2;
	bullet[index].y2=ship.y+bullet[index].sy2+bullet[index].h;
	bullet[index].x3=ship.x+bullet[index].sx3;
	bullet[index].y3=ship.y+bullet[index].sy3+bullet[index].h;
	bullet[index].speed=BULLET_SPEED;
	reload=0;
	account--;

	PlaySoundBuffer(FIRE_SOUND);
}

void NewBullet(OBJECT * obj,OBJECT * sh)
{

	obj->x=sh->x+(sh->cx-obj->cx);
	obj->y=sh->y;

	//AddObjToList(obj);
	AddObjToList(lpListBullet,obj);
//	AddObjToListBullet(obj);


	reload=0;
	account--;
	PlaySoundBuffer(FIRE_SOUND);
}


void DeletBullet(OBJECT * obj ,int ind)
{
	for (int i=ind;i<obj->power+ind+1;i++)
	{
		obj->y1=obj->a;
		obj->speed=0;
		obj++;
	}
}

//--------------------------------------SHIP (END) -------------------------------------------------
//
//
// ------------------------------------ STAR (BEGIN) ------------------------------------------------
void PrepareStar()
{
	if (pStarL->IsLost())
	{
		pStarL->Restore();
		LoadBMPinEXE(pStarL,MAKEINTRESOURCE(IDB_STAR_DEEP));
	}
	
	if (pStarL->IsLost())
	{
		pStarL->Restore();
		LoadBMPinEXE(pStarL,MAKEINTRESOURCE(IDB_STAR_L));
	}

	if (pStarM->IsLost())
	{
		pStarM->Restore();
		LoadBMPinEXE(pStarM,MAKEINTRESOURCE(IDB_STAR_M));
	}

	if (pStarS->IsLost())
	{
		pStarS->Restore();
		LoadBMPinEXE(pStarS,MAKEINTRESOURCE(IDB_STAR_S));
	}
}
void InitStar() // Инициализация звездного неба 
{
	int rnd;
	int i;

	for(i=0;i<MAX_STAR_DEEP;i++) 
	{
			while(true)
			{
				rnd=rand()-rand();
				if(rnd>0) break;
			}
			InitObject(&star_d[i],i*MUL_STAR_DEEP,rndInt(0,MAX_HEIGHT),1,1,0);
	}

	for(i=0;i<MAX_STAR_S;i++) 
	{
			while(true)
			{
				rnd=rand()-rand();
				if(rnd>0) break;
			}
			InitObject(&star_s[i],i*MUL_STAR_S,rndInt(0,MAX_HEIGHT),1,1,1);
	}

	for(i=0;i<MAX_STAR_M;i++) 
	{
			while(true)
			{
				rnd=rand()-rand();
				if(rnd>0) break;
			}
			InitObject(&star_m[i],i*MUL_STAR_M,rndInt(0,MAX_HEIGHT),1,1,2);
	}

	for(i=0;i<MAX_STAR_L;i++) 
	{
			while(true)
			{
				rnd=rand()-rand();
				if(rnd>0) break;
			}
			InitObject(&star_l[i],i*MUL_STAR_L,i-rnd,1,1,3);
	}
}

void DrawStar(LPDIRECTDRAWSURFACE7 lpdds, OBJECT * obj) // Прорисовка звезды
{
		obj->y+=obj->speed+g;
		if(	obj->y>MAX_HEIGHT)obj->y=-obj->h;

		if (l)
		{
			obj->x+=obj->speed;
			if(	obj->x>MAX_WIDTH)obj->x=-obj->w;
		}
		else if(r)
		{
			obj->x-=obj->speed;
			if(	obj->x<0)obj->x=MAX_WIDTH;
		}
		pBackBuffer->BltFast(obj->x,obj->y,lpdds,&obj->pic,DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
	//	DrawObjectNO(lpdds,obj);
}
// ----------------------------------- STAR (END) -----------------------------------------------
//
// ----------------------------------- LOGO (BEGIN) -------------------------------------------
void InitLogo()
{
	MoveShipLock=true;
	InitObject(&logo,MAX_WIDTH/2-565/2,5,565,88,0);
	InitObject(&cursor,MAX_WIDTH/2-14,MAX_HEIGHT/2-16,28,32,0);
	InitObject(&bname[7],MAX_WIDTH/2-16,MAX_HEIGHT/2-16,32,224,0);
	bname[7].b=3;
	
	for(int i=0;i<7;i++) 
	{
		InitObject(&bname[i],(int)((MAX_WIDTH/2-16) + 300* SinTable[60*i]),	(int)((MAX_HEIGHT/2-16) -300 * CosTable[60*i]),32,32,15);
		bname[i].b=500;
		bname[i].a=i;
	}
/*
	comet.a=6;
	comet.x1=4;
	comet.x2=2;
	comet.x3=0;

	comet.fb=40;
	comet.fx1=38;
	comet.fx2=36;
	comet.fx3=34;
	*/

	HoleInit();

}


void HoleInit()
{
	MoveShipLock=true;
	InitObject(&comet,0,0,1,1,1);

	comet.b=0;
	comet.y1=0;
	comet.y2=0;
	comet.y2=0;

	comet.a=0;
	comet.x1=0;
	comet.x2=90;
	comet.x3=135;
	k1=2;
	k2=1;
	r_hi=110;
	r_low=20;
	loop=1;
	inc=1;
}

void Logo()
{
	PrepareLogo();
	ClearSurface(pBackBuffer);

	//ПРОРИСОВКА ЗВЁЗД	

//	for(id=0;id<MAX_STAR_DEEP;id++) DrawStar(pStarD,&star_d[id]);
	for(id=0;id<MAX_STAR_S;id++) 	DrawStar(pStarS,&star_s[id]);
	for(id=0;id<MAX_STAR_M;id++) 	DrawStar(pStarM,&star_m[id]);
	for(id=0;id<MAX_STAR_L;id++) 	DrawStar(pStarL,&star_l[id]);

	if (loop<180)loop++;
	if(fasa==2)loop-=10;

	if(fasa==3||fasa==2)
	for(int i=0;i<loop;i++)
	{
		comet.x=(int)screen.cx + comet.b * SinTable[comet.a];
		comet.y=(int)screen.cy -comet.b * CosTable[comet.a];
		comet.a-=k1;
		comet.b-=k2;

		if(comet.a<0) comet.a=360;
		if(comet.a>360) comet.a=0;
		if(comet.b<r_low) comet.b=r_hi;

		DrawObjectNO(pStarD,&comet);

		comet.x=(int)screen.cx + comet.y1 * SinTable[comet.x1];
		comet.y=(int)screen.cy -comet.y1 * CosTable[comet.x1];
		comet.x1-=3;
		comet.y1-=k2;

		if(comet.x1<0) comet.x1=360;
		if(comet.x1>360) comet.x1=0;
		if(comet.y1<100) comet.y1=190;

		DrawObjectNO(pStarD,&comet);

		comet.x=(int)screen.cx + comet.y2 * SinTable[comet.x2];	
		comet.y=(int)screen.cy -comet.y2 * CosTable[comet.x2];
		comet.x2-=k1;
		comet.y2-=k2;

		if(comet.x2<0) comet.x2=360;
		if(comet.x2>360) comet.x2=0;
		if (comet.y2<r_low) comet.y2=r_hi;

		DrawObjectNO(pStarD,&comet);

		comet.x=(int)screen.cx + comet.y3 * SinTable[comet.x3];	
		comet.y=(int)screen.cy -comet.y3  * CosTable[comet.x3];
		comet.x3-=2;
		comet.y3-=1;

		if(comet.x3<0) comet.x3=360;
		if(comet.x3>360) comet.x3=0;
		if (comet.y3<180) comet.y3=270;

		DrawObjectNO(pStarD,&comet);
/*

		dx=screen.cx + comet.y1 * SinTable[comet.x1];
		dy=screen.cy -comet.y1 * CosTable[comet.x1];

		comet.x=(int)dx;	
		comet.y=(int)dy;
		comet.x1-=2;
		comet.y1+=k;

		if(comet.x1<0) comet.x1=360;
		if (comet.y1>rd) comet.y1=0;




		comet.x=(int)screen.cx + comet.y2 * SinTable[comet.x2];	
		comet.y=(int)screen.cy -comet.y2 * CosTable[comet.x2];
		comet.x2-=2;
		comet.y2-=k;

		if(comet.x2<0) comet.x2=360;
		if (comet.y2<r_low) comet.y2=r_hi;

//		DrawObjectNO(pStarD,&comet);


		comet.x=(int)screen.cx + comet.y3 * SinTable[comet.x3];	
		comet.y=(int)screen.cy -comet.y3  * CosTable[comet.x3];
		comet.x3-=2;
		comet.y3-=k;

		if(comet.x3<0) comet.x3=360;
		if (comet.y3<r_low) comet.y3=r_hi;
		*/
//		DrawObjectNO(pStarD,&comet);
	}

	AnimMenu(fasa,fasa);

	if(logo.visible)
	{
		DrawObjectSRC(pLogo,&logo);
		OutStr(MAX_WIDTH-100,MAX_HEIGHT-35,ver);
	}



	if(ship.visible) DrawObjectSRC(pShip,&ship);
	if(!bKliss && ship.a==6)
	{
		SetRect(&bname[id].pic,0,7*32-32,32,7*32);
		DrawObjectSRC(pBrandName,&bname[7]);
	}


	if (fasa==3) DrawObjectSRC(pMouse,&cursor);
	if (fasa==0) 
	{
		bLogo=false;
		fasa=1;
		NewGame();
		PlayLevel(++CurrentLevel);
	}

	if (BlackScreen())
	{
		ClearSurface(pBackBuffer);
		PostMessage(hMainWnd,WM_CLOSE,0,0);
	}


	if(DEBUG) UpdateFPS();

	pPrimarySurface->Flip(NULL,DDFLIP_WAIT);
}

void MoveMouse(int x,int y, int scroll,bool lb, bool rb,bool cb)
{
	if (!MouseLock)
	{
		cursor.x=x;
		cursor.y=y;
		if (lb) 
			for (int i=0;i<7;i++)
			{
				if (Crossing(&bname[i],x,y) && ship.a!=6)
				{
					fasa=2;
					ship.visible=true;
				}
			}
				
		if (fasa!=2) ship.visible=false;

		if(!ship.visible)
		for (int i=0;i<7;i++)
			if (Crossing(&bname[i],x,y))
					if(InitShip(&ship,i)) 
					{
						ship.visible=true; 
						ship.a=i;
					}
					else
					{
						ship.visible=false;
					}

	}
}
// ----------------------------------- LOGO (END) -------------------------------------------
//
//
// ----------------------------------- OBJECT (BEGIN) ----------------------------------------
void InitObject(OBJECT * obj,int x, int y, int w,int h,int speed)
{
	obj->x=		x;
	obj->y=		y;
	obj->h=		h;
	obj->w=		w;
	obj->cx=	w/2;
	obj->cy=	h/2;
	obj->speed=	speed;
	obj->id=	GetNewID();
	obj->frame	=0;
	obj->visible=true;
	obj->a		=0;
	obj->b		=0;
	SetRect(&obj->pic,0,0,w,h);
	SetODZ(obj);
}

//--------------------- Область допустимых значений ---------------------------

void SetODZ(OBJECT *obj)
{
	SetRect(&obj->odz,4,4,MAX_WIDTH-(obj->w+4),MAX_HEIGHT-(obj->h+4));
}

//--------------------- Пересечение точки с объектом ---------------------------

inline bool Crossing(OBJECT *obj, int x, int y)
{	
	if ((x>obj->x) && (x< obj->x+obj->w) && (y>obj->y) && (y< obj->y+obj->h )) return true; else return false;
}

//--------------------- Пересечение объектов ---------------------------

inline bool Crossing(OBJECT *obj1, OBJECT *obj2)
{
	if ((max(obj1->x,obj2->x)	>= min(obj1->x+obj1->w,obj2->x+obj2->w))||
		(max(obj1->y,obj2->y)   >= min(obj1->y+obj1->h,obj2->y+obj2->h)))
		return false;
	else
		return true;
}
//--------------------- CLIPPER ---------------------------

void DrawObjectCutSRC(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj)
{
	if((obj->y>-obj->h) && (obj->y<MAX_HEIGHT) && (obj->x>-obj->w) && (obj->x<MAX_WIDTH) && (obj->visible==true))
	{
		int left,top,right,bottom,x=obj->x,y=obj->y;	

		if (MAX_HEIGHT-obj->y<obj->h)	bottom=MAX_HEIGHT-obj->y;		else bottom=obj->h;
		if (MAX_WIDTH-obj->x<obj->w)	right=MAX_WIDTH-obj->x;			else right=obj->w;
		if (obj->y<0)					{top=abs(obj->y); y=0;}			else top=0;	
		if (obj->x<0)					{left=abs(obj->x); x=0;}		else left=0;

		SetRect(&obj->pic,left,top,right,bottom);
		pBackBuffer->BltFast(x,y,lpdds,&obj->pic,DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
	}
}

void DrawObjectSRC(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj)
{
	if (obj->visible)
	{
		pBackBuffer->BltFast(obj->x,obj->y,lpdds,&obj->pic,DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
	}
}

void DrawObjectNO(LPDIRECTDRAWSURFACE7 lpdds,OBJECT * obj)
{
	if (obj->visible)
	{
		pBackBuffer->BltFast(obj->x,obj->y,lpdds,&obj->pic,DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
	}
}


// ----------------------------------- OBJECT (END) -------------------------------------------
//
//
// ----------------------------------- NEW GAME (RESET WAR) -------------------------------------------
void NewGame()
{
	expl_id=		0;
	index=			0;
	reload=			0;
	g=				0;
	id=				0;
	account=		0;
	l=				false;
	r=				false;
	GameOver=		false;
	MoveShipLock=	false;

	InitObject(&screen,0,0,MAX_WIDTH,MAX_HEIGHT,0);
	
	ship.visible=true;

	for (int i=0;i<MAX_ENEMI_L;i++ )	
	{
		InitObject(&enemi_l[i],i*MUL_ENEMI_L,rndInt(-9999,0),56,40,1);
		enemi_l[i].a=enemi_l[i].x;
		enemi_l[i].b=enemi_l[i].y;
		enemi_l[i].hp=enemi_l[i].hp_=200;
		enemi_l[i].damage_min=30;
		enemi_l[i].damage_max=50;
		enemi_l[i].visible=true;
		enemi_l[i].type=ID_ENEMI_L;
	}

	for (i=0;i<MAX_ENEMI_M;i++ )	
	{
		if (i<MAX_ENEMI_M/2) 
			InitObject(&enemi_m[i],rndInt(10,200),-i*MUL_ENEMI_M,32,32,5);
		else 
			InitObject(&enemi_m[i],rndInt(MAX_WIDTH-200,MAX_WIDTH-10),-i*MUL_ENEMI_M,32,32,4);
		enemi_m[i].a=enemi_m[i].x;
		enemi_m[i].b=enemi_m[i].y-3000;
		enemi_m[i].hp=enemi_s[i].hp_=100;
		enemi_m[i].damage_min=10;
		enemi_m[i].damage_max=20;
		enemi_m[i].visible=true;
		enemi_m[i].type=ID_ENEMI_M;
	}

	for (i=0;i<MAX_ENEMI_S;i++ )	
	{
		InitObject(&enemi_s[i],i*MUL_ENEMI_S,rndInt(-1050,-1000),20,28,4);
		enemi_s[i].a=enemi_s[i].x;
		enemi_s[i].b=enemi_s[i].y-1000;
		enemi_s[i].hp=enemi_s[i].hp_=50;
		enemi_s[i].damage_min=5;
		enemi_s[i].damage_max=10;
		enemi_s[i].visible=true;
		enemi_s[i].type=ID_ENEMI_S;
	}

	for (i=0;i<128;i++ )	//BULLET
	{
		bullet[i].speed=0;
		bullet[i].h=24;
		bullet[i].w=2;
		bullet[i].y1=bullet[i].y2=bullet[i].a=MAX_HEIGHT+500;
		bullet[i].damage_max=60;
		bullet[i].damage_min=50;
		bullet[i].power=1;
		SetRect(&bullet[i].pic,0,0,2,24);
	}

	InitObject(&weapon_blaster,0,0,2,24,10);
	weapon_blaster.damage_max=60;
	weapon_blaster.damage_min=50;
	weapon_blaster.power=1;
	weapon_blaster.type=ID_BLASTER;


	for (i=0;i<32;i++ )	InitObject(&expl_m[i],-500,-500,40,12,0);

	InitObject(&planet,700,-200,160,160,5);
	InitObject(&fps,MAX_WIDTH-100,5,FPS_WIDTH,FPS_HEIGHT,0);

//	HoleInit();
	PointText(point);

	level.length=2300;
	level.length_d=2300;
	level.number=1;

	if (CurrentLevel>0) PlayMusic(LEVEL_MUSIC);


	lpList=new LIST;
	lpList->next=NULL;
	lpList->prev=NULL;
	lpHead=lpList;

	lpListBullet=new LIST;
	lpListBullet->next=NULL;
	lpListBullet->prev=NULL;
	lpHeadBullet=lpListBullet;


}

void PointText(POINT_TEXT p[32])
{
	p[0].x=176;	p[0].y=290;	p[1].x=254;	p[1].y=290;	p[2].x=216;	p[2].y=342;	p[3].x=218;	p[3].y=390;
	p[4].x=332;	p[4].y=288;	p[5].x=330;	p[5].y=390;	p[6].x=290;	p[6].y=360;	p[7].x=368;	p[7].y=364;
	p[8].x=370;	p[8].y=310;	p[9].x=296;	p[9].y=308;
	p[10].x=414;
	p[10].y=290;
	p[11].x=486;
	p[11].y=288;
	p[12].x=420;
	p[12].y=368;
	p[13].x=482;
	p[13].y=372;
	p[14].x=452;
	p[14].y=390;
	p[15].x=574;
	p[15].y=388;
	p[16].x=576;
	p[16].y=290;
	p[17].x=624;
	p[17].y=292;
	p[18].x=630;
	p[18].y=386;
	p[19].x=648;
	p[19].y=334;
	p[20].x=694;
	p[20].y=288;
	p[21].x=696;
	p[21].y=392;
	p[22].x=736;
	p[22].y=394;
	p[23].x=736;
	p[23].y=290;
	p[24].x=736;
	p[24].y=338;
	p[25].x=808;
	p[25].y=390;
	p[26].x=804;
	p[26].y=288;
	p[27].x=770;
	p[27].y=292;
	p[28].x=776;
	p[28].y=392;
	p[29].x=766;
	p[29].y=342;
	p[30].x=576;
	p[30].y=340;
	p[31].x=694;
	p[31].y=336;
}


void AnimMenu(BYTE direct, BYTE & out)
{
	switch(direct)
	{
		case 0:
			break;
		case 1:
			for(id=0;id<7;id++)//сворачивание 
			{
				if(	bname[id].b>100)
				{
					bname[id].b-=bname[id].speed;
					bname[id].x=(int)((MAX_WIDTH/2-16) + bname[id].b* SinTable[60*id]);
					bname[id].y=(int)((MAX_HEIGHT/2-16) -bname[id].b* CosTable[60*id]);
					SetRect(&bname[id].pic,0,id*32-32,32,id*32);
					DrawObjectSRC(pBrandName,&bname[id]);
					ship.visible=false;
					logo.visible=false;
				}
				else
				{
					out=3;
				}
			}
			break;
		case 2:
			for(id=0;id<7;id++)// разворачивание
			{
				if(	bname[id].b<700)
				{
					bname[id].b+=bname[id].speed;
					bname[id].x=(int)((MAX_WIDTH/2-16) + bname[id].b* SinTable[60*id]);
					bname[id].y=(int)((MAX_HEIGHT/2-16) -bname[id].b* CosTable[60*id]);
					SetRect(&bname[id].pic,0,id*32-32,32,id*32);
					DrawObjectSRC(pBrandName,&bname[id]);
					logo.visible=false;
				}
				else
				{
					out=0;
				}
			}
			break;
		case 3:
			for(id=0;id<7;id++)
			{
				if (g>0) g--;	
				logo.visible=true;
				SetRect(&bname[id].pic,0,id*32-32,32,id*32);
				DrawObjectSRC(pBrandName,&bname[id]);
			}
			break;
	}
}


bool BlackScreen()
{
	if (bExit)
	{
		if (DEBUG) return true;
		for (int i = 1; i < 256; i++)
		{
			if(pal[i].peRed>0)		pal[i].peRed--;				
			if(pal[i].peGreen>0)	pal[i].peGreen--;		
			if(pal[i].peBlue>0)		pal[i].peBlue--;
			if(pal[i].peRed>0)		pal[i].peRed--;				
			if(pal[i].peGreen>0)	pal[i].peGreen--;		
			if(pal[i].peBlue>0)		pal[i].peBlue--;
			if(pal[i].peRed>0)		pal[i].peRed--;				
			if(pal[i].peGreen>0)	pal[i].peGreen--;		
			if(pal[i].peBlue>0)		pal[i].peBlue--;
			if(pal[i].peRed>0)		pal[i].peRed--;				
			if(pal[i].peGreen>0)	pal[i].peGreen--;		
			if(pal[i].peBlue>0)		pal[i].peBlue--;
			if(pal[i].peRed>0)		pal[i].peRed--;				
			if(pal[i].peGreen>0)	pal[i].peGreen--;		
			if(pal[i].peBlue>0)		pal[i].peBlue--;
		}
		void	FadeOut();
		pDDPal->SetEntries(0, 0, 256, pal);
		if (bscreen>50) return true; else bscreen++;
	}
	return false;
}


void PlayLevel(BYTE number)
{
	if (number>0) PlayMusic(LEVEL_MUSIC);
}



//----------------------------------- DRAW FRAME (BEGIN) ---------------------------------------
//------------------------------------------------------
// Вывод очередного кадра игры
//------------------------------------------------------
void DrawFrame()
{
	PrepareFrame();
	ClearSurface(pBackBuffer);

//	ПРОРИСОВКА ЗВЁЗД	
//	for(id=0;id<MAX_STAR_DEEP;id++) DrawStar(pStarD,&star_d[id]);
	for(id=0;id<MAX_STAR_S;id++) 	DrawStar(pStarS,&star_s[id]);
	for(id=0;id<MAX_STAR_M;id++) 	DrawStar(pStarM,&star_m[id]);
	for(id=0;id<MAX_STAR_L;id++) 	DrawStar(pStarL,&star_l[id]);


	if ((level.length%500)==0)
	{
		for(id=0;id<MAX_ENEMI_L;id++)AddObjToList(lpList,&enemi_l[id]);
		level.length_d--;
	}
	

	if ((level.length%30)==0)
	{
		for(id=0;id<MAX_ENEMI_S;id++)AddObjToList(lpList,&enemi_s[id]);
		level.length_d--;
	}

	if ((level.length%100)==0)
	{
		for(id=0;id<MAX_ENEMI_M/2;id++)AddObjToList(lpList,&enemi_m[id]);
		level.length_d--;
	}

	if ((level.length%50)==0)
	{
		for(id=MAX_ENEMI_M/2;id<MAX_ENEMI_M;id++)AddObjToList(lpList,&enemi_m[id]);
		level.length_d--;
	}



	lpListBullet=lpHeadBullet;
	while(NextBullet())
	{
		if (DEBUG)object_count++;

		if(lpListBullet->obj.type==ID_DELETE) 
		{
			if(lpListBullet->next!=NULL && lpListBullet->prev!=NULL)
			{
				lpTmp=lpListBullet;
				lpListBullet->next->prev=lpListBullet->prev;
				lpListBullet->prev->next=lpListBullet->next;
				lpListBullet=lpListBullet->prev;
				if(!lpTmp)	delete lpTmp;
			}
			continue;
		}


		lpListBullet->obj.y-=lpListBullet->obj.speed;		
		if(lpListBullet->obj.y>0)
		{
			DrawObjectNO(pBullet,&lpListBullet->obj);
		}
		else
		{
			lpListBullet->obj.type=0;
		}
	}


	if (DEBUG)
	{
		wsprintf(buffer,"%d",object_count);
		OutStr(30,30,buffer);
		object_count=0;
	}



	lpList=lpHead;
	while(Next())
	{
		if (DEBUG)object_count++;
		
		if(lpList->obj.type==ID_DELETE) 
		{
			if(lpList->next!=NULL && lpList->prev!=NULL)
			{
				lpTmp=lpList;
				lpList->next->prev=lpList->prev;
				lpList->prev->next=lpList->next;
				lpList=lpList->prev;
				if(!lpTmp)	delete lpTmp;
			}
			continue;
		}



		switch(lpList->obj.type)
		{

//SMALL 
			case ID_ENEMI_S:

				lpList->obj.y+=lpList->obj.speed+g;
				if ((lpList->obj.y>0) && (lpList->obj.y<MAX_HEIGHT))
				{
					if(!GameOver)
					{		
						if (lpList->obj.x>ship.x)lpList->obj.x-=1; else lpList->obj.x+=1; 
					}
				}

				if(lpList->obj.visible) DrawObjectCutSRC(pEnemiS,&lpList->obj);

				break;

//MEDIUM 
			case ID_ENEMI_M:

				lpList->obj.y+=lpList->obj.speed+g;
				if ((lpList->obj.y>0) && (lpList->obj.y<MAX_HEIGHT))
				{
					if(!GameOver)
					{		
						if (lpList->obj.x>ship.x)lpList->obj.x-=2; else lpList->obj.x+=2; 
					}
				}

				if(lpList->obj.visible) DrawObjectCutSRC(pEnemiM,&lpList->obj);

				break;

//LARGE
			case ID_ENEMI_L:

				lpList->obj.y+=lpList->obj.speed+g;

				if(!GameOver)
				{		

				}

				if(lpList->obj.visible) DrawObjectCutSRC(pEnemiL,&lpList->obj);

				break;
		}

		if (lpList->obj.y>screen.h)
		{
			lpList->obj.type=0;
			continue;
		}

		if (Crossing(&lpList->obj,&ship))
		{
			ship.hp-=			rndInt(lpList->obj.damage_min,lpList->obj.damage_max);
			lpList->obj.hp-=	rndInt(ship.damage_min,ship.damage_max);

			PlaySoundBuffer(HIT_SOUND);
		}

		lpListBullet=lpHeadBullet;
		while (NextBullet())
		{
			if (Crossing(&lpList->obj,lpListBullet->obj.x,lpListBullet->obj.y))
			{
				lpList->obj.hp-=rndInt(lpListBullet->obj.damage_min,lpListBullet->obj.damage_max);
				lpListBullet->obj.type=0;

				if(lpListBullet->next!=NULL && lpListBullet->prev!=NULL)
				{
					lpTmp=lpListBullet;
					lpListBullet->next->prev=lpListBullet->prev;
					lpListBullet->prev->next=lpListBullet->next;
					lpListBullet=lpListBullet->prev;
					if(!lpTmp)	delete lpTmp;
				}
				break;
			}
		}


		if(lpList->obj.hp<1)
		{
			PlaySoundBuffer(BOOM_SOUND);
			NewBoom(&lpList->obj);
			account+=lpList->obj.cost;
			lpList->obj.type=0;
		}
	
	}//END 	while(Next())
	


	if (DEBUG)
	{
		wsprintf(buffer,"%d",object_count);
		OutStr(5,30,buffer);
		object_count=0;
	}

	//ПРОРИСОВКА КОРАБЛИКА (CHIP)
	if(ship.visible)
	{
		if(	ship.hp<1)
		{
			PlaySoundBuffer(CAPUT_SOUND);
			NewBoom(&ship);
			GameOver=MoveShipLock=true;
		}
		else
		{
			level.length_d-=ship.speed_f+ship.speed_f*g;
			level.length=(unsigned long)level.length_d;
			DrawObjectSRC(pShip,&ship);
		}
	}


	//ПРОРИСОВКА ВЗРЫВА
	for (id=0;id<expl_id+1;id++)
	{
		if (expl_m[id].frame>0)
		{
			DrawObjectSRC(pExplM,&expl_m[id]);
			expl_m[id].frame--;
		}
	}

	UpdateAccount();
	UpdateFPS();


	pBackBuffer->BltFast(0,0,pFPS,&rPic,DDBLTFAST_SRCCOLORKEY|DDBLTFAST_WAIT);
	pPrimarySurface->Flip(NULL,DDFLIP_WAIT);
	Sleep(delay);
}
// ----------------------------------- DRAW FRAME (END) ---------------------------------------
//

void NewBoom(OBJECT *obj)
{
	expl_id++;
	if (expl_id>32)expl_id=0;
	expl_m[expl_id].x=obj->x;
	expl_m[expl_id].y=obj->y;
	expl_m[expl_id].frame=20;

	obj->x=obj->a;
	obj->y=obj->b;
//	obj->visible=false;
	obj->hp=obj->hp_;
}


void AddObjToList(LIST *lp,OBJECT * obj)
{
	while (lp->next!=NULL)
	{
		if(lp->next!=NULL)
		{
			lp->next->prev=lp;
			lp=lp->next;
		}
	}

	if(lp->next==NULL)
	{
		lp->next=new LIST;
		lp->next->prev=lp;
		lp->next->next=NULL;


		lp->obj.x=obj->x;
		lp->obj.y=obj->y;
		lp->obj.w=obj->w;
		lp->obj.h=obj->h;
		lp->obj.cx=obj->cx;
		lp->obj.cy=obj->cy;
		lp->obj.a=obj->a;
		lp->obj.b=obj->b;
		lp->obj.speed=obj->speed;
		lp->obj.type=obj->type;
		lp->obj.damage_max=obj->damage_max;
		lp->obj.damage_min=obj->damage_min;
		lp->obj.hp=obj->hp;
		lp->obj.visible=obj->visible;
		lp->obj.pic=obj->pic;
		lp->obj.id=obj->id;
		lp=lp->next;
	}
}

void AddObjToList(OBJECT * obj)
{
	End();
	if(lpList->next==NULL)
	{
		lpList->next=new LIST;
		lpList->next->prev=lpList;
		lpList->next->next=NULL;
		lpList=lpList->next;

		lpList->obj.x=obj->x;
		lpList->obj.y=obj->y;
		lpList->obj.w=obj->w;
		lpList->obj.h=obj->h;
		lpList->obj.cx=obj->cx;
		lpList->obj.cy=obj->cy;
		lpList->obj.a=obj->a;
		lpList->obj.b=obj->b;
		lpList->obj.speed=obj->speed;
		lpList->obj.type=obj->type;
		lpList->obj.damage_max=obj->damage_max;
		lpList->obj.damage_min=obj->damage_min;
		lpList->obj.hp=obj->hp;
		lpList->obj.visible=obj->visible;
		lpList->obj.pic=obj->pic;
		lpList->obj.id=obj->id;
	}
}


void AddObjToListBullet(OBJECT * obj)
{
	EndBullet();

	if(lpListBullet->next==NULL)
	{
		lpListBullet->next=new LIST;
		lpListBullet->next->prev=lpListBullet;
		lpListBullet->next->next=NULL;


		lpListBullet->obj.x=obj->x;
		lpListBullet->obj.y=obj->y;
		lpListBullet->obj.w=obj->w;
		lpListBullet->obj.h=obj->h;
		lpListBullet->obj.cx=obj->cx;
		lpListBullet->obj.cy=obj->cy;
		lpListBullet->obj.a=obj->a;
		lpListBullet->obj.b=obj->b;
		lpListBullet->obj.speed=obj->speed;
		lpListBullet->obj.type=obj->type;
		lpListBullet->obj.damage_max=obj->damage_max;
		lpListBullet->obj.damage_min=obj->damage_min;
		lpListBullet->obj.hp=obj->hp;
		lpListBullet->obj.visible=obj->visible;
		lpListBullet->obj.pic=obj->pic;
		lpListBullet->obj.id=obj->id;
		lpListBullet=lpListBullet->next;
	}
}


bool NextBullet()
{
	if(lpListBullet->next!=NULL)
	{
		lpListBullet->next->prev=lpListBullet;
		lpListBullet=lpListBullet->next;
		return true;
	}
	else
	{
		return false;
	}
}

bool PrevBullet()
{
	if(lpListBullet->prev!=NULL)
	{
		lpListBullet->prev->next=lpListBullet;
		lpListBullet=lpListBullet->prev;
		return true;
	}
	else
	{
		return false;
	}
}

void EndBullet()
{
	while (lpListBullet->next!=NULL)
	{
		NextBullet();
	}
}


bool Next(LPLIST lp)
{
	if(lp->next!=NULL)
	{
		lp->next->prev=lp;
		lp=lp->next;
		return true;
	}
	else
	{
		return false;
	}
}

bool Prev(LPLIST lp)
{
	if(lp->prev!=NULL)
	{
		lp->prev->next=lp;
		lp=lp->prev;
		return true;
	}
	else
	{
		return false;
	}
}

bool Next()
{
	if(lpList->next!=NULL)
	{
		lpList->next->prev=lpList;
		lpList=lpList->next;
		return true;
	}
	else
	{
		return false;
	}
}

bool Prev()
{
	if(lpList->prev!=NULL)
	{
		lpList->prev->next=lpList;
		lpList=lpList->prev;
		return true;
	}
	else
	{
		return false;
	}
}

void Begin()
{
	while (lpList->prev!=NULL)
	{
		Prev();
	}
}


void End()
{
	while (lpList->next!=NULL)
	{
		Next();
	}
}

void Insert(OBJECT obj)
{
	if((lpList->next==NULL)&&(lpList->prev==NULL))
	{

	}
	
	End();

	if(lpList->next==NULL)
	{
		lpList->next=new LIST;
		lpList->next->next=NULL;
		lpList->next->prev=lpList;

		lpList->obj=obj;

		//lpNode=lpNode->next;
	}
}

void Delete(int)
{
	if(lpList->prev==NULL)
	{
		lpList->next->prev=NULL;
		lpList=lpList->next;
	}
	else if (lpList->next==NULL)
	{
		lpList->prev->next=NULL;
		lpList=lpList->next;
	}
	else
	{
		lpList->next->prev=lpList->prev;
		lpList->prev->next=lpList->next;
		lpList=lpList->next;
	}
}

void RemoveGame()
{
	delete lpList;
	delete lpListBullet;

}

void DisplayFrameRate( void )
{
    DWORD               time2;
   /// char                buff[256];
    
    dwFrameCount++;
    time2 = timeGetTime() - dwFrameTime;
    if( time2 > 1000 )
    {
        dwFrames = (dwFrameCount*1000)/time2;
        dwFrameTime = timeGetTime();
        dwFrameCount = 0;
    }
    if( dwFrames == 0 )
    {
        return;
    }
    
    if (dwFrames != dwFramesLast)
    {
        dwFramesLast = dwFrames;
    }
    
    if( dwFrames > 99 )
    {
        dwFrames = 99;
    }
    fps.fname[0] = (char)((dwFrames / 10) + '0');
    fps.fname[1] = (char)((dwFrames % 10) + '0');
    fps.fname[2] = '\0';
	strcat(fps.fname," fps");
	OutStr(fps.x,fps.y,fps.fname);
}