/*
*
*/

#include "..\Common.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "..\logs\Logs.h"

//temporary settings
const int AuthSocketPort = 3724;

class ByteBufferSocket
{
	public:
		ByteBufferSocket();
		~ByteBufferSocket();

		bool BuildSocketsAuth();
		void BufferClear(int size);
		void BufferAuth_recv();
		void BufferAuth_send(int SizeBuff);

	private:
		char ByteBuffer[0x1000];

		bool ChekSocket;
		Log BBSLog;

		WSADATA WinSocketData;
		SOCKET AuthSocket;
		//SOCKET NewAuthSocket;
		//SOCKET WordSocket;
		sockaddr_in AuthSock_in;
};

