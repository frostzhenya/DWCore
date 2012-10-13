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
	system("mode con cols=90 lines=35");

	AuthServer pct;
	pct._HandleLogonChallenge();

	system("pause");

}