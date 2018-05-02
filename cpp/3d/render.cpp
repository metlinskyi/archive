//-----------------------------------------------------------------------------
// File: render.cpp
//
// Desc: DirectX window application 
//-----------------------------------------------------------------------------
#define STRICT

//#include <xmmintrin.h>
//#include <emmintrin.h>
#include "dx/d3d.h"
#include "dx/d3dfont.h"
#include "dx/dxutil.h"
#include "dx/dmutil.h"
#include "dx/d3dutil.h"
#include "dx/dxinput.h"
#include "dx/particles.h"
#include "class/cobject3d.h"
#include "class/cplanet.h"
#include "class/cbullet.h"
#include "class/cenemy.h"
#include "class/cship.h"
#include "class/cstack.h"

#include "render.h"
#include "global.h"
#include "math3d.h"
#include "mp3.h"
#include "res.h"


//-----------------------------------------------------------------------------
// Prototype function
//-----------------------------------------------------------------------------
cParticleEmitter ShipEngine;

inline void Render();
inline void Update();
inline void Control();

void PlanetsInit();
void ShipInit();
void Environment();

// Robot mesh object
//D3DXMESHCONTAINER_EX *g_ShipMesh = NULL;

//-----------------------------------------------------------------------------
// Name: Install()
// Desc: Inicializi all object
//-----------------------------------------------------------------------------
bool Install(HWND hWnd,int Width, int Height,bool mode,int engine)
{
	dwWidth=Width;
	dwHeight=Height;
	fWidth=(float)Width;
	fHeight=(float)Height;
	fHalfWidth=(float)Width/2; 
	fHalfHeight=(float)Height/2;
	vGuide=D3DXVECTOR2(fHalfWidth-8.0f,fHalfHeight-8.0f);

	// Initialize Direct3D
	if (FAILED(InitD3D(&pD3D, &pDevice,hWnd,Width,Height,32,mode)))
	{
		Error("Initialize Direct3D");
		return false;
	}

//	LoadMesh(&g_ShipMesh, pDevice, "..\\_hyper\\ship.x", "..\\_hyper\\");

	// Initialize Direct Input
	if (!InitInput(hWnd ))
	{
		Error("Initialize Direct Input");
		return false;
	}

	ZeroMemory( &Input, sizeof(UserInput) );

	// Initialize Direct Music
	if (!InitAudio(&pDXMusic,hWnd ))
	{
		Error("Initialize Direct Music");
		return false;
	}

    // Create fonts
	pFont = new CD3DFont( _T("Arial"), 8, D3DFONT_BOLD );
    if( pFont == NULL )
	{
		Error("Create fonts");
        return false;
	}

    if( FAILED( pFont->InitDeviceObjects(pDevice ) ) )
        return false;

	pFont->RestoreDeviceObjects();

	//Init PRIMITIV

	// Create new object Laser
    if (FAILED(D3DXCreateCylinder(pDevice, 0.005f, 0.005f, 2.0f, 4, 2, &pMeshLaser, NULL)))
        return false;

	// Create new object Sonic 
    if (FAILED(D3DXCreateSphere(pDevice, 0.5f, 24, 24, &pMeshSonic[0], NULL)))
       return false;
		// Create new object Sonic 
    if (FAILED(D3DXCreateSphere(pDevice, 0.4f, 6, 6, &pMeshSonic[1], NULL)))
       return false;
		// Create new object Sonic
    if (FAILED(D3DXCreateSphere(pDevice, 0.3f, 4, 4, &pMeshSonic[2], NULL)))
       return false;
		// Create new object Sonic
    if (FAILED(D3DXCreateSphere(pDevice, 0.2f, 3, 3, &pMeshSonic[3], NULL)))
       return false;


		// Create new object Sonic 2 
    if (FAILED(D3DXCreateSphere(pDevice, 0.07f,8, 8, &pMeshSonic2[0], NULL)))
       return false;
		// Create new object Sonic  2
    if (FAILED(D3DXCreateSphere(pDevice, 0.05f, 6, 6, &pMeshSonic2[1], NULL)))
       return false;
		// Create new object Sonic 2
    if (FAILED(D3DXCreateSphere(pDevice, 0.04f, 4, 4, &pMeshSonic2[2], NULL)))
       return false;
		// Create new object Sonic 2
    if (FAILED(D3DXCreateSphere(pDevice, 0.03f, 3, 3, &pMeshSonic2[3], NULL)))
       return false;

	for (int i=0;i<4;i++)
	{
		pMeshSonic[i]->OptimizeInplace(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
		pMeshSonic2[i]->OptimizeInplace(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
	}
//////////////////////////////// LOAD RESOURCE DLL /////////////////////////////////////////

	// Load Resource
    HMODULE	hResource = LoadLibrary("resource.dll");
	if (hResource==NULL)
	{
		Error("Load Library (resource.dll)");
		return false;
	}

    //load the sounds from resources (or files)
    if (FAILED(pDXMusic->CreateSegmentFromResource( &pSfxSonic, _T("SFX_SONIC"), _T("WAVE"),hResource)))
	{
		Error("Load Sound FX (sonic)");
		return false;
	}

	if (FAILED(pDXMusic->CreateSegmentFromResource( &pSfxLaser, _T("SFX_LASER"), _T("WAVE"),hResource)))
	{
		Error("Load Sound FX (laser)");
		return false;
	}

	if (FAILED(pDXMusic->CreateSegmentFromResource( &pSfxExp[0], _T("SFX_EXP0"), _T("WAVE"),hResource)))
	{
			Error("Load Sound FX (exp0)");
			return false;
	}

	if (FAILED(pDXMusic->CreateSegmentFromResource( &pSfxExp[1], _T("SFX_EXP1"), _T("WAVE"),hResource)))
	{
			Error("Load Sound FX (exp2)");
			return false;
	}

	if (FAILED(pDXMusic->CreateSegmentFromResource( &pSfxExp[2], _T("SFX_EXP2"), _T("WAVE"),hResource)))
	{
			Error("Load Sound FX (exp3)");
			return false;
	}

	if (FAILED(pDXMusic->CreateSegmentFromResource( &pSfxExp[3], _T("SFX_EXP3"), _T("WAVE"),hResource)))
	{
			Error("Load Sound FX (exp3)");
			return false;
	}


    LPD3DXBUFFER	pD3DXMtrlBuffer;

	// Loda Solar System  ---------------------------------------------------------------------
    if( FAILED( D3DXLoadMeshFromXResource( hResource,MAKEINTRESOURCE(MDL_SHPERE),"XFILES",
		D3DXMESH_SYSTEMMEM,	pDevice,NULL,&pD3DXMtrlBuffer,	NULL,&objPlanets[0].NumMat,	&pMeshPlanet)))
	{
		Error("Load SHPERE (resource.dll)");
        return false;
	}

	PlanetsInit();

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    pPlanetsMaterials = new D3DMATERIAL9[objPlanets[0].NumMat];

    for( DWORD index=0; index<PLANETS_COUNT; index++ )
    {
		objPlanets[index].texture=new LPDIRECT3DTEXTURE9[objPlanets[0].NumMat];
	}

	for( DWORD i=0; i<objPlanets[SUN].NumMat; i++ )
	{
		// Copy the material
		pPlanetsMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		pPlanetsMaterials[i].Ambient = pPlanetsMaterials[i].Diffuse;

		for( DWORD index=0; index<PLANETS_COUNT; index++ )
		{
			if( FAILED( D3DXCreateTextureFromFile(pDevice,(LPCTSTR)objPlanets[index].name,&objPlanets[index].texture[i])))
			{			
				if(!CreateTextureFromResource(pDevice,(LPCTSTR)objPlanets[index].name,&objPlanets[index].texture[i],hResource))
				{
					Error("Load texture planets");
					return false;

				}
			}
		}
	}
	pMeshPlanet->OptimizeInplace(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
	pD3DXMtrlBuffer->Release();


	// Load Ship ---------------------------------------------------------------------
    LPD3DXBUFFER	pMtrlBuffer=NULL;
	LPD3DXBUFFER    AdjacencyBuffer = NULL;

	ZeroMemory(&objShip.name, 32);
	strcpy(objShip.name,"SHIP");

	if( FAILED(D3DXLoadMeshFromXResource( hResource,MAKEINTRESOURCE(MDL_SHIP),"XFILES",
		D3DXMESH_SYSTEMMEM,	pDevice,&AdjacencyBuffer,&pMtrlBuffer,NULL,&objShip.NumMat,&pMeshShip)))
	{
		Error("Load SHIP (resource.dll)");
        return false;
	}

	D3DXMATERIAL* pDXMaterials = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
	pShipMaterials =new D3DMATERIAL9[objShip.NumMat];
	objShip.pTexture = new LPDIRECT3DTEXTURE9[objShip.NumMat];

	for( DWORD i=0; i<objShip.NumMat; i++ )
	{
		// Copy the material
		pShipMaterials[i] = pDXMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		pShipMaterials[i].Ambient = pShipMaterials[i].Diffuse;

		objShip.pTexture[i] = NULL;

		// Create the texture
		if( FAILED( D3DXCreateTextureFromFile(pDevice,(LPCTSTR)objShip.name,&objShip.pTexture[i])))
		{
			if(!CreateTextureFromResource(pDevice,(LPCTSTR)objShip.name,&objShip.pTexture[i],hResource))
			{
				Error("Load texture ship");
				return false;
			}
		}
	}
	pMtrlBuffer->Release();

	pMeshShip->OptimizeInplace(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);


	// Load Enemy
    if( FAILED( D3DXLoadMeshFromXResource( hResource,MAKEINTRESOURCE(MDL_ENEMY2),"XFILES",
		D3DXMESH_SYSTEMMEM,	pDevice,NULL,&pD3DXMtrlBuffer,	NULL,&objEnemy.NumMat,	&pMeshEnemy)))
	{
		Error("Load ENEMY (resource.dll)");
        return false;
	}

	pMeshEnemy->OptimizeInplace(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);

	FreeLibrary(hResource);

	ShipInit();
//////////////////////////////// END LOAD RESOURCE DLL /////////////////////////////////////////

	// Create GUI 
	if(!CreateTextureFromResource(pDevice,"GUI_CURSOR",&GuideTexture))
	{
		Error("Create GUI (cursor)");
        return false;
	}

	D3DXCreateSprite(pDevice, &Guide);


	// Init MP3 Player
	if(!InitPlayer())
	{
		Error("Init MP3 Player (mp3.dll)");
        return false;
	}

	if (!Play("music/menu.mp3",true))
	{
		//Error("Load music file (menu.mp3)");
 	}

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

	Environment();

    // Initialize the application timer
    DXUtil_Timer( TIMER_START );

	return true;
}
//-----------------------------------------------------------------------------
// Name: UnInstall()
// Desc: Destroy all object
//-----------------------------------------------------------------------------
void UnInstall()
{
	int i;

	void Stop();
	void Clean();
	void CleanupDirectInput();

	pFont->DeleteDeviceObjects();

	ShipEngine.Free();

    if( pPlanetsMaterials != NULL ) 
        delete[] pPlanetsMaterials;

	for (i=0;i<PLANETS_COUNT;i++)
	{

	}

	for (i=0;i<4;i++)
	{
		ReleaseCOM(pMeshSonic[i]);
	}

	for (i=0;i<SFX_MAX_EXP;i++)
	{
		SAFE_DELETE(pSfxExp[i]);
	}

    SAFE_DELETE(pSfxSonic);
	SAFE_DELETE(pSfxLaser);
    SAFE_DELETE(pDXMusic);

	ReleaseCOM(pMeshLaser );
	ReleaseCOM( pMeshShip);
	ReleaseCOM(pMeshEnemy);
	ReleaseCOM(pMeshPlanet);
	ReleaseCOM(GuideTexture);
	ReleaseCOM(Guide);
    ReleaseCOM(pDevice) ;
    ReleaseCOM(pD3D);
}

//-----------------------------------------------------------------------------
// Name: Idle()
// Desc: Rendering Scene
//-----------------------------------------------------------------------------
void Idle()
{
    if( NULL == pDevice )return;

	// Keep track of the frame count
    static FLOAT fLastTime	= 0.0f;
    static DWORD dwFrames	= 0;

    fTime					= DXUtil_Timer( TIMER_GETABSOLUTETIME );
    fTimeLapsed				= DXUtil_Timer( TIMER_GETELAPSEDTIME );	// Get the time lapsed since the last frame

    ++dwFrames;

    // Update the scene stats once per second
    if( fTime - fLastTime > 1.0f )
    {
        fFPS    = dwFrames / (fTime - fLastTime);
        fLastTime = fTime;
        dwFrames  = 0;

		const int cchMaxFrameStats = sizeof(strFPS) / sizeof(TCHAR);
	    _sntprintf( strFPS, cchMaxFrameStats, _T("%.00f fps"), fFPS);
		strFPS[cchMaxFrameStats - 1] = TEXT('\0');
    }

	Control();

	Update();

    pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_RGBA(0,0,30,255), 1.0f, 0 );
    if( SUCCEEDED( pDevice->BeginScene() ) )
    {

		Render();

		// Show frame rate
        pFont->DrawText( 10, 10, D3DCOLOR_ARGB(bAlfa,0,255,20), strFPS );

		/*
		pFont->DrawText( 10,  30, D3DCOLOR_ARGB(bAlfa,0,255,60), "C - camera" );
        pFont->DrawText( 10,  40, D3DCOLOR_ARGB(bAlfa,0,255,80), "X - laser weapon" );
        pFont->DrawText( 10,  50, D3DCOLOR_ARGB(bAlfa,0,255,100), "Z - sonic weapon" );
        pFont->DrawText( 10,  60, D3DCOLOR_ARGB(bAlfa,0,255,120), "Esc - exit" );
  		pFont->DrawText( fWidth-99,  11, D3DCOLOR_ARGB(255,0,0,255), "Hyper 3D" );
		pFont->DrawText( fWidth-100,  10, D3DCOLOR_ARGB(255,255,0,0), "Hyper 3D" );
	    pFont->DrawText( fWidth-110, 31 , D3DCOLOR_ARGB(bAlfa,0,0,255), "reality@ua.fm" );	
		pFont->DrawText( fWidth-111,  30 , D3DCOLOR_ARGB(bAlfa,255,0,0), "reality@ua.fm" );
		*/
        pDevice->EndScene();
    }
    pDevice->Present( NULL, NULL, NULL, NULL );
}

bool Esc()
{
//	delete g_ShipMesh; g_ShipMesh = NULL;
	return true; 
}

void Environment()
{
	// Get the projection transformation for use later
	pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

   // Setup a material
    D3DMATERIAL9 mtrl;
    D3DUtil_InitMaterial( mtrl, 1.0f, 0.0f, 0.0f );
    pDevice->SetMaterial( &mtrl );

    // Set up the textures
    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    // Set miscellaneous render states
    pDevice->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    pDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    pDevice->SetRenderState( D3DRS_ZENABLE,        TRUE );
    pDevice->SetRenderState( D3DRS_AMBIENT,        0x000F0F0F );


	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE );
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
    pDevice->SetRenderState( D3DRS_AMBIENT,D3DCOLOR_COLORVALUE( 0.1, 0.1,0.1, 1.0 ) );

    // SUN the light
    D3DLIGHT9 light[2];

    ZeroMemory( &light[0], sizeof(light[0]) );
	D3DUtil_InitLight( light[0], D3DLIGHT_POINT, 0.0f, 0.0f, 0.0f );
	light[0].Attenuation0 = 0.05f;
	light[0].Attenuation1 = 0.0f;
	light[0].Attenuation2 = 0.0f;
	light[0].Phi =0.0f;
    light[0].Range      = 1000000.0f;
    pDevice->SetLight( 0, &light[0] );
    pDevice->LightEnable( 0, FALSE  );


    ZeroMemory( &light[1], sizeof(light[1]) );
	D3DUtil_InitLight( light[1], D3DLIGHT_POINT, 0.0f,	500.0f, 0.0f );
	light[1].Attenuation0 = 0.1f;
	light[1].Attenuation1 = 0.0f;
	light[1].Attenuation2 = 0.0f;
	light[1].Phi =0.0f;
    light[1].Range      = 1000000.0f;
    pDevice->SetLight( 1, &light[1] );
    pDevice->LightEnable( 1, FALSE );

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void PlanetsInit()
{
	float fAE		= 149.6f * 3.0f;
	float fSE		= 12742.0f /1000;
	float fRE		= 0.007f; 	
	float fStart	= 100000.0f;

    for(index=0; index<PLANETS_COUNT; index++ )
	{
		ZeroMemory(&objPlanets[index].name, 32);
		objPlanets[index].vVel.x=0.0f;
		objPlanets[index].vVel.y=0.0f;
		objPlanets[index].vVel.z=0.0f;	
		objPlanets[index].fSize=1.0f;
		objPlanets[index].fRadius=5.0f * index;
		objPlanets[index].fRate=0.1f;
		objPlanets[index].fPeriod=0.2f;
	}

	strcpy(objPlanets[SUN].name,		"SUN");
    strcpy(objPlanets[MERCUR].name,		"MERCUR");	
	strcpy(objPlanets[VENUS].name,		"VENUS");
	strcpy(objPlanets[EARTH].name,		"EARTH");
	strcpy(objPlanets[MOON].name,		"MOON");
	strcpy(objPlanets[MARS].name,		"MARS");
	strcpy(objPlanets[JUPITER].name,	"JUPITER");
	strcpy(objPlanets[SATURN].name,		"SATURN");
	strcpy(objPlanets[URAN].name,		"URAN");
	strcpy(objPlanets[NEPTUN].name,		"NEPTUN");	
	strcpy(objPlanets[PLUTON].name,		"PLUTON");	

	objPlanets[SUN].fSize			= fSE * 54.6f/20; 
    objPlanets[MERCUR].fSize		= fSE * 0.382f;
	objPlanets[VENUS].fSize			= fSE * 0.97f;
	objPlanets[EARTH].fSize			= fSE; 
	objPlanets[MARS].fSize			= fSE * 0.53f/3; 
	objPlanets[JUPITER].fSize		= fSE * 11.19f/3; 
	objPlanets[SATURN].fSize		= fSE * 9.1f; 
	objPlanets[URAN].fSize			= fSE * 3.75f;
	objPlanets[NEPTUN].fSize		= fSE * 3.5f;
	objPlanets[PLUTON].fSize		= fSE * 0.5f;

    objPlanets[SUN].fRadius			= 0.1f; 
    objPlanets[MERCUR].fRadius		= fAE * 0.387f;
	objPlanets[VENUS].fRadius		= fAE * 0.723f;
	objPlanets[EARTH].fRadius		= fAE; 
	objPlanets[MARS].fRadius		= fAE * 1.524f; 
	objPlanets[JUPITER].fRadius		= fAE * 5.203f; 
	objPlanets[SATURN].fRadius		= fAE * 9.535f; 
	objPlanets[URAN].fRadius		= fAE * 19.18f;
	objPlanets[NEPTUN].fRadius		= fAE * 30.06f; 
	objPlanets[PLUTON].fRadius		= fAE * 39.75f;

	objPlanets[SUN].fRate			= 0.0f; 
    objPlanets[MERCUR].fRate		= fRE / 0.241f;
	objPlanets[VENUS].fRate			= fRE / 0.615f;
	objPlanets[EARTH].fRate			= fRE; 
	objPlanets[MARS].fRate			= fRE / 1.881f; 
	objPlanets[JUPITER].fRate		= fRE / 11.862f; 
	objPlanets[SATURN].fRate		= fRE / 29.457f; 
	objPlanets[URAN].fRate			= fRE / 84.015f;
	objPlanets[NEPTUN].fRate		= fRE / 164.782f; 
	objPlanets[PLUTON].fRate		= fRE / 250.6f;

	objPlanets[MOON].vVel			= objPlanets[EARTH].vPos;
	objPlanets[MOON].fRadius		= objPlanets[EARTH].fSize*3;
	objPlanets[MOON].fSize			= fSE * 0.34f;
	objPlanets[MOON].fRate			= fRE*27;

	for (index=1;index<PLANETS_COUNT;index++)
	{
		objPlanets[index].Move(fStart);
	}


	//STAR
	float a;
	float m;

	for (int i=0;i< STAR_OBJECT_COUNT;i++ )
	{
		if (i<STAR_OBJECT_COUNT/2)
		{
			a=fRnd(-PI,PI);
			m=fRnd(	objPlanets[MARS].fRadius+fAE, objPlanets[JUPITER].fRadius-fAE*2.0f);

			objStar[i].x-=sinf(a) * m;
			objStar[i].z-=cosf(a) * m;
			objStar[i].y=fRnd(-20.0f, 20.0f);
			objStar[i].null=nRnd(2,3);
		}
		else
		{
			objStar[i].null=3;
			objStar[i].x=fRnd(- SPACE_SIZE,  SPACE_SIZE);
			objStar[i].y=fRnd(- SPACE_SIZE/2, SPACE_SIZE/2);
			objStar[i].z=fRnd(- SPACE_SIZE, SPACE_SIZE);

			//ShipEngine.Add(nRnd(2,4),&D3DXVECTOR3(objStar[i].x,objStar[i].y,objStar[i].z),fRnd(1.0f,  5.0f),D3DCOLOR_RGBA(nRnd(200,255),nRnd(200,255),nRnd(200,255),10),50,&D3DXVECTOR3(0.0f, 0.0f, 0.0f)); 
		}
	}
	
    objPlanets[SUN].vVel.x=0.0f;
	objPlanets[SUN].vVel.y=0.0f;
	objPlanets[SUN].vVel.z=0.0f;

	//ShipEngine.Add(1,&D3DXVECTOR3(0.0f, 0.0f, 0.0f),450.0f,D3DCOLOR_RGBA(255,255,0,10), 50,&D3DXVECTOR3(0.0f, 0.0f, 0.0f)); 
	ShipEngine.Add(1,&D3DXVECTOR3(0.0f, 0.0f, 0.0f),objPlanets[SUN].fSize*7,D3DCOLOR_RGBA(255,255,0,10), 50,&D3DXVECTOR3(0.0f, 0.0f, 0.0f)); 
	//ShipEngine.Add(0,&D3DXVECTOR3(objPlanets[EARTH].vPos.x, objPlanets[EARTH].vPos.y, objPlanets[EARTH].vPos.z),50.3f,D3DCOLOR_RGBA(0,255,255,50), 50,&D3DXVECTOR3(0.0f, 0.0f, 0.0f)); 
}

void ShipInit()
{
	vUpVec= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	objShip.fSize=0.05f;
	objShip.vPos.x=objPlanets[EARTH].vPos.x;
	objShip.vPos.y=objPlanets[SUN].fSize+20.0f;
	objShip.vPos.z=objPlanets[EARTH].vPos.z;
	objShip.fMaxSpeed=objShip.fTmp=10.0f;
	objShip.nMode=0;
	objShip.fAngle=objShip.fMove=0;
	objShip.vPos.x -= sinf(objShip.fAngle) * objShip.fMove ;
	objShip.vPos.z -= cosf(objShip.fAngle) * objShip.fMove;
	objShip.bClose=false;

	objEnemy.fSize=0.06f;
	objEnemy.vPos.x=objPlanets[MERCUR].vPos.x;
	objEnemy.vPos.y=objPlanets[SUN].fSize+20.0f;
	objEnemy.vPos.z=objPlanets[MERCUR].vPos.z;
	objEnemy.fMaxSpeed=objEnemy.fTmp=10.0f;
	objEnemy.nMode=0;
	objEnemy.fAngle=objEnemy.fMove=0;
	objEnemy.vPos.x -= sinf(objEnemy.fAngle) * objEnemy.fMove ;
	objEnemy.vPos.z -= cosf(objEnemy.fAngle) * objEnemy.fMove;
	objEnemy.bClose=false;


	// Create the particle emitters
	ShipEngine.Create(pDevice, &D3DXVECTOR3( objShip.vPos.x,objShip.vPos.y, objShip.vPos.z ), EMITTER_CLOUD);
}

///////////////////////////////////// RENDERING /////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: Control()
// Desc: Controle Ship 
//-----------------------------------------------------------------------------
inline void Control()
{
	// Process the button data 
	UpdateInput(&Input);

	//Ускорение
	if(Input.bAxil )	objShip.fMaxSpeed=SHIP_AXIL;
	if (!Input.bAxil)	objShip.fMaxSpeed=objShip.fTmp;


	if(( Input.bLeft || Input.bRight ) && (objShip.fRoll<1.0f)) 
	{	
		objShip.fRoll+=0.05f;
	}

	if((!Input.bLeft && !Input.bRight) && (objShip.fRoll>0.0f))
	{
		objShip.fRoll-=0.01f;
	}

	if( Input.bLeft && !Input.bRight ) 
	{
		objShip.fAngle-= fTimeLapsed* objShip.fRoll ; 
	}

	if( Input.bRight && !Input.bLeft )
	{
		objShip.fAngle+= fTimeLapsed* objShip.fRoll ;
	}

	//	Вперед
    if( Input.bUp )
	{
		if ((objShip.fAxil>-5.0f) && !Input.bDown && objShip.bReversUp)
		{
			objShip.fAxil-=1.0f;
			objShip.fMove =-objShip.fAxil * fTimeLapsed;

			if (objShip.fAxil<0.0f)	objShip.bReversUp=false;			
		}

		if (!objShip.bReversUp && !Input.bDown)
		{
			if (objShip.fAxil<objShip.fMaxSpeed) objShip.fAxil+=1.0f;
			objShip.fMove =objShip.fAxil * fTimeLapsed;
			objShip.bReversDw =true;
		}
	}
	//	Задний ход
    if( Input.bDown  )
	{
		if ((objShip.fAxil>-5.0f) && !Input.bUp && objShip.bReversDw)
		{
			objShip.fAxil-=1.0f;
			objShip.fMove =objShip.fAxil * fTimeLapsed;

			if (objShip.fAxil<0.0f) objShip.bReversDw =false;
		}

		if (!objShip.bReversDw && !Input.bUp)
		{
			if (objShip.fAxil<objShip.fMaxSpeed) objShip.fAxil+=1.0f;
			objShip.fMove =-objShip.fAxil * fTimeLapsed;
			objShip.bReversUp=true;
		}
	}

	//Скольжение
    if( !Input.bUp && !Input.bDown  )
	{
		if (objShip.fAxil>0.0f) objShip.fAxil-=0.1f;

		if (objShip.fMove <0.0f)
			objShip.fMove =-objShip.fAxil * fTimeLapsed;
		else 
			objShip.fMove =objShip.fAxil * fTimeLapsed;

	}

	objShip.fAngleSin=sinf(objShip.fAngle);
	objShip.fAngleCos=cosf(objShip.fAngle);

	objShip.vPos.x -=objShip.fAngleSin  * objShip.fMove ;
	objShip.vPos.z -=objShip.fAngleCos  * objShip.fMove;

	// SONIC
    if( Input.bFirstWeapon && objShip.bReadySonic )
    {
		PlaySound(pSfxSonic);

		objBullet.New(objShip.vPos,objShip.fAngle,SONIC);
		objShip.bReadySonic=false;
	}

    if( (!Input.bFirstWeapon) && (!objShip.bReadySonic))
    {
		objShip.bReadySonic=true;
	}

	// LASER
	if( Input.bSecendWeapon && objShip.bReadyLaser)
    {
		PlaySound(pSfxLaser);
		objShip.bReadyLaser=false;

		VECTOR3 v3;
			
		v3=objShip.vPos;

		v3.x-=sinf(objShip.fAngle-PI_DIV_2)*0.25f;
		v3.y-=0.1f;
		v3.z-=cosf(objShip.fAngle-PI_DIV_2)*0.25f;

		objLaser[0].New(v3,objShip.fAngle,LASER);

		v3=objShip.vPos;

		v3.x-=sinf(objShip.fAngle+PI_DIV_2)*0.25f;
		v3.y-=0.1f;
		v3.z-=cosf(objShip.fAngle+PI_DIV_2)*0.25f;
		objLaser[1].New(v3,objShip.fAngle,LASER);
    }

    if( (!Input.bSecendWeapon) && (!objShip.bReadyLaser))
    {
		objShip.bReadyLaser=true;
	}

	//Смена вида
	if( Input.bCamera && objShip.bClose)
    {
		objShip.bClose=false;
		objShip.nMode++;
		if (objShip.nMode>3)
		{
			objShip.nMode=0;
		}
	}

	if( ! Input.bCamera )
	{
		objShip.bClose=true;
	}

}

float xx[4];
float yy[4];
float zz[4];
//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Update Scene
//-----------------------------------------------------------------------------
inline void Update()
{
	if(objShip.nMode==MODE_UP)//вид сверху
	{
		vEyePt		= D3DXVECTOR3(objShip.vPos.x, objShip.vPos.y+20.0f, objShip.vPos.z+0.1f );
		vLookatPt	= D3DXVECTOR3(objShip.vPos.x, 0.0f  ,objShip.vPos.z );
	}

	if(objShip.nMode==MODE_CABIN)//вид из кабины
	{
		vEyePt		= D3DXVECTOR3(objShip.vPos.x, objShip.vPos.y+0.3f, objShip.vPos.z);
		vLookatPt	= D3DXVECTOR3(objShip.vPos.x-objShip.fAngleSin*500.0f, objShip.vPos.y, objShip.vPos.z-objShip.fAngleCos*500.0f );
	}

	if(objShip.nMode==MODE_BACK)//вид сзади
	{
		vEyePt		= D3DXVECTOR3(objShip.vPos.x+objShip.fAngleSin*10.0f, objShip.vPos.y+10.0f , objShip.vPos.z+objShip.fAngleCos*10.0f );
		vLookatPt	= D3DXVECTOR3(objShip.vPos.x-objShip.fAngleSin*20.0f, objShip.vPos.y-10.0f, objShip.vPos.z-objShip.fAngleCos*20.0f );
    }

	if(objShip.nMode==MODE_FREE)//наблюдение
	{
		vLookatPt	= D3DXVECTOR3(objShip.vPos.x, objShip.vPos.y  ,objShip.vPos.z );
    }

    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    pDevice->SetTransform( D3DTS_VIEW, &matView );

    if( !Input.bDown  )
	{
		objShip.fEngineL=objShip.fAngle-0.2f;
		objShip.fEngineR=objShip.fAngle+0.2f;
	
		ShipEngine.HandleEngine(
			&D3DXVECTOR3(objShip.vPos.x+sinf(objShip.fEngineL)*0.4f,  objShip.vPos.y, objShip.vPos.z+cosf(objShip.fEngineL)*0.4f),
			&D3DXVECTOR3(objShip.vPos.x+sinf(objShip.fEngineR)*0.4f,  objShip.vPos.y, objShip.vPos.z+cosf(objShip.fEngineR)*0.4f),
			fTimeLapsed); 
	}


	objPlanets[SUN].fRoll-=0.003f ;
    objPlanets[MERCUR].Move(fTimeLapsed);
	objPlanets[VENUS].Move(fTimeLapsed);
	objPlanets[EARTH].Move(fTimeLapsed);
	objPlanets[MARS].Move(fTimeLapsed);
	objPlanets[JUPITER].Move(fTimeLapsed);
	objPlanets[SATURN].Move(fTimeLapsed);
	objPlanets[URAN].Move(fTimeLapsed);
	objPlanets[NEPTUN].Move(fTimeLapsed);
	objPlanets[PLUTON].Move(fTimeLapsed);

	objPlanets[MOON].vVel=	objPlanets[EARTH].vPos;
	objPlanets[MOON].fRoll=-objPlanets[EARTH].fRoll*2;
	objPlanets[MOON].Move(fTimeLapsed);


	if (bRGB) bAlfa--; else bAlfa++;
	if (bAlfa>254)bRGB=true; 
	if (bAlfa<1)bRGB=false; 

	xx[2]=xx[1];
	yy[2]=yy[1];
	zz[2]=zz[1];

	xx[1]=xx[0];
	yy[1]=yy[0];
	zz[1]=zz[0];

	xx[0]=objBullet.vPos.x;
	yy[0]=yy[0];
	zz[0]=objBullet.vPos.z;
	objBullet.Update(fTimeLapsed);

	objLaser[0].Update(fTimeLapsed);
	objLaser[1].Update(fTimeLapsed);

	float vx=objShip.vPos.x-objEnemy.vPos.x;
	float vz=objShip.vPos.z-objEnemy.vPos.z;

	if (vz<0)
		objEnemy.fAngle=atan(vx/vz)-PI_DIV_2;
	else
		objEnemy.fAngle=atan(vx/vz)+PI_DIV_2;

	float length=Distance2D(vx,vz);


    if((length<1000.0f) && (length>5.0f))
	{
		vx=0.15f*vx/length;
		vz=0.15f*vz/length;
		objEnemy.vPos.x+=vx;
		objEnemy.vPos.z+=vz;
	}

	if ((length<20.0f)&& (objEnemy.bReadySonic==true))
	{
		PlaySound(pSfxSonic);

		objBullet.New(objEnemy.vPos,objEnemy.fAngle+PI_DIV_2,SONIC);
		objBullet.Update(fTimeLapsed);
		objEnemy.bReadySonic=false;
	}
	static float Timer = 0.0f;
	Timer += fTimeLapsed;
	if(Timer > 1.0f) 
	{
		Timer = 0.0f;
		objEnemy.bReadySonic=true;
	}

	ShipEngine.Process(fTimeLapsed);
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draw Scene 
//-----------------------------------------------------------------------------
inline void Render()
{	
	//SUN
	D3DXMatrixScaling( &matWorld, objPlanets[SUN].fSize, objPlanets[SUN].fSize, objPlanets[SUN].fSize);
	D3DXMatrixRotationYawPitchRoll(&matTemp, objPlanets[SUN].fRoll,objPlanets[SUN].fRoll,0.0f);
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
	D3DXMatrixTranslation( &matTemp,objPlanets[SUN].vPos.x,objPlanets[SUN].vPos.y,objPlanets[SUN].vPos.z);
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
  	pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	pDevice->SetMaterial( &pPlanetsMaterials[0] );
    pDevice->SetTexture( 0, objPlanets[SUN].texture[0] );
	pMeshPlanet->DrawSubset( 0 );

	// Enable lighting
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	{
		pDevice->LightEnable( SUN, TRUE);
		pDevice->LightEnable( 1, FALSE);
		//Planets
		for (index=1;index<PLANETS_COUNT;index++)
		{
			D3DXMatrixScaling( &matWorld, objPlanets[index].fSize, objPlanets[index].fSize, objPlanets[index].fSize);
			D3DXMatrixRotationYawPitchRoll(&matTemp, objPlanets[index].fRoll,0.0f ,0.0f );
			D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
			D3DXMatrixTranslation( &matTemp,objPlanets[index].vPos.x,objPlanets[index].vPos.y,objPlanets[index].vPos.z);
			D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
			pDevice->SetTransform( D3DTS_WORLD, &matWorld );
			pDevice->SetMaterial( &pPlanetsMaterials[0] );
			pDevice->SetTexture( 0, objPlanets[index].texture[0] );
      		pMeshPlanet->DrawSubset( 0);  
		}

		//pDevice->LightEnable( SUN, FALSE);
		pDevice->LightEnable( 1, TRUE);

		//Enemy
		D3DXMatrixScaling( &matTemp, objEnemy.fSize, objEnemy.fSize,objEnemy.fSize );
		D3DXMatrixRotationYawPitchRoll(&matWorld, objEnemy.fAngle+PI_DIV_2,0.0f ,0.0f );
		D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
		D3DXMatrixTranslation( &matTemp, objEnemy.vPos.x, objEnemy.vPos.y,objEnemy.vPos.z );
		D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
		pDevice->SetTransform( D3DTS_WORLD, &matWorld );
		for(item=0; item<objEnemy.NumMat; item++ )
		{
			pDevice->SetMaterial( &pShipMaterials[item]  );
			pDevice->SetTexture( 0, objPlanets[MERCUR].texture[0]  );
			pMeshEnemy->DrawSubset(item);
		}

		//Ship	
		D3DXMatrixScaling( &matTemp, objShip.fSize, objShip.fSize,objShip.fSize );
		D3DXMatrixRotationYawPitchRoll(&matWorld, objShip.fAngle ,0.0f ,0.0f );
		D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
		D3DXMatrixTranslation( &matTemp, objShip.vPos.x, objShip.vPos.y,objShip.vPos.z );
		D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
		pDevice->SetTransform( D3DTS_WORLD, &matWorld );
		if (objShip.nMode==MODE_CABIN)
		{
			Guide->Draw(GuideTexture, NULL, NULL, NULL, 0.0f, &vGuide, 0xFFFFFFFF);
			//Guide->Draw(GuideTexture, NULL, &D3DXVECTOR3(0.0f,0.0f,0.0f), &vGuide, 0xFFFFFFFF);
		}
		else
		{
			for(item=0; item<objShip.NumMat; item++ )
			{
				pDevice->SetMaterial( &pShipMaterials[item] );
				pDevice->SetTexture( 0, objShip.pTexture[item] );
      			pMeshShip->DrawSubset(item);
 			}
		}
 
 	}// Disable lighting
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	pDevice->SetTexture( 0, NULL );
	//Star

	for (index=0;index< STAR_OBJECT_COUNT;index++ )
	{
		if(Distance3D(objShip.vPos.x-objStar[index].x,objShip.vPos.y-objStar[index].y ,objShip.vPos.z-objStar[index].z )<DIS_NEAR)
		{
			D3DXMatrixTranslation( &matWorld, objStar[index].x,objStar[index].y,objStar[index].z );
			pDevice->SetTransform( D3DTS_WORLD, &matWorld );
			pMeshSonic[objStar[index].null]->DrawSubset(0);
		}
	}


	//Sonic
	D3DXMatrixTranslation( &matWorld, xx[2],objBullet.vPos.y,zz[2]);
	pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	pMeshSonic2[3]->DrawSubset(0);

	D3DXMatrixTranslation( &matWorld, xx[1],objBullet.vPos.y,zz[1] );
	pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	pMeshSonic2[2]->DrawSubset(0);

	D3DXMatrixTranslation( &matWorld, xx[0],objBullet.vPos.y,zz[0] );
	pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	pMeshSonic2[1]->DrawSubset(0);

	D3DXMatrixTranslation( &matWorld, objBullet.vPos.x,objBullet.vPos.y, objBullet.vPos.z);
	pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	pMeshSonic2[0]->DrawSubset(0);

	//Laser
	D3DXMatrixTranslation( &matTemp, objLaser[0].vPos.x, objLaser[0].vPos.y,objLaser[0].vPos.z );
	D3DXMatrixRotationYawPitchRoll(&matWorld,objLaser[0].fAngle ,0.0f ,0.0f );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
	pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	pMeshLaser->DrawSubset(0);

	D3DXMatrixTranslation( &matTemp, objLaser[1].vPos.x, objLaser[1].vPos.y,objLaser[1].vPos.z );
	D3DXMatrixRotationYawPitchRoll(&matWorld,objLaser[1].fAngle ,0.0f ,0.0f );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTemp );
	pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	pMeshLaser->DrawSubset(0);

	// Enable alpha testing for transparent blits
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// Enable alpha blending
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	{
		// Draw the particles	
		ShipEngine.Begin(&matView,&matProj);
		ShipEngine.Render();
		ShipEngine.End();
	} // Disable alpha testing and alpha blending
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void TimerStart(){ DXUtil_Timer( TIMER_START );}
void TimerStop(){  DXUtil_Timer( TIMER_STOP );}


