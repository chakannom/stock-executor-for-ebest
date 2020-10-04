#pragma once
#pragma warning(disable: 4996)

#include <afxwin.h>

class CStringUtil
{
public:
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //	구조체 필드값을 문자열 형태로 변환하는 유틸리티 함수입니다.
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    static CStringA _scopy_a(const char* szData, int nSize)
    {
        char szBuf[512]; //필드의 최대 크기는 상황에 따라 조절할 필요가 있음
        memset(szBuf, 0, sizeof szBuf);
        strncpy(szBuf, szData, nSize);

        return szBuf;
    };

    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //	문자열을 구조체 필드로 적절히 채우는 유틸리티 함수입니다. (넘치면 잘림)
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    static void _smove_a(char* szTarget, int nSize, CStringA strSource)
    {
        int nMin = min(nSize, strSource.GetLength());

        memset(szTarget, ' ', nSize);
        strncpy(szTarget, strSource, nMin);
    };

    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //	 숫자에 천 단위로 컴마 삽입하는 유틸리티 함수입니다.
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    static CStringA _comma_a(const char* szData, int nSize, int decimal = 0)
    {
        CStringA strInput = _scopy_a(szData, nSize);
        strInput.TrimLeft();
        strInput.TrimRight();

        double fInput = atof(strInput);

        CStringA strTemp;

        if (fInput >= 1000000000.0F)
            strTemp.Insert(strTemp.GetLength() - 9, ',');
        if (fInput >= 1000000.0F)
            strTemp.Insert(strTemp.GetLength() - 6, ',');
        if (fInput >= 1000.0F)
            strTemp.Insert(strTemp.GetLength() - 3, ',');

        CStringA strOutput;
        strOutput.Format("%*s", nSize, strTemp);

        return strOutput;
    };
};

#define SCOPY_A(x) CStringUtil::_scopy_a(x, sizeof x)
#define SMOVE_A(fTarget, strSource) CStringUtil::_smove_a(fTarget, sizeof fTarget, strSource)
#define COMMA_A(x) CStringUtil::_comma_a(x, sizeof x)
#define COMMAF_A(x, d) CStringUtil::_comma_a(x, sizeof x, d)