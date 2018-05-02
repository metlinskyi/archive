#include "cstack.h"

#define NULL 0

CSTACK::CSTACK()
{
	stack=NULL;
}

CSTACK::~CSTACK()
{
	DATA tmp;
	if (stack!=NULL)
	{
		while(Pop(tmp));
	}
}

bool CSTACK::Pop(DATA &dt)
{
	LIST*  lp;

	if (stack==NULL)return false;

	dt=stack->data;
	lp=stack->next;

	delete stack;

	stack=lp;
	lp=0;

	return true;
}

void CSTACK::Push(DATA dt)
{
	LIST* lst = new LIST;

	lst->data=dt;
	lst->next=stack;
	stack=lst;
}