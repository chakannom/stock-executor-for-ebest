#pragma once
enum
{
    DATA_TYPE_STRING = 0,		// 문자열
    DATA_TYPE_LONG,				// 정수
    DATA_TYPE_FLOAT,			// 실수
    DATA_TYPE_FLOAT_DOT,		// 실수( 소숫점을 가지고 있음 )
};
#define DATA_TYPE_DOUBLE	DATA_TYPE_FLOAT
#define DATA_TYPE_DOT		DATA_TYPE_FLOAT_DOT

CStringA GetStringAData(char* psData, int nSize, int nType, int nDotPos = 0);
CStringW GetStringWData(char* psData, int nSize, int nType, int nDotPos = 0);
long GetLongData(char* psData, int nSize, int nType, int nDotPos = 0);
void SetPacketData(char* psData, int nSize, LPCSTR pszSrc, int nType, int nDotPos = 0);