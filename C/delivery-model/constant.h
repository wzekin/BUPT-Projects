#include <Windows.h>
#pragma once

const int X = GetPrivateProfileInt(L"init", L"x", 8, L"./init.ini"); // ��ʼx
const int Y = GetPrivateProfileInt(L"init", L"y", 9, L"./init.ini"); // ��ʼy

const int INIT_MONEY =
    GetPrivateProfileInt(L"init", L"init_money", 1000, L"./init.ini"); //�����Ǯ
const int OUTDATE_TIME =
    GetPrivateProfileInt(L"init", L"outdate_time", 30, L"./init.ini"); //��ʱʱ��
const int ILLEGAL_TIME =
    GetPrivateProfileInt(L"init", L"illegal_time", 60, L"./init.ini"); //�ϵ�ʱ��
