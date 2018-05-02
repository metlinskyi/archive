//---------------------------------------------------------
//	Файл:		CWAVE.CPP
//	Описание:	Класс для чтения файлов формата WAV
//---------------------------------------------------------

#include <windows.h>
#include "cwave.h"

#define	WAV_HEADER_SIZE		44 // Полный размер заголовка WAV
#define	WAVEFORMATEX_SHIFT	20 // Смещение от начала файла до структуры WAVEFORMATEX

CMappedWave::CMappedWave()
{
	//Обнуляем все члены класса
	pbWaveFile=NULL;
	pWaveHeader=NULL;
	dwCurPosition=0;
	dwDataLength=0;
}


CMappedWave::~CMappedWave()
{
	//Закрываем файл, если он был открыт
	Close();
}


BOOL CMappedWave::Open(CHAR* szFilename)
{
	//Закрываем файл, если он был открыт
	Close();

	HANDLE	hFile,hMappedFile;
	DWORD	dwFileLength;
	
	//Пытаемся открыть файл
	hFile=CreateFile(szFilename,GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,NULL,NULL);
	if (hFile==INVALID_HANDLE_VALUE)
		return (FALSE);
	
	//Определяем размер файла
	dwFileLength=GetFileSize(hFile,NULL);

	//Создаём объект - проекция файла
	hMappedFile=CreateFileMapping(hFile,NULL,PAGE_READONLY,
		0,dwFileLength,NULL);
	
	//Закрываем описатель файла (но не сам файл!!!)
	CloseHandle(hFile);
	
	if (!hMappedFile)
		return (FALSE);
	
	//Проецируем файл на адресное пространство
	pbWaveFile=(PBYTE)MapViewOfFile(hMappedFile,FILE_MAP_READ,0,0,0);
	
	//Закрываем описатель проекции файла
	CloseHandle(hMappedFile);

	if (!pbWaveFile)
		return (FALSE);
	
	//Инициализируем оставшиеся члены класса
	pWaveHeader=(LPWAVEFORMATEX)(pbWaveFile+WAVEFORMATEX_SHIFT);
	dwCurPosition=WAV_HEADER_SIZE;
	dwDataLength=*((DWORD*)(pbWaveFile+WAV_HEADER_SIZE-sizeof(DWORD)));

	return (TRUE);
}

void CMappedWave::Reset()
{
	//Сбрасываем текущую позицию в значение по умолчанию
	if (pbWaveFile)
		dwCurPosition=WAV_HEADER_SIZE;
}

void CMappedWave::Close()
{
	if (pbWaveFile)
	{
		//Отключаем проекцию файла от адресного пространства
		UnmapViewOfFile(pbWaveFile);
		pbWaveFile=NULL;
	}
	
	//Обнуляем члены класса
	pWaveHeader=NULL;
	dwDataLength=0;
	dwCurPosition=0;

}

DWORD CMappedWave::Read(BYTE* pbData,DWORD dwSizeToRead)
{
	DWORD dwRemainder;

	//Если файл не был открыт, либо все данные прочитаны
	//возвращаем 0 - больше делать нечего
	if ((!pbWaveFile)||((dwDataLength+WAV_HEADER_SIZE)==dwCurPosition))
		return (0);

	if (dwSizeToRead<=(dwDataLength+WAV_HEADER_SIZE-dwCurPosition))
	{
		//Если достаточно данных для чтения
		CopyMemory(pbData,pbWaveFile+dwCurPosition,dwSizeToRead);
		dwCurPosition+=dwSizeToRead;
		dwRemainder=dwSizeToRead;
	}
	else //Если данных недостаточно
	{
		//Копируем сколько есть
		dwRemainder=dwDataLength-dwCurPosition+WAV_HEADER_SIZE;
		CopyMemory(pbData,pbWaveFile+dwCurPosition,dwRemainder);
        
		//Заполняем остаток молчанием
		FillMemory(pbData+dwRemainder,dwSizeToRead - dwRemainder, 
                (pWaveHeader->wBitsPerSample==8) ? 128 : 0);

		dwCurPosition+=dwRemainder;
	}
	return (dwRemainder);
}

DWORD CMappedWave::GetWaveSize()
{
	//Возвращаем размер только звуковых данных - без заголовка
	if (pbWaveFile)
		return (dwDataLength);
	else
		return (0);
}

