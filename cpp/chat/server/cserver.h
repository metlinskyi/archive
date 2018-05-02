#pragma comment ( lib, "wsock32.lib" )
#pragma comment ( lib, "winmm.lib." )


#include <winsock2.h>

class CSERVER
{
	WSADATA			wsa; 
	SOCKET			sock;
	SOCKADDR_IN		addr;

public:
    CSERVER();
	~CSERVER();
	bool Start(char*, unsigned short);

	int SendMsg(SOCKET socket,const char* msg );
	bool Chat();

	friend void observer(char * msg);
};