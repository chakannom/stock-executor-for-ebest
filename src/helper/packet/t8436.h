#ifndef _t8436_H_
#define _t8436_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 주식종목조회 API용 ( BLOCK,HEADTYPE=A )
#pragma pack( push, 1 )

#define NAME_t8436     "t8436"

// 기본입력
typedef struct _t8436InBlock
{
    char    gubun               [   1];    // [string,    1] 구분(0:전체1:코스피2:코스닥)    StartPos 0, Length 1
} t8436InBlock, *LPt8436InBlock;
#define NAME_t8436InBlock     "t8436InBlock"

// 출력1                          , occurs
typedef struct _t8436OutBlock
{
    char    hname               [  20];    // [string,   20] 종목명                          StartPos 0, Length 20
    char    shcode              [   6];    // [string,    6] 단축코드                        StartPos 20, Length 6
    char    expcode             [  12];    // [string,   12] 확장코드                        StartPos 26, Length 12
    char    etfgubun            [   1];    // [string,    1] ETF구분(1:ETF2:ETN)             StartPos 38, Length 1
    char    uplmtprice          [   8];    // [long  ,    8] 상한가                          StartPos 39, Length 8
    char    dnlmtprice          [   8];    // [long  ,    8] 하한가                          StartPos 47, Length 8
    char    jnilclose           [   8];    // [long  ,    8] 전일가                          StartPos 55, Length 8
    char    memedan             [   5];    // [string,    5] 주문수량단위                    StartPos 63, Length 5
    char    recprice            [   8];    // [long  ,    8] 기준가                          StartPos 68, Length 8
    char    gubun               [   1];    // [string,    1] 구분(1:코스피2:코스닥)          StartPos 76, Length 1
    char    bu12gubun           [   2];    // [string,    2] 증권그룹                        StartPos 77, Length 2
    char    spac_gubun          [   1];    // [string,    1] 기업인수목적회사여부(Y/N)       StartPos 79, Length 1
    char    filler              [  32];    // [string,   32] filler(미사용)                  StartPos 80, Length 32
} t8436OutBlock, *LPt8436OutBlock;
#define NAME_t8436OutBlock     "t8436OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _t8436_H_
