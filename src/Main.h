/*
*
*/

#pragma once
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>


const int UPDATEFIELDS_BUILD                            = 7799;


const int   AUTH_OK                              = 0;
const int   AUTH_BANNED                          = 3; // 3 - The World of Warcraft account has been closed and no longer in service
const int   AUTH_NOT_VALID                       = 4; // 4,5 - The information you have entered is not valid
const int   AUTH_NOT_VALID5                      = 5;
const int   AUTH_ALREADY_LOGGED                  = 6; // 6 - This account is already logged into World of Warcraft
const int   AUTH_NO_PREPAID_TIME                 = 7; // 7 - you have used up your prepaid time for this account. please purchase more to continue playing
const int   AUTH_TRY_LATER                       = 8; // 8 - could not login into wow at this time. please try again letter
const int   AUTH_INVALID_VERSION                 = 9; // 9 - invalid version
const int   AUTH_DOWNLOAD_FILE                   = 10;
const int   AUTH_SUSPENDED                       = 12; // 12 - this account has been suspended
const int   AUTH_BLOCKED_BY_PARENT               = 15; // 15 access to this account hase been blocked by parentals control


// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
