/****** Макросы общего назначения ************************************/
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define RAND_INT(x) (rand()*x/RAND_MAX)
#define RANDOM_VELOCITY() ((RAND_INT(5)+3)*2)
#define KEYDOWN(name,key) (name[key] & 0x80) 
#define INIT_DIRECTDRAW_STRUCT(x) (ZeroMemory(&x, sizeof(x)), x.dwSize=sizeof(x))

/****** Файл MAIN.CPP **********************************************/
#define ClassName	"Hole DX"
#define AppName		"Hole DX"

extern HWND	hMainWnd;
/****** Файл DRAW.CPP ***********************************************/
#define MAX_HEIGHT          768
#define MAX_WIDTH           1024
#define COLOR_DEPTH         8
#define TRASPARENT_COLOR    0x0

#define MOUSE_HEIGHT		24
#define MOUSE_WIDTH			28
#define DEBUG				1	
#define MAX_POINT			5	

#define COLOR_RED			1	
#define COLOR_GREEN			2	
#define COLOR_BLUE			3	

#define CONTROL_MAX_POINT	0
#define CONTROL_CURRENT		1	
#define CONTROL_LOOP		2
#define CONTROL_A			3			
#define CONTROL_B			4	
#define CONTROL_A_K			5			
#define CONTROL_B_K			6	
#define CONTROL_HI_R		7			
#define CONTROL_LOW_R		8	
#define CONTROL_ROUND_MAX	9			
#define CONTROL_ROUND_MIN	10	
#define CONTROL_COLOR		11	
#define CONTROL_OSX			12	
#define CONTROL_OSY			13
#define CONTROL_TANX		14	
#define CONTROL_TANY		15
#define CONTROL_VAR			16
#define CONTROL_FPS			17
	
#define	MAX_CONTROL			18



enum	SurfaceLocation { SYSTEM = 1, VIDEO, BOTH };
extern	SurfaceLocation fSurfLoc;

void	ErrorHandle(HWND hwnd, LPCTSTR szError);
BOOL	InitDirectDraw ();
void	RemoveDirectDraw ();
void	DrawFrame(void);

void	MoveMouse(int x,int y, int scroll,bool lb, bool rb,bool cb);
void	Press(bool sp, bool f1,bool f2);


/****** Файл INPUT.CPP **********************************************/

BOOL	InitDirectInput ();
void	RemoveDirectInput();
void	AccessMouse(BOOL isActive);
void	AccessKeyboard(BOOL isActive);
void	ProcessMouse(BOOL isButton=TRUE);
void	ProcessKeyboard(BOOL canShuffle=TRUE);
void	SleepGame(DWORD dwMilliseconds);


void InitEditor();

struct OBJECT
{
	int x,y;
	int h,w;
	int cx,cy;
	int speed;
	bool visible;
	RECT pic;
	char name[16];

	int a,b;
	int radius_hi;
	int radius_low;
	int round_max;
	int round_min;

	int ka,kb;
	int angle;
	int loop;
	int color;

	float osx;
	float osy;

	float tanx;
	float tany;
};



struct RGB
{
	BYTE red;
	BYTE green;
	BYTE blue;
};


