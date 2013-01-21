/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: util.cpp
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#include "util.h"
#include "utf8cpp/utf8.h"

bool Utf8toWStr(char const* utf8str, size_t csize, wchar_t* wstr, size_t& wsize)
{
    try
    {
        size_t len = utf8::distance(utf8str,utf8str+csize);
        if(len > wsize)
        {
            if(wsize > 0)
                wstr[0] = L'\0';
            wsize = 0;
            return false;
        }

        wsize = len;
        utf8::utf8to16(utf8str,utf8str+csize,wstr);
        wstr[len] = L'\0';
    }
    catch(std::exception)
    {
        if(wsize > 0)
            wstr[0] = L'\0';
        wsize = 0;
        return false;
    }

    return true;
}

bool Utf8toWStr(const std::string& utf8str, std::wstring& wstr)
{
    try
    {
        size_t len = utf8::distance(utf8str.c_str(),utf8str.c_str()+utf8str.size());
        wstr.resize(len);

        if (len)
            utf8::utf8to16(utf8str.c_str(),utf8str.c_str()+utf8str.size(),&wstr[0]);
    }
    catch(std::exception)
    {
        wstr = L"";
        return false;
    }

    return true;
}

void vutf8printf(FILE *out, const char *str, va_list* ap)
{
#if PLATFORM == PLATFORM_WINDOWS
	char temp_buf[32*1024];
	wchar_t wtemp_buf[32*1024];

	size_t temp_len = vsnprintf(temp_buf, 32*1024, str, *ap);

	size_t wtemp_len = 32*1024-1;
	Utf8toWStr(temp_buf, temp_len, wtemp_buf, wtemp_len);

	CharToOemBuffW(&wtemp_buf[0], &temp_buf[0], wtemp_len+1);
	fprintf(out, "%s", temp_buf);
#else
    vfprintf(out, str, *ap);
#endif
}