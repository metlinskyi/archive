//------------------------------------------------------------------
//	Файл:		SOUND.CPP
//	Описание:	Воспроизведение звуков средствами DirectSound
//------------------------------------------------------------------

#define STRICT

#include <windows.h>
#include <dsound.h>

#include "main.h"
#include "cwave.h"
#include "resource.h"

LPDIRECTSOUND pDS = NULL;

LPDIRECTSOUNDBUFFER pFire = NULL;
LPDIRECTSOUNDBUFFER pBoom = NULL;
LPDIRECTSOUNDBUFFER pCaput = NULL;
LPDIRECTSOUNDBUFFER pHit = NULL;


CMappedWave* pFireWave=NULL;
CMappedWave* pBoomWave=NULL;
CMappedWave* pCaputWave=NULL;
CMappedWave* pHitWave=NULL;

BOOL CreateBuffers();
BOOL PrepareBuffers();

BOOL fUseSound=TRUE;

LONG Volume;

IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, LPCTSTR lpName);
BOOL DSReloadSoundBuffer(IDirectSoundBuffer *pDSB, LPCTSTR lpName);
BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pdwWaveSize);
BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD dwWaveSize);
BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pdwWaveSize);

void FadeOut();

static const char c_szWAV[] = "WAV";
//--------------------------------------------------------
// Освобождает все объекты связанные с DirectSound
//--------------------------------------------------------
void RemoveDirectSound()
{
	SAFE_DELETE(pBoomWave);
	SAFE_DELETE(pFireWave);
	SAFE_DELETE(pCaputWave);	
	SAFE_DELETE(pHitWave);

	SAFE_RELEASE(pFire);
	SAFE_RELEASE(pBoom);
	SAFE_RELEASE(pCaput);
	SAFE_RELEASE(pHit);

	SAFE_RELEASE(pDS);
}

//--------------------------------------------------------
// Главная функция инициализации DirectSound
//--------------------------------------------------------
BOOL	InitDirectSound ()
{
	HRESULT hRet;
	
	hRet=DirectSoundCreate( NULL, &pDS, NULL );
	if (FAILED(hRet))
	{
		if(DEBUG) ErrorHandle(hMainWnd,"почемуто DirectSound нехочет создаватся!");
		return (FALSE);
	}
	
	hRet=pDS->SetCooperativeLevel(hMainWnd,DSSCL_PRIORITY);
	if (FAILED(hRet))
	{
		if(DEBUG) ErrorHandle(hMainWnd,"звуковой Cooperative Level тупит!");
		return (FALSE);
	}

	if (!CreateBuffers())
	{
		if(DEBUG) ErrorHandle(hMainWnd,"Вы удалили файлы с звуковыми эффетками - верните их на место!");
		return (FALSE);
	}

	if (!PrepareBuffers())
	{
		if(DEBUG) ErrorHandle(hMainWnd,"Маленькие буфера!");
		return (FALSE);
	}
	
//	hRet=GetVolume(&Volume);
	

	return (TRUE);
}

//--------------------------------------------------------
// Создаёт требуемые звуковые буферы
//--------------------------------------------------------
BOOL CreateBuffers()
{
	HRESULT hRet; 
	DSBUFFERDESC dsbd;

	pFireWave = new CMappedWave();
	if (!pFireWave)
		return (FALSE);

	pBoomWave = new CMappedWave();
	if (!pBoomWave)
		return (FALSE);

	pCaputWave = new CMappedWave();
	if (!pCaputWave)
		return (FALSE);

	pHitWave = new CMappedWave();
	if (!pHitWave)
		return (FALSE);

	if (!pFireWave->Open("shoot.wav"))	return (FALSE);
	if (!pBoomWave->Open("caput.wav"))	return (FALSE);
	if (!pCaputWave->Open("boom.wav"))	return (FALSE);
	if (!pHitWave->Open("hit.wav"))	return (FALSE);


	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize        = sizeof(DSBUFFERDESC);
	dsbd.dwFlags       = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = pFireWave->GetWaveSize();
	dsbd.lpwfxFormat   = pFireWave->GetWaveHeader();

	hRet=pDS->CreateSoundBuffer(&dsbd, &pFire, NULL );
	if (FAILED(hRet))
		return (FALSE);


	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize        = sizeof(DSBUFFERDESC);
	dsbd.dwFlags       = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = pBoomWave->GetWaveSize();
	dsbd.lpwfxFormat   = pBoomWave->GetWaveHeader();

	hRet=pDS->CreateSoundBuffer(&dsbd, &pBoom, NULL );
	if (FAILED(hRet))
		return (FALSE);


	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize        = sizeof(DSBUFFERDESC);
	dsbd.dwFlags       = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = pCaputWave->GetWaveSize();
	dsbd.lpwfxFormat   = pCaputWave->GetWaveHeader();

	hRet=pDS->CreateSoundBuffer(&dsbd, &pCaput, NULL );
	if (FAILED(hRet))
		return (FALSE);


	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize        = sizeof(DSBUFFERDESC);
	dsbd.dwFlags       = DSBCAPS_STATIC;
	dsbd.dwBufferBytes = pHitWave->GetWaveSize();
	dsbd.lpwfxFormat   = pHitWave->GetWaveHeader();

	hRet=pDS->CreateSoundBuffer(&dsbd, &pHit, NULL );
	if (FAILED(hRet))
		return (FALSE);

	return (TRUE);
}

//--------------------------------------------------------
// Заполняет звуковые буферы данными
//--------------------------------------------------------
BOOL PrepareBuffers()
{
	HRESULT hRet; 
	VOID*   pPtr1  = NULL;
	VOID*   pPtr2 = NULL;
	DWORD   dwSize1=0;
	DWORD   dwSize2=0;


	DWORD dwWaveSize=pFireWave->GetWaveSize();
	pFireWave->Reset();

	hRet = pFire->Lock( 0, dwWaveSize, &pPtr1, &dwSize1, &pPtr2, &dwSize2, 0L);
	if (FAILED(hRet))
		return (FALSE);
	pFireWave->Read((PBYTE)pPtr1,dwWaveSize);
	pFire->Unlock( pPtr1, dwWaveSize, NULL, 0 );




	dwWaveSize=pBoomWave->GetWaveSize();
	pBoomWave->Reset();

	hRet = pBoom->Lock( 0, dwWaveSize, &pPtr1, &dwSize1, &pPtr2, &dwSize2, 0L);
	if (FAILED(hRet))
		return (FALSE);
	pBoomWave->Read((PBYTE)pPtr1,dwWaveSize);
	pBoom->Unlock( pPtr1, dwWaveSize, NULL, 0 );




	dwWaveSize=pCaputWave->GetWaveSize();
	pCaputWave->Reset();

	hRet = pCaput->Lock( 0, dwWaveSize, &pPtr1, &dwSize1, &pPtr2, &dwSize2, 0L);
	if (FAILED(hRet))
		return (FALSE);
	pCaputWave->Read((PBYTE)pPtr1,dwWaveSize);
	pCaput->Unlock( pPtr1, dwWaveSize, NULL, 0 );


	dwWaveSize=pHitWave->GetWaveSize();
	pHitWave->Reset();

	hRet = pHit->Lock( 0, dwWaveSize, &pPtr1, &dwSize1, &pPtr2, &dwSize2, 0L);
	if (FAILED(hRet))
		return (FALSE);
	pHitWave->Read((PBYTE)pPtr1,dwWaveSize);
	pHit->Unlock( pPtr1, dwWaveSize, NULL, 0 );

	return (TRUE);
}

//--------------------------------------------------------
// Воспроизводит выбранный звук
//--------------------------------------------------------
void PlaySoundBuffer(int iSound)
{
	if(fUseSound)
	{
		if (iSound==FIRE_SOUND)
		{
			pFire->SetCurrentPosition(0);
			pFire->Play(0, 0,0);
		}
		if (iSound==BOOM_SOUND)
		{
			pBoom->SetCurrentPosition(0);
			pBoom->Play(0, 0, 0);
		}

		if (iSound==CAPUT_SOUND)
		{
			pCaput->SetCurrentPosition(0);
			pCaput->Play(0, 0, 0);
		}

		if (iSound==HIT_SOUND)
		{
			pHit->SetCurrentPosition(0);
			pHit->Play(0, 0, 0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize)
{
    DWORD *pdw;
    DWORD *pdwEnd;
    DWORD dwRiff;
    DWORD dwType;
    DWORD dwLength;

    if (ppWaveHeader)
    *ppWaveHeader = NULL;

    if (ppbWaveData)
    *ppbWaveData = NULL;

    if (pcbWaveSize)
    *pcbWaveSize = 0;

    pdw = (DWORD *)pvRes;
    dwRiff = *pdw++;
    dwLength = *pdw++;
    dwType = *pdw++;

    if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
    goto exit;      // not even RIFF

    if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
    goto exit;      // not a WAV

    pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

    while (pdw < pdwEnd)
    {
    dwType = *pdw++;
    dwLength = *pdw++;

    switch (dwType)
    {
    case mmioFOURCC('f', 'm', 't', ' '):
        if (ppWaveHeader && !*ppWaveHeader)
        {
        if (dwLength < sizeof(WAVEFORMAT))
            goto exit;      // not a WAV

        *ppWaveHeader = (WAVEFORMATEX *)pdw;

        if ((!ppbWaveData || *ppbWaveData) &&
            (!pcbWaveSize || *pcbWaveSize))
        {
            return TRUE;
        }
        }
        break;

    case mmioFOURCC('d', 'a', 't', 'a'):
        if ((ppbWaveData && !*ppbWaveData) ||
        (pcbWaveSize && !*pcbWaveSize))
        {
        if (ppbWaveData)
            *ppbWaveData = (LPBYTE)pdw;

        if (pcbWaveSize)
            *pcbWaveSize = dwLength;

        if (!ppWaveHeader || *ppWaveHeader)
            return TRUE;
        }
        break;
    }

    pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
    }

exit:
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize)
{
    if (pDSB && pbWaveData && cbWaveSize)
    {
    LPVOID pMem1, pMem2;
    DWORD dwSize1, dwSize2;

    if (SUCCEEDED(IDirectSoundBuffer_Lock(pDSB, 0, cbWaveSize,
        &pMem1, &dwSize1, &pMem2, &dwSize2, 0)))
    {
        CopyMemory(pMem1, pbWaveData, dwSize1);

        if ( 0 != dwSize2 )
        CopyMemory(pMem2, pbWaveData+dwSize1, dwSize2);

        IDirectSoundBuffer_Unlock(pDSB, pMem1, dwSize1, pMem2, dwSize2);
        return TRUE;
    }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
// DSGetWaveResource
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize)
{
    HRSRC hResInfo;
    HGLOBAL hResData;
    void *pvRes;

    if (((hResInfo = FindResource(hModule, lpName, c_szWAV)) != NULL) &&
    ((hResData = LoadResource(hModule, hResInfo)) != NULL) &&
    ((pvRes = LockResource(hResData)) != NULL) &&
    DSParseWaveResource(pvRes, ppWaveHeader, ppbWaveData, pcbWaveSize))
    {
    return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//
// DSLoadSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////

IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, LPCTSTR lpName)
{
    IDirectSoundBuffer *pDSB = NULL;
    DSBUFFERDESC dsBD = {0};
    BYTE *pbWaveData;

    if (DSGetWaveResource(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData, &dsBD.dwBufferBytes))
    {
        dsBD.dwSize  = sizeof(dsBD);
        dsBD.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | 
                       DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2;

        if (SUCCEEDED(IDirectSound_CreateSoundBuffer(pDS, &dsBD, &pDSB, NULL)))
        {
            if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
            {
            IDirectSoundBuffer_Release(pDSB);
            pDSB = NULL;
            }
        }
        else 
		{

			pDSB = NULL; 
		}
    }

    return pDSB;
}

///////////////////////////////////////////////////////////////////////////////
//
// DSReloadSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSReloadSoundBuffer(IDirectSoundBuffer *pDSB, LPCTSTR lpName)
{
    BOOL result=FALSE;
    BYTE *pbWaveData;
    DWORD cbWaveSize;

    if (DSGetWaveResource(NULL, lpName, NULL, &pbWaveData, &cbWaveSize))
    {
		if (SUCCEEDED(IDirectSoundBuffer_Restore(pDSB)) && DSFillSoundBuffer(pDSB, pbWaveData, cbWaveSize))
		{
			result = TRUE;
		}
    }
    return result;
}

void FadeOut()
{
//	SetVolume(Volume--);
}