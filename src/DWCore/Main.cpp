/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: Main.cpp
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/
 // information in socket http://incpp.blogspot.com/2010/11/openssl.html

#include "Common.h"
#include "AuthServer.h"
#include "Logs.h"
#include "Config\Config.h"
#include "database\DatabaseEnv.h"
#include <openssl/opensslv.h>
#include <openssl/crypto.h>

#include <ace/Get_Opt.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/TP_Reactor.h>
#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>

#include "Define.h"

#define _REALMD_CONFIG "realmd.conf"
#define DEFAULT_REALMSERVER_PORT 3724

//int WS_PORT = 7000;
bool stopEvent = false;
DatabaseType LoginDatabase;

extern int main(int argc, char **argv)
{
	system("mode con cols=90 lines=35");
	Log sLog;
	Config sConfig;

	if(!sConfig.SetSource(_REALMD_CONFIG))
	{
		sLog.ErrorLog("Could not find configuration file %s.", _REALMD_CONFIG);
		return 1;
	}

	sLog.MainLog("<Ctrl-C> to stop.");
	sLog.MainLog("Using configuration file %s.", _REALMD_CONFIG);
	sLog.MainLog("%s (Library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
	sLog.MainLog("Using ACE: %s", ACE_VERSION);

#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)
    ACE_Reactor::instance(new ACE_Reactor(new ACE_Dev_Poll_Reactor(ACE::max_handles(), 1), 1), true);
#else
    ACE_Reactor::instance(new ACE_Reactor(new ACE_TP_Reactor(), true), true);
#endif

	sLog.MainLog("Max allowed open files is %d", ACE::max_handles());
    std::string dbstring = sConfig.GetStringDefault("LoginDatabaseInfo", "");
    if(dbstring.empty())
    {
		sLog.ErrorLog("Database not specified");
        return 1;
    }

	if(!LoginDatabase.InitDB(dbstring.c_str()))
	{
		return 1;
	}

	//- Launch the listening network socket
	ACE_Acceptor<AuthServer, ACE_SOCK_Acceptor> acceptor;

	uint16 RealmPort = sConfig.GetIntDefault("RealmServerPort", DEFAULT_REALMSERVER_PORT);
	std::string bind_ip = sConfig.GetStringDefault("BindIP", "0.0.0.0");
	
	ACE_INET_Addr bind_addr(RealmPort, bind_ip.c_str());

	if(acceptor.open(bind_addr, ACE_Reactor::instance(), ACE_NONBLOCK) == -1)
	{
		sLog.ErrorLog("DWCore realmd can not bind to %s:%d", bind_ip.c_str(), RealmPort);
		return 1;
	}

	// Wait for termination signal
    while (!stopEvent)
    {
        // dont move this outside the loop, the reactor will modify it
        ACE_Time_Value interval(0, 100000);
		
		if (ACE_Reactor::instance()->run_reactor_event_loop(interval) == -1)
            break;
	}
	return 0;
}