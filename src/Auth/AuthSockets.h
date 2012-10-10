/*
*
*/

#include "..\Common.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "..\logs\Logs.h"

//temporary settings
const int AuthSocketPort = 3724;

class AuthSockets
{
	public:
		char ByteBuffer[0x1000];

		AuthSockets();
		~AuthSockets();
		bool BuildSocketsAuth();


	private:
		bool _OSocket;
		Log AuthSocketsLog;

		WSADATA Authdata;
		SOCKET AuthSocket;
		sockaddr_in AuthSock_in;
};