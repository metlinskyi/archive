#ifndef CPLANET_H
#define CPLANET_H

#include "cobject3d.h"



class CPLANET : public C3DOBJECT
{
public:
    float	fRoll;
    float	fAngle;
    float	fRate;
    float	fPeriod;

    float	fRadius;
    float	fSize;

	char	name[32];	
	//char	textur[32];	

	DWORD	NumMat;
	
	LPDIRECT3DTEXTURE9* texture;
    
public:
	CPLANET();

	void New(VECTOR3,char* str);
	void Move(float &);
};

#endif