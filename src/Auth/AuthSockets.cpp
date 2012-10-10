/*
*
*/

#include "AuthSockets.h"

AuthSockets::AuthSockets()
{
}

AuthSockets::~AuthSockets()
{
}

bool AuthSockets::BuildSocketsAuth()
{
	_OSocket = false;

	if (WSAStartup(0x0202, &Authdata)==0)
	{
		AuthSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (AuthSocket == INVALID_SOCKET)
		{
			AuthSocketsLog.ErrorLog("ERROR OPEN AUTH SOCKET\n");
			exit(0);
		}

		AuthSock_in.sin_family = AF_INET;
		AuthSock_in.sin_port = htons(AuthSocketPort);
		AuthSock_in.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("127.0.0.1");

		int i = bind(AuthSocket, (sockaddr*)(&AuthSock_in), sizeof(AuthSock_in));
		int l = listen(AuthSocket, SOMAXCONN);

		if (i == 0 && l == 0)
		{
			AuthSocketsLog.MainLog("Auth bind and listen port %d\n", AuthSocketPort);
			_OSocket = true;
		}
		else
		{
			AuthSocketsLog.ErrorLog("ERROR AUTH BIND OR LISTEN PORT %d\n", AuthSocketPort);
			exit(0);
		}
	}

	return _OSocket;
}