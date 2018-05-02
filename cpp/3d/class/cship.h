#ifndef CSHIP_H
#define CSHIP_H

#include "cobject3d.h"


class CSHIP : public C3DOBJECT
{
public:
	DWORD 	NumMat;
	char	name[32];

    float	fSize;
    float	fRoll;
    float	fAngle;
    float	fAngleSin;
    float	fAngleCos;

    float	fRate;
    float	fAxil;
    float	fMove;
    float	fMaxSpeed;
    float	fTmp;

	float	fEngineL;
	float	fEngineR;

	int		nMode;

	bool bReversDw;
	bool bReversUp;
	bool bReadyLaser;
	bool bReadySonic;
	bool bClose;

	LPDIRECT3DTEXTURE9*		pTexture;

public:
	CSHIP();
};


#endif