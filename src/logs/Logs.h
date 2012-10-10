/*
*
*/
#ifndef _LOGS_H
#define _LOGS_H

#include "..\Common.h"

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

		void setColor(Color color);
		void MainLog(const char* _fromat,...);
		void ErrorLog(const char* _fromat,...);
		void DebugLog(const char* _fromat,...);
	private:
		std::string times();
};
#endif
