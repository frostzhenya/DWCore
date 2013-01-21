/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: Logs.h
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#ifndef _LOGS_H
#define _LOGS_H

#include "Common.h"

enum Color
{
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    MAGENTA,
    CYAN,
    GREY,
    YELLOW,
    LRED,
    LGREEN,
    LBLUE,
    LMAGENTA,
    LCYAN,
    WHITE
};

const int Color_count = int(WHITE)+1;

class Log
{
	public:
		Log();
		~Log();

		void outTime();
		void setColor(Color color);
		void MainLog(const char* str,...);
		void ErrorLog(const char* str,...);
		void ErrDBLog(const char* str,...);
		void DebugLog(const char* str,...);
	private:
		std::string times();
		bool out_inlucdeTime; // пойдет в конф
};
#endif
