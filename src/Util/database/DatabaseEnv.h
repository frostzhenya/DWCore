/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: DBEnv.h
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#include "Common.h"
#include "DatabaseMysql.h"

typedef DatabaseMysql DatabaseType;
typedef MYSQL_ROW DBRow;

extern DatabaseType WorldDatabase;
extern DatabaseType CharacterDatabase;
extern DatabaseType LoginDatabase;