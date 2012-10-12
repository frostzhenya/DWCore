/*
*
*/

#include "Logs.h"
#include "..\Common.h"
#include <Windows.h>

const char* MAIN_LOG_FILE_NAME = "DWCore.log";
const char* ERROR_LOG_FILE_NAME = "Errors.log";
const char* DEBUG_LOG_FILE_NAME = "Debugs.log";

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

	std::ofstream file(MAIN_LOG_FILE_NAME, std::ios::out | std::ios::app);

	for(int i = 0; i < std::strlen(_fromat); i++)
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
					char_s = va_arg(MList, char*);
					std::cout<<char_s;
					file<<char_s;

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
		if (i == 0)
		{
			std::cout<<times().c_str()<<_fromat[i];
			file<<times().c_str()<<_fromat[i];
		}
		else
		{
			std::cout<<_fromat[i];
			file<<_fromat[i];
		}
	}
	va_end(MList);
}

void Log::ErrorLog(const char* _fromat,...)
{
	// test in color
	setColor(RED);

	va_list MList;
	va_start(MList, _fromat);
	int int_d;
	unsigned short u_s;
	char* char_s;
	char char_c;

	std::ofstream file(ERROR_LOG_FILE_NAME, std::ios::out | std::ios::app);

	for(int i = 0; i < std::strlen(_fromat); i++)
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
					char_s = va_arg(MList, char*);
					std::cout<<char_s;
					file<<char_s;
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
		if (i == 0)
		{
			std::cout<<times().c_str()<<_fromat[i];
			file<<times().c_str()<<_fromat[i];
		}
		else
		{
			std::cout<<_fromat[i];
			file<<_fromat[i];
		}
	}
	va_end(MList);
}

void Log::DebugLog(const char* _fromat,...)
{
	// test in color
	setColor(GREEN);

	va_list MList;
	va_start(MList, _fromat);
	int int_d;
	unsigned short u_s;
	unsigned char u_c;
	char* char_s;
	char char_c;
	std::string string_s;
	std::ofstream file(DEBUG_LOG_FILE_NAME, std::ios::out | std::ios::app);

	for(int i = 0; i < std::strlen(_fromat); i++)
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
					char_s = va_arg(MList, char*);
					std::cout<<char_s;
					file<<char_s;
					break;
				}
			case 'u':
				{
					// unsigned short
					if(_fromat[i+2] == 's' && _fromat[i+3] != '#')
					{
						u_s = va_arg(MList, unsigned short);
						std::cout<<u_s;
						file<<u_s;
						i=i+1;
					}
					else if (_fromat[i+2] == 's' && _fromat[i+3] == '#' && _fromat[i+4] == 'x')
					{
						u_s = va_arg(MList, unsigned short);
						std::cout<<"0x"<<std::hex<<u_s;
						file<<"0x"<<std::hex<<u_s;
						i=i+3;
					}

					// unsigned char
					if (_fromat[i+2] == 'c')
					{
						u_c = va_arg(MList, unsigned char);
						std::cout<<u_c;
						file<<u_c;
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
		if (i == 0)
		{
			std::cout<<times().c_str()<<_fromat[i];
			file<<times().c_str()<<_fromat[i];
		}
		else
		{
			std::cout<<_fromat[i];
			file<<_fromat[i];
		}
	}
	va_end(MList);
}

std::string Log::times()
{
    time_t t = time(NULL);
    tm* aTm = localtime(&t);
    char buf[22];
    sprintf(buf,"%04d.%02d.%02d[%02d:%02d:%02d] ",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
    return std::string(buf);
}