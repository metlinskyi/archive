struct LIST
{
	LIST * next;
	int x;
	int y;
};

class CSTACK
{
public:

	LIST* stack;
	long  deep;

	CSTACK();
	~CSTACK();

	bool Pop(int &x, int &y);
	void Push(int x, int y);
};

