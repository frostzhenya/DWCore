/*
*
*/

#include "Logs.h"
#include "..\Common.h"
#include <Windows.h>

const char* LOG_FILE_NAME = "DWCore.log";
char LOG_FILE[];

Log::Log()
{
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

void Log::MainLog(const char* _fromat,...)
{
	// test in color
	setColor(LCYAN);

	va_list MList;
	va_start(MList, _fromat);
	int int_d;
	unsigned short u_s;
	char* char_s;
	char char_c;
	std::string string_s;
	std::ofstream file(LOG_FILE_NAME, std::ios::out | std::ios::app);

	for(long int i=0; i < std::strlen(_fromat); i++)
	{
		if (_fromat[i] == '%')
		{
			switch(_fromat[i+1])
			{
			case 'd':
				{
					int_d = va_arg(MList, int);
					std::cout<<int_d;
					file<<int_d;
					break;
				}
			case 's':
				{
					if (_fromat[i+2] == 't')
					{
						string_s = va_arg(MList, std::string);
						std::cout<<string_s;
						file<<string_s;
						i=i+1;
					}
					else
					{
						char_s = va_arg(MList, char*);
						std::cout<<char_s;
						file<<char_s;
					}
					break;
				}
			case 'u':
				{
					if(_fromat[i+2] == 's')
					{
						u_s = va_arg(MList, unsigned short);
						std::cout<<u_s;
						file<<u_s;
						i=i+1;
					}
					break;
				}
			case 'c':
				{
						char_c = va_arg(MList, char);
						std::cout<<char_c;
						file<<char_c;
				}
			}
			i=i+2;
		}
		std::cout<<_fromat[i];
		file<<_fromat[i];
	}
	va_end(MList);
}