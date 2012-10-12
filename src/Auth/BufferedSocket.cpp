/*
* code in mangos
*/

#include "BufferedSocket.h"
#include <new>

ByteBufferSocket::ByteBufferSocket()
{
	if (WSAStartup(0x0202, &WinSocketData) != 0)
	{
		BBSLog.ErrorLog("ERROR WSAStartup\n");
	}
	else
	{
		BBSLog.DebugLog("Successfully WSAStartup\n");
	}
}

ByteBufferSocket::~ByteBufferSocket()
{
}

bool ByteBufferSocket::BuildSocketsAuth()
{
	ChekSocket = false;
	
	AuthSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (AuthSocket == INVALID_SOCKET)
	{
		BBSLog.ErrorLog("ERROR open AuthSocket\n");
		ChekSocket = false;
	}
	else
		BBSLog.DebugLog("Successfully open AuthSocket\n");

	AuthSock_in.sin_family = AF_INET;
	AuthSock_in.sin_port = htons(AuthSocketPort);
	AuthSock_in.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("127.0.0.1");

	int BindAS = bind(AuthSocket, (sockaddr*)(&AuthSock_in), sizeof(AuthSock_in));
	int ListeAS = listen(AuthSocket, SOMAXCONN);

	if (BindAS == 0 && ListeAS == 0)
	{
		BBSLog.MainLog("Auth bind and listen port %d\n", AuthSocketPort);
		ChekSocket = true;
	}
	else
	{
		BBSLog.ErrorLog("ERROR Auth bind or listen port %d\n", AuthSocketPort);
		ChekSocket = false;
	}

	int k = sizeof(AuthSock_in);
	AuthSocket = accept(AuthSocket, (sockaddr*)&AuthSock_in, &k);

	if (AuthSocket == INVALID_SOCKET)
	{
		BBSLog.ErrorLog("ERROR accept AuthSocket\n");
		ChekSocket = false;
	}
	else
	{
		BBSLog.DebugLog("Successfully accept AuthSocket\n");
		ChekSocket = true;
	}

	return ChekSocket;
}

void ByteBufferSocket::BufferClear(int size)
{
	for (int i = 0; i <= size; i++)
	{
		ByteBuffer[i] = 0;
	}
}

void ByteBufferSocket::BufferAuth_recv()
{
	Count_p = recv(AuthSocket, ByteBuffer, sizeof(ByteBuffer), 0);
}

void ByteBufferSocket::BufferAuth_send(int SizeBuff)
{
	send(AuthSocket, ByteBuffer, SizeBuff, 0);
}

void ByteBufferSocket::AppendPackage(unsigned char ArrBytes[], int CountBytes, int point)
{
	for(int i = point; i <= CountBytes; i++)
	{
		ByteBuffer[i] = ArrBytes[i - point];
	}
}

int ByteBufferSocket::SizePackage()
{
	return Count_p;
}