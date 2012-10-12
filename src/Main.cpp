/*
* information in socket http://incpp.blogspot.com/2010/11/openssl.html
*/

#include "Main.h"
#include "Logs\Logs.h"
#include "Common.h"
#include "Auth\AuthServer.h"

//temporary settings
//int WS_PORT = 7000;

void main()
{
	AuthServer pct;
	pct._HandleLogonChallenge();

	system("pause");

}