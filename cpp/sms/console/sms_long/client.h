#include <winsock2.h>


class cclient
{

public:

	cclient();
	cclient(bool);
	~cclient();

	bool init();
	bool _connect(const char* host,unsigned short port);
	void stop();
	void active();

	void _send(char data);
	char _recv();

	void _sendex(char* data,int size);
	void _recvex(char* data,int size);

	friend void observer(char * msg);

private:

	WSADATA			wsaData; 
	SOCKET			sock;
	SOCKADDR_IN		addr;
	hostent*		hp; 

	bool			debug;
protected:

};