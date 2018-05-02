#pragma once
#include "cobject3d.h"

#define SONIC 1
#define LASER 2

class CBULLET :	public C3DOBJECT
{
public:

	float	fAngle;
	float	fSpeed;
	bool	bReady;

	CBULLET(void);
	~CBULLET(void);

	void Update(float);
	void New(VECTOR3 v,float a,short t);

};
