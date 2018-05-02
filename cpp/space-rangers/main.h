//------------------------------------------------------
//	Файл:		MAIN.H
//	Описание:	Главный заголовочный файл Rengers DX
//------------------------------------------------------
/****** Макросы общего назначения ************************************/
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define RAND_INT(x) (rand()*x/RAND_MAX)
#define RANDOM_VELOCITY() ((RAND_INT(5)+3)*2)
#define KEYDOWN(name,key) (name[key] & 0x80) 
#define INIT_DIRECTDRAW_STRUCT(x) (ZeroMemory(&x, sizeof(x)), x.dwSize=sizeof(x))
#define IS_NUM(c)     ((c) >= '0' && (c) <= '9')
#define IS_SPACE(c)   ((c) == ' ' || (c) == '\r' || (c) == '\n' || (c) == '\t' || (c) == 'x')

/****** Файл MAIN.CPP **********************************************/
#define ClassName	"Rangers DX"
#define AppName		"Rangers DX"

extern HWND	hMainWnd;
/****** Файл DRAW.CPP ***********************************************/
#define MAX_HEIGHT          768
#define MAX_WIDTH           1024
#define COLOR_DEPTH         8
#define TRASPARENT_COLOR    0x0
#define DEBUG				0


#define MOUSE_HEIGHT		32
#define MOUSE_WIDTH			28

#define MAX_STAR_DEEP		256
#define MAX_STAR_S			128
#define MAX_STAR_M			64
#define MAX_STAR_L			32
#define MUL_STAR_DEEP		4
#define MUL_STAR_S			8
#define MUL_STAR_M			16
#define MUL_STAR_L			32

#define STREAM_ENEMI_L		1
#define STREAM_ENEMI_M		2
#define STREAM_ENEMI_S		3

#define SHIP_SPEED			10
#define SHIP_TYPE_F			1
#define SHIP_TYPE_G			2
#define SHIP_TYPE_H			3
#define SHIP_TYPE_M			4
#define SHIP_TYPE_P			5
#define SHIP_TYPE_K			6
#define	SHIP_TYPE_DEF		SHIP_TYPE_H
#define	SHIP_MAX_G			8

#define MAX_ACCELERATION	5

#define MAX_ENEMI_S			32
#define MAX_ENEMI_M			16
#define MAX_ENEMI_L			16

#define MUL_ENEMI_S			32
#define MUL_ENEMI_M			64
#define MUL_ENEMI_L			64

#define ID_DELETE			0
#define ID_ENEMI_S			1
#define ID_ENEMI_M			2
#define ID_ENEMI_L			3

#define	BULLET_SPEED		10
#define WEAPON_POWER		4

#define FPS_HEIGHT			35
#define FPS_WIDTH			60
#define	ACCOUNT_WIDTH		100	
#define	ACCOUNT_HEIGHT		35

#define	MAX_LEVEL			16
#define	LOGO_SLEEP			0
#define	GAME_SLEEP			0




enum	SurfaceLocation { SYSTEM = 1, VIDEO, BOTH };
extern	SurfaceLocation fSurfLoc;

void	ErrorHandle(HWND hwnd, LPCTSTR szError);
BOOL	InitDirectDraw ();
void	RemoveDirectDraw ();
void	DrawFrame(void);
void	NewGame();
void	MoveShip(bool up,bool down, bool left, bool rigth,bool fire,bool accsel);
void	MoveMouse(int x,int y, int scroll,bool lb, bool rb,bool cb);
void	Logo();
void	InitStar();
void 	InitLogo();
void	InitSin_CosTables(void);
void	HoleInit();

extern	BYTE	fasa;
/****** Файл INPUT.CPP **********************************************/

BOOL	InitDirectInput ();
void	RemoveDirectInput();
void	AccessMouse(BOOL isActive);
void	AccessKeyboard(BOOL isActive);
void	ProcessMouse(BOOL isButton=TRUE);
void	ProcessKeyboard(BOOL canShuffle=TRUE);
void	SleepGame(DWORD dwMilliseconds);



/****** Файл FONT.CPP **********************************************/
#define SCORE_FONT	2
#define FPS_FONT	1

BOOL	InitFonts();
void	RemoveFonts();
void	ShadowTextOut(HDC hdc, BYTE font, int x, int y,LPCSTR string);

/****** Файл SOUND.CPP **********************************************/
#define HIT_SOUND	5
#define CLICK_SOUND	4
#define CAPUT_SOUND	3
#define BOOM_SOUND	2
#define FIRE_SOUND	1

extern BOOL fUseSound;
extern BOOL fUseMusic;
extern bool	bLogo;
extern bool	bLogo;
extern char	ver[8];
extern LONG Volume;

BOOL	InitDirectSound ();
void	RemoveDirectSound();
void	PlaySoundBuffer(int iSound);
void	FadeOut();



/****** Файл MUSIC.CPP **********************************************/

#define TITLE_MUSIC		1
#define MENU_MUSIC		2
#define LEVEL_MUSIC		3
#define BOSS_MUSIC		4

enum	MusicType { DM_FULL = 1, DM_LIGHT, MCI };
extern	MusicType fMusic;

BOOL	InitDirectMusic();
void	RemoveDirectMusic();
void	PlayMusic(int iMusic);

extern	char current_dir[MAX_PATH];
extern  WCHAR wszHomeDir[MAX_PATH];
extern  WCHAR wszLevelDir[MAX_PATH];
extern	bool bExit;
extern	bool MouseLock;


void PlayLevel(BYTE number);


/****** Файл GAME.CPP **********************************************/
int			rndInt( int low, int high );
double		rndDouble( double low, double high );

unsigned long 	GetNewID();

extern double	SinTable[360];
extern double	CosTable[360];
extern BYTE CurrentLevel;


void	RemoveGame();
int getint(char**p);

#define ID_BLASTER	666