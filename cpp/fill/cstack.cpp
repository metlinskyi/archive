#include "cstack.h"

#define NULL 0

CSTACK::CSTACK()
{
	stack=0;
	deep=0;
}

CSTACK::~CSTACK()
{
	int tmpx,tmpy;
	if (stack!=NULL)
	{
		while(Pop(tmpx,tmpy));
	}
}

bool CSTACK::Pop(int &x, int &y)
{
	LIST*  lp;

	if (stack==0)return false;

	x=stack->x;
	y=stack->y;

	lp=stack->next;

	delete stack;

	stack=lp;

	lp=0;

	deep--;

	return true;
}

void CSTACK::Push(int x, int y)
{
	LIST* lst = new LIST;

	lst->x=x;
	lst->y=y;
	lst->next=stack;
	stack=lst;

	deep++;
}