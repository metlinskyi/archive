#include "cbullet.h"

CBULLET::CBULLET(void)
{
}

CBULLET::~CBULLET(void)
{
}

void CBULLET::New(VECTOR3 v,float a,short t)
{
    dwType =t;
	fAngle = a;
	vPos.x=v.x;
	vPos.y=v.y-0.15f;
	vPos.z=v.z;

	if (dwType == SONIC)
	{
		fSpeed=30.0f;
	}

	if (dwType == LASER)
	{
		fSpeed=80.0f;
	}


}

void CBULLET::Update(float eTime)
{
	vPos.x -= sinf(fAngle) * fSpeed*eTime;
	//vPos.y -= cosf(fAngle) * fSpeed*eTime;
	vPos.z -= cosf(fAngle) * fSpeed*eTime;

}