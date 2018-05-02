#ifndef CASTEROID_H
#define CASTEROID_H

#include "cobject3d.h"



class CASTEROID : public C3DOBJECT
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
	CASTEROID();

	void New(VECTOR3,char* str);
	void Move(float);
};

#endif