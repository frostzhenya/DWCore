/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: DBConnect.h
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#include "Common.h"
#include "..\Logs.h"
#ifdef WIN32
#include <winsock2.h>
#include <mysql/mysql.h>
#else
#include <mysql.h>
#endif

#define MAX_QUERY_LEN   32*1024

class DatabaseMysql
{
public:
	DatabaseMysql();
	~DatabaseMysql();

	bool InitDB(const char * infoString);
	bool Execute(const char* sql);
	bool PExecute(const char * format,...);
	MYSQL_ROW Query(const char* str);
	MYSQL_ROW PQuery(const char * format,...);
	unsigned long Pescape_string(char *to, const char *from, unsigned long length);
	void DatabaseMysql::escape_string(std::string& str);
	void ResultFree();

private:
	MYSQL_RES * mRes;
	MYSQL *mMysql;
	std::string host, port, user, password, db;
	Log DBLog; 
};