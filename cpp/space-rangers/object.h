struct OBJECT
{
	int x,y;
	int cx,cy;
	int h,w;
	int speed;
	int hp;
	int	hp_;
	int cost;

	unsigned long id;
	char fname[8];
	bool visible;
	int frame;

	RECT odz;
	RECT pic;

	int a,b;

	int x1,y1;
	int x2,y2;
	int x3,y3;

	int sx1,sy1;
	int sx2,sy2;
	int sx3,sy3;

	int damage_max;
	int damage_min;

	BYTE power;
	BYTE type;

	float fx,fy;
	float fx1,fy1;
	float fx2,fy2;
	float fx3,fy3;
	float fa,fb;

	float speed_f;
	float accel_f;
};


struct POINT_TEXT
{
	int x,y;
	int id;
};


struct LEVEL
{
	unsigned long	length;
	double			length_d;
	BYTE			number;
	char			next[16];
};

struct SCORE
{
	char			name[16];
	BYTE			race;

	unsigned long	shoot_enemi_l;
	unsigned long	shoot_enemi_s;
	unsigned long	shoot_enemi_m;

	unsigned long	shoot_pirat_l;
	unsigned long	shoot_pirat_s;
	unsigned long	shoot_pirat_m;

	unsigned long	shoot_transport;

	unsigned long	crash_enemi_l;
	unsigned long	crash_enemi_s;
	unsigned long	crash_enemi_m;


	unsigned long	kill_boss;
	unsigned long	crash_boss;

	unsigned long	riches;

	unsigned long	sicrets;
};

struct LIST
{
	LIST	* prev;
	LIST	* next;
	OBJECT  obj;
};

typedef LIST *LPLIST;