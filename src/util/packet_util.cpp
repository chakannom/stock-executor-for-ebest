#include <afx.h>
#include "packet_util.h"

BOOL g_bShowRaw = FALSE;

//--------------------------------------------------------------------------------------
// Packet 데이터를 String 데이터로 변환한다.
//--------------------------------------------------------------------------------------
CStringA GetStringAData(char* psData, int nSize, int nType, int nDotPos)
{
    CStringA strData(psData, nSize);

    // Raw 데이터를 보여주지 않을때는 SPACE, 숫자앞의 0, 그리고 소숫점을 추가한다.
    if (g_bShowRaw == FALSE)
    {
        //-----------------------------------------------------------------------
        // 문자열 : 뒷부분의 Space 제거
        if (nType == DATA_TYPE_STRING)
        {
            strData.TrimRight();
        }

        //-----------------------------------------------------------------------
        // 정수 : 앞의 0 을 제거. 쉽게 하기 위해 정수로 변환후 다시 문자로 변환
        else if (nType == DATA_TYPE_LONG)
        {
            CStringA strTemp;
            strTemp.Format("%d", atoi(strData));
            strData = strTemp;
        }

        //-----------------------------------------------------------------------
        // 실수 : 앞의 0 을 제거. 쉽게 하기 위해 실수로 변환후 다시 nDotPos 만큼 표시
        else if (nType == DATA_TYPE_FLOAT)
        {
            strData.TrimRight();
            // 소숫점이 찍히지 않은 데이터이므로 nDotPos 위치에 점을 찍는다.
            strData.Insert(strData.GetLength() - nDotPos, '.');

            // 소숫점 이하를 nDotPos 만큼 표시하기 위해 format을 만든다.
            CStringA strFormat;
            strFormat.Format("%%.%df", nDotPos);
            CStringA strTemp;
            strTemp.Format(strFormat, atof(strData));
            strData = strTemp;
        }

        //-----------------------------------------------------------------------
        // 실수 : 앞의 0 을 제거. 쉽게 하기 위해 실수로 변환후 다시 nDotPos 만큼 표시
        else if (nType == DATA_TYPE_FLOAT_DOT)
        {
            // 소숫점이 찍혀있다
            int nFind = strData.Find(".");
            nDotPos = strData.GetLength() - nFind - 1;

            // 소숫점 이하를 nDotPos 만큼 표시하기 위해 format을 만든다.
            CStringA strFormat;
            strFormat.Format("%%.%df", nDotPos);
            CStringA strTemp;
            strTemp.Format(strFormat, atof(strData));
            strData = strTemp;
        }
    }

    return strData;
}

CStringW GetStringWData(char* psData, int nSize, int nType, int nDotPos)
{
    return CStringW(GetStringAData(psData, nSize, nType, nDotPos));
}

long GetLongData(char* psData, int nSize, int nType, int nDotPos)
{
    return strtol(GetStringAData(psData, nSize, nType, nDotPos), nullptr, 10);
}

void SetPacketData(char* psData, int nSize, LPCSTR pszSrc, int nType, int nDotPos)
{
    //-----------------------------------------------------------------------
    // 문자열
    if (nType == DATA_TYPE_STRING)
    {
        // 왼쪽 정렬
        // 뒤의 빈자리는 ' ' 로 채움

        // 버퍼크기가 원 데이터보다 작다면 데이터는 짤려야 하므로 에러발생
        int nSrcLen = strlen(pszSrc);

        // 먼저 Space를 채우고
        FillMemory(psData, nSize, ' ');

        // 앞에부터 데이터를 넣는다.
        // 원데이터가 크다면 뒷부분을 버린다.
        CopyMemory(psData, pszSrc, min(nSize, nSrcLen));
    }

    //-----------------------------------------------------------------------
    // 정수
    else if (nType == DATA_TYPE_LONG)
    {
        // 오른쪽 정렬
        // 앞의 빈자리는 '0' 으로 채움

        // 버퍼크기가 원 데이터보다 작다면 데이터는 짤려야 하므로 에러발생
        int nSrcLen = strlen(pszSrc);
        ASSERT(nSize >= nSrcLen);

        // 먼저 0 으로 채우고
        FillMemory(psData, nSize, '0');

        // 뒤에서부터 데이터를 넣는다.
        if (nSize >= nSrcLen)
        {
            CopyMemory(psData + nSize - nSrcLen, pszSrc, nSrcLen);
        }
        // 원데이터가 크다면 원데이터의 뒷부분을 버린다.
        else
        {
            CopyMemory(psData, pszSrc, nSize);
        }
    }

    //-----------------------------------------------------------------------
    // 실수 : 소숫점을 찍지 않는다.
    else if (nType == DATA_TYPE_FLOAT)
    {
        // 소숫점 위치를 기준으로 정렬
        // 소숫점을 찍지 않으며 정수부의 빈자리와 소수부의 빈자리는 0으로 채움

        int nSrcLen = strlen(pszSrc);

        // 먼저 0 으로 채우고
        FillMemory(psData, nSize, '0');

        // 원데이터에서 소숫점의 위치를 찾아서
        // 원데이터의 정수부의 길이와 소수부의 길이를 구한다.
        int nSrcIntLen;
        int nSrcDotLen;
        LPCSTR psz = strchr(pszSrc, '.');
        if (psz == NULL)		// 소수부가 없다.
        {
            nSrcIntLen = strlen(pszSrc);
            nSrcDotLen = 0;
        }
        else					// 소수부가 있다.
        {
            nSrcIntLen = psz - pszSrc;
            nSrcDotLen = strlen(pszSrc) - nSrcIntLen - 1;
        }

        // 정수부를 넣는다.
        if (nSize - nDotPos >= nSrcIntLen)
        {
            CopyMemory(psData + nSize - nDotPos - nSrcIntLen, pszSrc, nSrcIntLen);
        }
        else
        {
            // 원데이터의 정수부 길이가 더 긴 경우 정수부의 뒷자리는 삭제된다.
            ASSERT(FALSE);
            CopyMemory(psData, pszSrc, nSize - nDotPos);
        }

        // 소수부를 넣는데 원데이터의 소수부 길이가 더 긴 경우 소수부의 뒷자리는 삭제된다.
        ASSERT(nDotPos >= nSrcDotLen);
        CopyMemory(psData + nSize - nDotPos, pszSrc + strlen(pszSrc) - nSrcDotLen, min(nDotPos, nSrcDotLen));
    }

    //-----------------------------------------------------------------------
    // 실수 : 소숫점을 포함
    else if (nType == DATA_TYPE_FLOAT_DOT)
    {
        // 소숫점 위치를 기준으로 정렬
        // 소숫점을 찍지 않으며 정수부의 빈자리와 소수부의 빈자리는 0으로 채움

        int nSrcLen = strlen(pszSrc);

        // 먼저 0 으로 채우고
        FillMemory(psData, nSize, '0');

        // 원데이터에서 소숫점의 위치를 찾아서
        // 원데이터의 정수부의 길이와 소수부의 길이를 구한다.
        int nSrcIntLen;
        int nSrcDotLen;
        LPCSTR psz = strchr(pszSrc, '.');
        if (psz == NULL)		// 소수부가 없다.
        {
            nSrcIntLen = strlen(pszSrc);
            nSrcDotLen = 0;
        }
        else					// 소수부가 있다.
        {
            nSrcIntLen = psz - pszSrc;
            nSrcDotLen = strlen(pszSrc) - nSrcIntLen - 1;
        }

        // 정수부를 넣는다.
        if (nSize - nDotPos - 1 >= nSrcIntLen)
        {
            CopyMemory(psData + nSize - nDotPos - nSrcIntLen - 1, pszSrc, nSrcIntLen);
        }
        else
        {
            // 원데이터의 정수부 길이가 더 긴 경우 정수부의 뒷자리는 삭제된다.
            ASSERT(FALSE);
            CopyMemory(psData, pszSrc, nSize - nDotPos - 1);
        }

        // 소숫점을 찍는다.
        psData[nSize - nDotPos - 1] = '.';

        // 소수부를 넣는데 원데이터의 소수부 길이가 더 긴 경우 소수부의 뒷자리는 삭제된다.
        ASSERT(nDotPos >= nSrcDotLen);
        CopyMemory(psData + nSize - nDotPos, pszSrc + strlen(pszSrc) - nSrcDotLen, min(nDotPos, nSrcDotLen));
    }
}