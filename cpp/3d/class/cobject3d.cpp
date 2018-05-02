#include "cobject3d.h"

C3DOBJECT::C3DOBJECT()
{

}

C3DOBJECT::C3DOBJECT(short type,VECTOR3 p,VECTOR3 v )
{
    pNext    = 0;
    pPrev    = 0;
    bVisible = true;
    dwType   = type;
    vPos     = p;
    vVel     = v;
}