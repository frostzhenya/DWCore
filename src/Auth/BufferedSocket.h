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

		char ByteBuffer[0x1000];

		// ByteBuffer
		bool BuildSocketsAuth();
		void BufferClear(int size);
		void BufferAuth_recv();
		void BufferAuth_send(int SizeBuff);
		void AppendPackage(unsigned char ArrBytes[], int CountBytes, int point);
		int SizePackage();

	private:
		//char ByteBuffer[0x1000];
		int Count_p;
		bool ChekSocket;
		Log BBSLog;

		WSADATA WinSocketData;
		SOCKET AuthSocket;
		//SOCKET NewAuthSocket;
		//SOCKET WordSocket;
		sockaddr_in AuthSock_in;
};

