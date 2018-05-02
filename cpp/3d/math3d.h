// watch for multiple inclusions

#ifndef MATH3D
#define MATH3D

#include <stdlib.h>

// pi defines
#define PI         ((float)3.141592654f)
#define PI2        ((float)6.283185307f)
#define PI_DIV_2   ((float)1.570796327f)
#define PI_DIV_4   ((float)0.785398163f) 
#define PI_INV     ((float)0.318309886f) 
//-----------------------------------------------------------------------------
//  Macros
//-----------------------------------------------------------------------------
// used to compute the min and max of two expresions
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (b) : (a)) 
// used for swapping algorithm
#define SWAP(a,b,t) {t=a; a=b; b=t;}
// some math macros
#define DEG_TO_RAD(ang) ((ang)*PI/180.0)
#define RAD_TO_DEG(rads) ((rads)*180.0/PI)
// returns a rand in a range
#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))


//-----------------------------------------------------------------------------
// Fast Functioon (Inline)
//-----------------------------------------------------------------------------
inline int Distance(int x, int y);
inline float Distance2D(float x, float y);
inline float Distance3D(float fx, float fy, float fz);

// Simple function for generating random numbers
inline int nRnd( int low, int high )
{
    int range = high - low;
    int num = rand() % range;
    return( num + low );
}

inline float fRnd( float low, float high )
{
    return low + ( high - low ) * ( (float)rand() ) / RAND_MAX;
}
////////////////////////////////////////////////////////////////////////////

inline int Distance(int x, int y)
{
	// this function computes the distance from 0,0 to x,y with 3.5% error

	// first compute the absolute value of x,y
	x = abs(x);
	y = abs(y);

	// compute the minimum of x,y
	int mn = MIN(x,y);

	// return the distance
	return(x+y-(mn>>1)-(mn>>2)+(mn>>4));

} // end Fast_Distance_2D

inline float Distance2D(float x, float y)
{
	// this function computes the distance from 0,0 to x,y with 3.5% error

	// first compute the absolute value of x,y
	x = (float)fabs(x);
	y = (float)fabs(y);

	float mn = MIN(x,y);	// compute the minimum of x,y

	return(x+y-(mn/2)-(mn/4)+(mn/8));	// return the distance
}

///////////////////////////////////////////////////////////////////////////////

inline float Distance3D(float fx, float fy, float fz)
{
// this function computes the distance from the origin to x,y,z

	int temp;  // used for swaping
	int x,y,z; // used for algorithm

	// make sure values are all positive
	x = (float)fabs(fx) * 1024;
	y = (float)fabs(fy) * 1024;
	z = (float)fabs(fz) * 1024;

	// sort values
	if (y < x) SWAP(x,y,temp)

	if (z < y) SWAP(y,z,temp)

	if (y < x) SWAP(x,y,temp)

	int dist = (z + 11 * (y >> 5) + (x >> 2) );

	// compute distance with 8% error
	return((float)(dist >> 10));

} // end Fast_Distance_3D

//inline DWORD FtoDW( float f ) { return *((DWORD*)&f); }

#endif
