#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/SECTION:.text,EWRX")

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <iostream.h>
#include "ctexture.h"
#include "cstack.h"

#define WIDTH	60;
#define HEIGHT	20;

inline int nRnd( int low, int high ){ return ((rand() % (high - low)) + low); }

void CerateRnd(TEXTURE* texture,int density)//создание поверхности 
{
	char* lp=texture->data;
	for (int x=0;x<texture->width;x++)
	{
		for (int y=0;y<texture->height;y++)
		{
			if (nRnd(0,density)==1)
				*lp=176;
			else
				*lp=178;

			lp++;
		}
	}
}

void Out(TEXTURE* texture,int x, int y)// вывод поверхности 
{
	char*	head=texture->data;
	CONSOLE_SCREEN_BUFFER_INFO cinfo;

	cinfo.dwCursorPosition.X=x;
	cinfo.dwCursorPosition.Y=y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cinfo.dwCursorPosition);
	
	for (int y=0;y<texture->height;y++)
	{
		for (int x=0;x<texture->width;x++)
		{
			cout<<*head;
			head++;
		}
		cout<<endl;
	}
}

void main()
{
	int X=19;
	int Y=10;

	TEXTURE texture;
	texture.height=HEIGHT;
	texture.width=WIDTH;
	texture.data= new char[texture.width*texture.height];
    
	CerateRnd(&texture,2);// создание текстуры
	Out(&texture,0,1);
	getchar();

	Fill(&texture,X,Y,255);
	getchar();

	FillRecursion(&texture,X,Y,250);
	getchar();

	FillFast(&texture,X,Y,252);
	getchar();
}


