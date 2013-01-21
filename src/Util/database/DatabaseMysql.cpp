/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: DBConnect.cpp
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#include "Common.h"
#include "DatabaseMysql.h"

DatabaseMysql::DatabaseMysql()
{
}

DatabaseMysql::~DatabaseMysql()
{
	mysql_close(mMysql);
}

bool DatabaseMysql::InitDB(const char * infoString)
{
    MYSQL * mysqlInit = mysql_init(NULL);

    if (!mysqlInit)
    {
		DBLog.ErrorLog("Could not initialize Mysql connection");
        return false;
    }

	int count = 0;
	for (int i = 0; i < strlen(infoString); i++)
	{
		if(infoString[i] == ';')
		{
			count++;
		}
		else
		{
			switch(count)
			{
				case 0: { host += infoString[i]; break; }
				case 1: { port += infoString[i]; break; }
				case 2: { user += infoString[i]; break; }
				case 3: { password += infoString[i]; break; }
				case 4: { db += infoString[i]; break; }
				default: break;
			}
		}
	}
	mysql_options(mysqlInit,MYSQL_SET_CHARSET_NAME,"utf8");
	int port_ = atoi(port.c_str());

	mMysql = mysql_real_connect(mysqlInit, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port_, 0/*unix soket*/, 0);

    if (!mMysql)
    {
		DBLog.ErrorLog("Could not connect to MySQL database at %s: %s\n", host.c_str(),mysql_error(mysqlInit));
        mysql_close(mysqlInit);
        return false;
    }

	DBLog.DebugLog("Connected to MySQL database %s@%s:%s/%s", user.c_str(), host.c_str(), port.c_str(), db.c_str());
	DBLog.MainLog("MySQL client library: %s", mysql_get_client_info());
    DBLog.MainLog("MySQL server ver: %s ", mysql_get_server_info(mMysql));

    if (!mysql_autocommit(mMysql, 1))
		DBLog.DebugLog("AUTOCOMMIT SUCCESSFULLY SET TO 1");
    else
        DBLog.DebugLog("AUTOCOMMIT NOT SET TO 1");

	Execute("SET NAMES `utf8`");
	Execute("SET CHARACTER SET `utf8`");
	return true;
}

bool DatabaseMysql::Execute(const char* sql)
{
    if (!mMysql)
        return false;

    {
        if(mysql_query(mMysql, sql))
        {
			DBLog.ErrorLog("SQL: %s", sql);
            DBLog.ErrorLog("SQL ERROR: %s", mysql_error(mMysql));
            return false;
        }
        // end guarded block
    }

    return true;
}

bool DatabaseMysql::PExecute(const char * format,...)
{
	if (!format)
		return false;

	va_list ap;
	char szQuery [MAX_QUERY_LEN];
	va_start(ap, format);
	int res = vsnprintf( szQuery, MAX_QUERY_LEN, format, ap );
	va_end(ap);

	if(res==-1)
	{
		DBLog.ErrorLog("SQL Query truncated (and not execute) for format: %s",format);
		return false;
	}

	return Execute(szQuery);
}

MYSQL_ROW DatabaseMysql::Query(const char* sql)
{
	if (mysql_query(mMysql, sql) > 0)
	{
		DBLog.ErrDBLog("[SQL ERR]: %s",sql);
		return 0;
	}

	DBLog.DebugLog("[QUERY]: %s", sql);

	mRes = mysql_store_result(mMysql);

	return mysql_fetch_row(mRes);
}

MYSQL_ROW DatabaseMysql::PQuery(const char * format,...)
{
	if (!format)
		return false;

	va_list ap;
	char szQuery [MAX_QUERY_LEN];
	va_start(ap, format);
	int res = vsnprintf( szQuery, MAX_QUERY_LEN, format, ap );
	va_end(ap);

	return Query(szQuery);

}

unsigned long DatabaseMysql::Pescape_string(char *to, const char *from, unsigned long length)
{
    if (!mMysql || !to || !from || !length)
        return 0;

    return(mysql_real_escape_string(mMysql, to, from, length));
}

void DatabaseMysql::escape_string(std::string& str)
{
    if(str.empty())
        return;

    char* buf = new char[str.size()*2+1];
    //we don't care what connection to use - escape string will be the same
    Pescape_string(buf,str.c_str(),str.size());
    str = buf;
    delete[] buf;
}

void DatabaseMysql::ResultFree()
{
	mysql_free_result(mRes);
}