//---------------------------------------------------------
//	Файл:		CWAVE.H
//	Описание:	Класс для чтения файлов формата WAV
//---------------------------------------------------------

#ifndef MAPPED_WAVE_H
#define MAPPED_WAVE_H


class CMappedWave
{
protected:
	BYTE*	pbWaveFile;
	LPWAVEFORMATEX pWaveHeader;

	DWORD dwCurPosition;
	DWORD dwDataLength;

public:
	CMappedWave();
	~CMappedWave();

	BOOL Open( CHAR* szFilename );
	void Reset();
	DWORD Read(BYTE* pbData, DWORD dwSizeToRead);
	void Close();

	DWORD	GetWaveSize();
	LPWAVEFORMATEX GetWaveHeader() { return pWaveHeader; }
	
};

#endif MAPPED_WAVE_H