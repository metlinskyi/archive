#define STR_SIZE 32

typedef struct 
{
	int		nWidth;
	int		nHeight;
	bool	bFullScreen;

	char	szPlayerName[STR_SIZE];	
	char	szSessionName[STR_SIZE];

	char	strDXVer[STR_SIZE];
	char	strProcessor[STR_SIZE];
	char	strWindows[STR_SIZE];

	
	bool	bMMX;
	bool	bSSE1;
	bool	bSSE2;
	bool	b3DNOW;

	bool	bATI;
	bool	bNVIDIA;

} SETUPINFO, *LPSETUPINFO;

extern SETUPINFO Info;

bool Setup(HINSTANCE);