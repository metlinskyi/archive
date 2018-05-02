#define PLANETS_COUNT	11

#define SUN				0	
#define MERCUR			1	
#define VENUS			2	
#define EARTH			3	
#define MOON			4	
#define MARS			5
#define JUPITER			6
#define SATURN			7
#define URAN			8
#define NEPTUN			9
#define PLUTON			10

#define STAR_OBJECT_COUNT		1024

#define MODE_UP			0
#define MODE_CABIN		1
#define MODE_BACK		2
#define MODE_FREE		3

#define SFX_MAX_EXP		4
#define SHIP_AXIL		100.0f

#define DIS_FAR			700.0f
#define DIS_NEAR		500.0f

#define SPACE_SIZE		1000.0f
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             pD3D;	 // Used to create the D3DDevice
LPDIRECT3DDEVICE9       pDevice; // Our rendering device

DWORD					dwWidth; 
DWORD					dwHeight;
float					fWidth; 
float					fHeight;
float					fHalfWidth; 
float					fHalfHeight;

//-----------------------------------------------------------------------------
// Global MESH 
//-----------------------------------------------------------------------------
LPD3DXMESH				pMeshPlanet		= NULL;
LPD3DXMESH				pMeshEnemy		= NULL;
LPD3DXMESH				pMeshShip		= NULL;
LPD3DXMESH    			pMeshLaser		= NULL;  
LPD3DXMESH    			pMeshSonic[4]; 
LPD3DXMESH    			pMeshSonic2[4]; 

// Effect
LPD3DXEFFECT            pEffect;

// Global Object 
VECTOR3					objStar[STAR_OBJECT_COUNT];
CSHIP					objShip;
CSHIP					objEnemy;
CPLANET					objPlanets[PLANETS_COUNT];
D3DMATERIAL9*           pPlanetsMaterials = NULL; // Materials for our mesh
D3DMATERIAL9*           pShipMaterials = NULL; // Materials for our mesh


//  Global Font
CD3DFont*				pFont;
BYTE					bAlfa;
bool					bRGB;

// Variables for timing
float					fTime;            // Current time in seconds
float					fTimeLapsed;	// Time elapsed since last frame
FLOAT					fFPS;             // Instanteous frame rate
TCHAR					strFPS[90];		// String to hold frame stats
DWORD					Time ;
DWORD					Elapsed;

//  Global Matrix 
D3DXMATRIXA16			matWorld;
D3DXMATRIXA16			matTemp;
D3DXMATRIXA16			matView;
D3DXMATRIXA16			matProj;

// Control Panel
UserInput				Input;

// Camera
D3DXVECTOR3				vEyePt;
D3DXVECTOR3				vLookatPt;
D3DXVECTOR3				vUpVec;

// Guide texture and sprite interface
IDirect3DTexture9		*GuideTexture = NULL;
ID3DXSprite				*Guide        = NULL;
D3DXVECTOR2				vGuide;


// Sound FX Object
CMusicManager*			pDXMusic;				// DirectMusic manager class
CMusicSegment*			pSfxSonic;				// Sonic sound
CMusicSegment*			pSfxLaser;				// Laser sound
CMusicSegment*			pSfxExp[SFX_MAX_EXP];	// Laser sound


// Global Temporary index
DWORD					item;
int						index;


CBULLET					objBullet;
CBULLET					objLaser[2];