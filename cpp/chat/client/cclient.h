
class CCLIENT
{	

	SOCKET			sock;


	char buffer[256];
public:

	CCLIENT();
	~CCLIENT();
	bool Connect(const char* host,unsigned long port );
	void Disconnect();

	void SendMsg(const char* msg);
	void RecvMsg(char* msg,int size);
	void Exit();

	friend void observer(const char* msg);

};
