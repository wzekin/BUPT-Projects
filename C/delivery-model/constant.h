#include <Windows.h>
#pragma once

const int X = GetPrivateProfileInt(L"init", L"x", 8, L"./init.ini"); // 初始x
const int Y = GetPrivateProfileInt(L"init", L"y", 9, L"./init.ini"); // 初始y

const int INIT_MONEY =
    GetPrivateProfileInt(L"init", L"init_money", 1000, L"./init.ini"); //最初金钱
const int OUTDATE_TIME =
    GetPrivateProfileInt(L"init", L"outdate_time", 30, L"./init.ini"); //超时时间
const int ILLEGAL_TIME =
    GetPrivateProfileInt(L"init", L"illegal_time", 60, L"./init.ini"); //废单时间
