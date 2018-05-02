#include "cplanet.h"


CPLANET::CPLANET()
{

}

void CPLANET::New(VECTOR3 v,char *str)
{
	vPos.x=v.x;
	vPos.y=v.y;
	vPos.z=v.z;
}

void CPLANET::Move(float& fTimeLapsed)
{
	fRoll-=fPeriod *fTimeLapsed ;

	fAngle+=fRate *fTimeLapsed;
	vPos.x=vVel.x;
	vPos.y=vVel.y;
	vPos.z=vVel.z;

	vPos.x -= sinf(fAngle) * fRadius;
	vPos.z -= cosf(fAngle) * fRadius;

}