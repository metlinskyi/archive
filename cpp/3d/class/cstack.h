struct DATA
{
	int x;
	int y;
	int z;
};

struct LIST
{
	LIST*	next;
	DATA	data;
};

class CSTACK
{
public:

	LIST* stack;

	CSTACK();
	~CSTACK();

	bool Pop(DATA &dt);
	void Push(DATA dt);
};

