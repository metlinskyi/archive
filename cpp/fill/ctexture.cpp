#include <windows.h>
#include "ctexture.h"
#include "cstack.h"

#define NULL 0


TEXTURE::TEXTURE()
{
	data=NULL;
	width=height=0;
}

TEXTURE::~TEXTURE()
{
    delete data;
}
 
// - Прототипы функций -
void SetPoint(TEXTURE* texture, int x, int y, int color);
char GetPoint(TEXTURE* texture, int x, int y);
void FRecursion(TEXTURE* texture,int x, int y , char FillClr,int NewClr);

// -----------------------------------------------------------------------------
void SetPoint(TEXTURE* texture, int x, int y, int color)
{
	char*	lp=texture->data;

	int offset =(texture->width*(y-1))+(x);
	lp+=sizeof(char)*offset;

    *lp=color;
}

char GetPoint(TEXTURE* texture, int x, int y)
{
	char*	lp=texture->data;

	int offset =(texture->width*(y-1))+(x);
	lp+=sizeof(char)*offset;

	return *lp;
}


// -------------------------------------------------------------
//   - Заливка  - 
//
void Fill(TEXTURE* texture,int x, int y ,int NewClr)
{
	if ((x>texture->width-1) || (y>texture->height)) return;

	int		xPop;
	int		yPop;
	char	FillClr=GetPoint(texture,x, y);
	CSTACK	stack;
	
	stack.Push(x,y);
	
	while (stack.Pop(xPop,yPop))
	{
		if (GetPoint(texture,xPop, yPop)==FillClr)
			SetPoint(texture,xPop, yPop,NewClr);

		if (GetPoint(texture,++xPop, yPop)==FillClr)	stack.Push(xPop,yPop);
		if (GetPoint(texture,--xPop, ++yPop)==FillClr) stack.Push(xPop,yPop);
		if (GetPoint(texture,--xPop, --yPop)==FillClr) stack.Push(xPop,yPop);
		if (GetPoint(texture,++xPop, --yPop)==FillClr) stack.Push(xPop,yPop);

		Out(texture,0,1);
		Sleep(10);
	}
}// end Fill

// -------------------------------------------------------------------
//   - Рекурсивная заливка  - 
//
void FRecursion(TEXTURE* texture,int x, int y , char FillClr,int NewClr)
{
	if (GetPoint(texture,x, y) == FillClr )
	{
		SetPoint(texture,x, y,NewClr);

		FRecursion(texture,x+1,y, FillClr,NewClr);
		FRecursion(texture,x-1,y, FillClr,NewClr);
		FRecursion(texture,x,y+1, FillClr,NewClr);
		FRecursion(texture,x,y-1, FillClr,NewClr);

		Out(texture,0,1);
		Sleep(10);
	}
}

void FillRecursion(TEXTURE* texture,int x, int y ,int NewClr)
{
	if ((x>texture->width-1) || (y>texture->height)) return;

	FRecursion(texture,x,y, GetPoint(texture,x,y),NewClr);

}//end FillRecursion


// --------------------------------------------------------------------
//   - Построчная заливка (быстрая) - 
//
void FillFast(TEXTURE* texture,int x, int y, int NewClr)
{
	if ((x>texture->width-1) || (y>texture->height)) return;

	int		xPop;
	int		yPop;
	int		xLeft;
	int		xRight;
    int		xScan;

	char	FillClr =GetPoint(texture,x,y);
	CSTACK	stack;

	stack.Push(x,y);

	while (stack.Pop(xPop,yPop))
	{

		xLeft=xRight=xPop;

		SetPoint(texture,xPop, yPop,NewClr);

		while(true)
		{
			if (GetPoint(texture,++xRight,yPop)==FillClr)
				SetPoint(texture,xRight, yPop,NewClr);
			else
				break;
		}

		while(true)
		{
			if (GetPoint(texture,--xLeft,yPop)==FillClr)
				SetPoint(texture,xLeft, yPop,NewClr);
			else
				break;
		}

		xScan=xLeft;

		while (xScan < xRight)
		{
			if ((GetPoint(texture,xScan,yPop-1)==FillClr))
			{
				stack.Push(xScan,yPop-1);
			}

			if ((GetPoint(texture,xScan,yPop+1)==FillClr))
			{
				stack.Push(xScan,yPop+1);
			}

			xScan++;
		}

		Out(texture,0,1);
		Sleep(10);
	}
} //end  FillFast

// --------------------------------------------------------------------
//   - Короткая на с++ - 
//
void FillShort(TEXTURE* texture,int x, int y ,int NewClr){
	if ((x>texture->width-1) || (y>texture->height)) return;
	int		xPop,yPop,xLeft,xRight,xScan;
	char	FillClr =GetPoint(texture,x,y);
	CSTACK	stack;stack.Push(x,y);
	while (stack.Pop(xPop,yPop)){
		xLeft=xRight=xPop;
		SetPoint(texture,xPop, yPop,NewClr);
		while(true)	{
			if (GetPoint(texture,++xRight,yPop)==FillClr)
				SetPoint(texture,xRight, yPop,NewClr);
			else break;
		}
		while(true){
			if (GetPoint(texture,--xLeft,yPop)==FillClr)
				SetPoint(texture,xLeft, yPop,NewClr);
			else break;
		}
		xScan=xLeft;
		while (xScan < xRight){
			if ((GetPoint(texture,xScan,yPop-1)==FillClr))
				stack.Push(xScan,yPop-1);
			if ((GetPoint(texture,xScan,yPop+1)==FillClr))
				stack.Push(xScan,yPop+1);
			xScan++;
		}
	}
}//end FillShort