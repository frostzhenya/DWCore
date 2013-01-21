/*-------------------------------------------------------+
| DWCore Content Management System
| Copyright (C) 2012 - 2013 lovepsone
+--------------------------------------------------------+
| Filename: util.h
| Author: lovepsone
+--------------------------------------------------------+
| Removal of this copyright header is strictly prohibited 
| without written permission from the original author(s).
+--------------------------------------------------------*/

#include "Common.h"

bool Utf8toWStr(char const* utf8str, size_t csize, wchar_t* wstr, size_t& wsize);
bool Utf8toWStr(const std::string& utf8str, std::wstring& wstr);
void vutf8printf(FILE *out, const char *str, va_list* ap);