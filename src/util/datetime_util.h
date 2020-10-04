#pragma once
#include <time.h>
#include <iostream>
class CDateTimeUtil
{
public:
    static time_t GetEpochMilli(const char* szDate)
    {
        int year, month, day, hour, minute, second;
        sscanf_s(szDate, "%4d%2d%2d%2d%2d%2d", &year, &month, &day, &hour, &minute, &second);

        struct tm tm_date;
        tm_date.tm_year = year - 1900;
        tm_date.tm_mon = month - 1;
        tm_date.tm_mday = day;
        tm_date.tm_hour = hour;
        tm_date.tm_min = minute;
        tm_date.tm_sec = second;
        tm_date.tm_isdst = -1;

        return mktime(&tm_date) * 1000;
    };
};