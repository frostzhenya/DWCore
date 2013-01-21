/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: Common.h
| Author: RMDC,lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#ifndef COMMON_H
#define COMMON_H

#if COMPILER == COMPILER_MICROSOFT
#  pragma warning(disable:4996)
#ifndef __SHOW_STUPID_WARNINGS__
#  pragma warning(disable:4244)
#  pragma warning(disable:4355)
#endif
#endif    

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <map>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <conio.h>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <set>
#include <list>
#include <tchar.h>

#include "Define.h"

#include <ace/Basic_Types.h>
#include <ace/Guard_T.h>
#include <ace/RW_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <ace/OS_NS_arpa_inet.h>

#if PLATFORM == PLATFORM_WINDOWS
#  if !defined (FD_SETSIZE)
#    define FD_SETSIZE 4096
#  endif
#  include <ace/config-all.h>
#  include <ws2tcpip.h>
#endif

// Used in realmd
enum RealmFlags
{
	REALM_FLAG_NONE         = 0x00,
	REALM_FLAG_INVALID      = 0x01,
	REALM_FLAG_OFFLINE      = 0x02,
	// client will show realm version in RealmList screen in form "RealmName (major.minor.revision.build)"
	REALM_FLAG_SPECIFYBUILD = 0x04,
	REALM_FLAG_UNK1         = 0x08,
	REALM_FLAG_UNK2         = 0x10,
	REALM_FLAG_NEW_PLAYERS  = 0x20,
	REALM_FLAG_RECOMMENDED  = 0x40,
	REALM_FLAG_FULL         = 0x80
};

#endif