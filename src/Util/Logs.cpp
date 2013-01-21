/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: Logs.cpp
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#include "Logs.h"
#include "Common.h"
#include "util.h"
#include <Windows.h>

Log::Log()
{
	out_inlucdeTime = 1;
}

Log::~Log()
{
}

void Log::setColor(Color color)
{
	static unsigned short WinColorFG[Color_count] =
	{
        0,                                                  // BLACK
        FOREGROUND_RED,                                     // RED
        FOREGROUND_GREEN,                                   // GREEN
        FOREGROUND_RED | FOREGROUND_GREEN,                  // BROWN
        FOREGROUND_BLUE,                                    // BLUE
        FOREGROUND_RED |                    FOREGROUND_BLUE,// MAGENTA
        FOREGROUND_GREEN | FOREGROUND_BLUE,                 // CYAN
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,// WHITE
                                                            // YELLOW
        FOREGROUND_RED | FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
                                                            // RED_BOLD
        FOREGROUND_RED |                                      FOREGROUND_INTENSITY,
                                                            // GREEN_BOLD
        FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
        FOREGROUND_BLUE | FOREGROUND_INTENSITY,             // BLUE_BOLD
                                                            // MAGENTA_BOLD
        FOREGROUND_RED |                    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
                                                            // CYAN_BOLD
        FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
                                                            // WHITE_BOLD
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    };

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, WinColorFG[color]);
}

void Log::outTime()
{
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	//       YYYY   year
	//       MM     month (2 digits 01-12)
	//       DD     day (2 digits 01-31)
	//       HH     hour (2 digits 00-23)
	//       MM     minutes (2 digits 00-59)
	//       SS     seconds (2 digits 00-59)
	printf("%02d:%02d:%02d ",aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
}

void Log::MainLog(const char* str,...)
{
	if (!str)
		return;
	// test in color
	setColor(LCYAN);
	
	if (out_inlucdeTime)
		outTime();
	
	va_list ap;
	va_start(ap, str);
	vutf8printf(stdout, str, &ap);
	va_end(ap);
	
	printf( "\n" );

	fflush(stdout);
}

void Log::ErrorLog(const char* str,...)
{
	if (!str)
		return;
	// test in color
	setColor(RED);
	
	if (out_inlucdeTime)
		outTime();
	
	va_list ap;
	va_start(ap, str);
	vutf8printf(stdout, str, &ap);
	va_end(ap);
	
	printf( "\n" );

	fflush(stdout);
}

void Log::DBLog(const char* str,...)
{
	if (!str)
		return;
	// test in color
	setColor(BLUE);
	
	if (out_inlucdeTime)
		outTime();
	
	va_list ap;
	va_start(ap, str);
	vutf8printf(stdout, str, &ap);
	va_end(ap);
	
	printf( "\n" );

	fflush(stdout);
}

void Log::DebugLog(const char* str,...)
{
	if (!str)
		return;
	// test in color
	setColor(GREEN);

	if (out_inlucdeTime)
		outTime();
	
	va_list ap;
	va_start(ap, str);
	vutf8printf(stdout, str, &ap);
	va_end(ap);
	
	printf( "\n" );

	fflush(stdout);
}