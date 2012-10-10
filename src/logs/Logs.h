/*
*
*/

#include "..\Common.h"

#ifndef _LOGS_H
#define _LOGS_H

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
} sLog;



#endif
