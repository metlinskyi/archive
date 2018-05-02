class TEXTURE 
{
public:
	int		width;
	int		height;
	char*	data;

	TEXTURE();
	~TEXTURE();
};


void Out(TEXTURE* texture,int x, int y);

void Fill(TEXTURE* texture,int x, int y,int NewClr);
void FillRecursion(TEXTURE* texture,int x, int y ,int NewClr);
void FillFast(TEXTURE* texture,int x, int y, int NewClr);

