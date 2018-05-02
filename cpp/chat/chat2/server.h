#include <winsock2.h>

class cserver
{

public:

	cserver();
	cserver(bool);
	~cserver();

	bool init();
	bool create(unsigned short port,int count);
	void stop();
	void action();

	friend void observer(char * msg);

private:

	WSADATA			wsaData; 
	SOCKET			sock;
	SOCKADDR_IN		addr;
	hostent*		hp; 

	int				max_user;
	bool			debug;


protected:

};


