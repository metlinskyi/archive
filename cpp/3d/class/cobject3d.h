#ifndef C3DOBJECT_H
#define C3DOBJECT_H
#include <d3d9.h>
#include <math.h>

struct VECTOR3
{
	float x;
	float y;
	float z;
	int null;
};

class C3DOBJECT
{
public: 
    C3DOBJECT*		pNext;          // Link to next object
    C3DOBJECT*		pPrev;          // Link to previous object
    
    short			dwType;            // Type of object
    bool			bVisible;          // Whether the object is visible

    VECTOR3			vPos;              // Position
    VECTOR3			vVel;              // Velocity
    float			fSize;

public:    
    // Constructor
	C3DOBJECT();
	C3DOBJECT(short,VECTOR3 ,VECTOR3 );
};



#endif
