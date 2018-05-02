#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <mmsystem.h>
#include "main.h"
#include "resource.h"
#include "object.h"

BYTE CurrentLevel=0;

LEVEL	Level;
LEVEL*	lpLevel;

unsigned long global_id=0;




int rndInt( int low, int high )
{
    int range = high - low;
    int num = rand() % range;
    return( num + low );
}



double rndDouble( double low, double high )
{
    double range = high - low;
    double num = range * (double)rand()/(double)RAND_MAX;
    return( num + low );
}

unsigned long GetNewID()	{	return 	global_id++;} //глобальный идентификатор



void InitSin_CosTables(void)
{
	int angle;
    for(angle = -1;angle < 361;angle++)
    {
        //SinTable[Angle] = (int)(sin((double)(Angle) / 256 * 6.28) * 0x100);
        //CosTable[Angle] = (int)(cos((double)(Angle) / 256 * 6.28) * 0x100);
		SinTable[angle]=sin((double)angle* 3.14 / 180);
		CosTable[angle]=cos((double)angle* 3.14 / 180);
    }
} 

int getint(char**p)
{
    int i=0;
    
    
    while (IS_SPACE(**p))
        (*p)++;
    
    if (!IS_NUM(**p))
        return 0;
    
    while (IS_NUM(**p))
        i = i*10 + *(*p)++ - '0';
    
    while (IS_SPACE(**p))
        (*p)++;
    
    return i;
}
