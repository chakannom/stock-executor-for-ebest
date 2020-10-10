#include "core/framework.h"
#include "packet/t1101.h"
#include "packet/t1102.h"
#include "packet/t8436.h"
#include "response_supporter.h"

web::json::value CResponseSupporter::GetLoginInformation()
{
    web::json::value json = web::json::value::object();

    // data.loginDate: 로그인 성공 시간 (Milliseconds)
    json[L"loginDate"] = web::json::value::number(time(0) * 1000);

    return json;
}

web::json::value CResponseSupporter::GetLoginStatus(bool isLogin)
{
    web::json::value json = web::json::value::object();

    json[L"status"] = web::json::value::boolean(isLogin);

    return json;
}

web::json::value CResponseSupporter::GetStocksByGubun(LPt8436OutBlock pT8436OutBlock, int nBlockCount)
{
    web::json::value json = web::json::value::object();

    for (int i = 0; i < nBlockCount; i++) {
        // data.stocks[].hname: 종목명
        CStringW hname = GetStringWData(pT8436OutBlock[i].hname, sizeof(pT8436OutBlock[i].hname), DATA_TYPE_STRING);
        json[L"stocks"][i][L"hname"] = web::json::value::string(hname.GetString());
        // data.stocks[].shcode: 단축코드
        CStringW shcode = GetStringWData(pT8436OutBlock[i].shcode, sizeof(pT8436OutBlock[i].shcode), DATA_TYPE_STRING);
        json[L"stocks"][i][L"shcode"] = web::json::value::string(shcode.GetString());
        // data.stocks[].expcode: 확장코드
        CStringW expcode = GetStringWData(pT8436OutBlock[i].expcode, sizeof(pT8436OutBlock[i].expcode), DATA_TYPE_STRING);
        json[L"stocks"][i][L"expcode"] = web::json::value::string(expcode.GetString());
        // data.stocks[].etfgubun: ETF구분(1:ETF2:ETN)
        CStringW etfgubun = GetStringWData(pT8436OutBlock[i].etfgubun, sizeof(pT8436OutBlock[i].etfgubun), DATA_TYPE_STRING);
        json[L"stocks"][i][L"etfgubun"] = web::json::value::string(etfgubun.GetString());
        // data.stocks[].uplmtprice: 상한가
        long uplmtprice = GetLongData(pT8436OutBlock[i].uplmtprice, sizeof(pT8436OutBlock[i].uplmtprice), DATA_TYPE_LONG);
        json[L"stocks"][i][L"uplmtprice"] = web::json::value::number(uplmtprice);
        // data.stocks[].dnlmtprice: 하한가
        long dnlmtprice = GetLongData(pT8436OutBlock[i].dnlmtprice, sizeof(pT8436OutBlock[i].dnlmtprice), DATA_TYPE_LONG);
        json[L"stocks"][i][L"dnlmtprice"] = web::json::value::number(dnlmtprice);
        // data.stocks[].jnilclose: 전일가
        long jnilclose = GetLongData(pT8436OutBlock[i].jnilclose, sizeof(pT8436OutBlock[i].jnilclose), DATA_TYPE_LONG);
        json[L"stocks"][i][L"jnilclose"] = web::json::value::number(jnilclose);
        // data.stocks[].memedan: 주문수량단위
        CStringW memedan = GetStringWData(pT8436OutBlock[i].memedan, sizeof(pT8436OutBlock[i].memedan), DATA_TYPE_STRING);
        json[L"stocks"][i][L"memedan"] = web::json::value::string(memedan.GetString());
        // data.stocks[].recprice: 기준가
        long recprice = GetLongData(pT8436OutBlock[i].recprice, sizeof(pT8436OutBlock[i].recprice), DATA_TYPE_LONG);
        json[L"stocks"][i][L"recprice"] = web::json::value::number(recprice);
        // data.stocks[].gubun: 구분(1:코스피2:코스닥)
        CStringW gubun = GetStringWData(pT8436OutBlock[i].gubun, sizeof(pT8436OutBlock[i].gubun), DATA_TYPE_STRING);
        json[L"stocks"][i][L"gubun"] = web::json::value::string(gubun.GetString());
        // data.stocks[].bu12gubun: 증권그룹
        CStringW bu12gubun = GetStringWData(pT8436OutBlock[i].bu12gubun, sizeof(pT8436OutBlock[i].bu12gubun), DATA_TYPE_STRING);
        json[L"stocks"][i][L"bu12gubun"] = web::json::value::string(bu12gubun.GetString());
        // data.stocks[].spacGubun: 기업인수목적회사여부(Y/N)
        CStringW spacgubun = GetStringWData(pT8436OutBlock[i].spac_gubun, sizeof(pT8436OutBlock[i].spac_gubun), DATA_TYPE_STRING);
        json[L"stocks"][i][L"spacgubun"] = web::json::value::string(spacgubun.GetString());
        // data.stocks[].filler: filler(미사용)
        CStringW filler = GetStringWData(pT8436OutBlock[i].filler, sizeof(pT8436OutBlock[i].filler), DATA_TYPE_STRING);
        //json[L"stocks"][i][L"filler"] = web::json::value::string(filler.GetString());
    }
    json[L"totalCount"] = web::json::value::number(nBlockCount);

    return json;
}

web::json::value CResponseSupporter::GetStockCurrentAskingPriceByCode(LPt1101OutBlock pT1101OutBlock)
{
    web::json::value json = web::json::value::object();

    // data.stock.hname: 한글명
    CStringW hname = GetStringWData(pT1101OutBlock->hname, sizeof(pT1101OutBlock->hname), DATA_TYPE_STRING);
    json[L"stock"][L"hname"] = web::json::value::string(hname.GetString());
    // data.stock.price: 현재가
    long price = GetLongData(pT1101OutBlock->price, sizeof(pT1101OutBlock->price), DATA_TYPE_LONG);
    json[L"stock"][L"price"] = web::json::value::number(price);
    // data.stock.sign: 전일대비구분
    CStringW sign = GetStringWData(pT1101OutBlock->sign, sizeof(pT1101OutBlock->sign), DATA_TYPE_STRING);
    json[L"stock"][L"sign"] = web::json::value::string(sign.GetString());
    // data.stock.change: 전일대비
    long change = GetLongData(pT1101OutBlock->change, sizeof(pT1101OutBlock->change), DATA_TYPE_LONG);
    json[L"stock"][L"change"] = web::json::value::number(change);
    // data.stock.diff: 등락율
    float diff = GetFloatData(pT1101OutBlock->diff, sizeof(pT1101OutBlock->diff), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"diff"] = web::json::value::number(diff);
    // data.stock.volume: 누적거래량
    long volume = GetLongData(pT1101OutBlock->volume, sizeof(pT1101OutBlock->volume), DATA_TYPE_LONG);
    json[L"stock"][L"volume"] = web::json::value::number(volume);
    // data.stock.jnilclose: 전일종가
    long jnilclose = GetLongData(pT1101OutBlock->jnilclose, sizeof(pT1101OutBlock->jnilclose), DATA_TYPE_LONG);
    json[L"stock"][L"jnilclose"] = web::json::value::number(jnilclose);
    // data.stock.offerho1: 매도호가1
    long offerho1 = GetLongData(pT1101OutBlock->offerho1, sizeof(pT1101OutBlock->offerho1), DATA_TYPE_LONG);
    json[L"stock"][L"offerho1"] = web::json::value::number(offerho1);
    // data.stock.bidho1: 매수호가1
    long bidho1 = GetLongData(pT1101OutBlock->bidho1, sizeof(pT1101OutBlock->bidho1), DATA_TYPE_LONG);
    json[L"stock"][L"bidho1"] = web::json::value::number(bidho1);
    // data.stock.offerrem1: 매도호가수량1
    long offerrem1 = GetLongData(pT1101OutBlock->offerrem1, sizeof(pT1101OutBlock->offerrem1), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem1"] = web::json::value::number(offerrem1);
    // data.stock.bidrem1: 매수호가수량1
    long bidrem1 = GetLongData(pT1101OutBlock->bidrem1, sizeof(pT1101OutBlock->bidrem1), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem1"] = web::json::value::number(bidrem1);
    // data.stock.preoffercha1: 직전매도대비수량1
    long preoffercha1 = GetLongData(pT1101OutBlock->preoffercha1, sizeof(pT1101OutBlock->preoffercha1), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha1"] = web::json::value::number(preoffercha1);
    // data.stock.prebidcha1: 직전매수대비수량1
    long prebidcha1 = GetLongData(pT1101OutBlock->prebidcha1, sizeof(pT1101OutBlock->prebidcha1), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha1"] = web::json::value::number(prebidcha1);
    // data.stock.offerho2: 매도호가2
    long offerho2 = GetLongData(pT1101OutBlock->offerho2, sizeof(pT1101OutBlock->offerho2), DATA_TYPE_LONG);
    json[L"stock"][L"offerho2"] = web::json::value::number(offerho2);
    // data.stock.bidho2: 매수호가2
    long bidho2 = GetLongData(pT1101OutBlock->bidho2, sizeof(pT1101OutBlock->bidho2), DATA_TYPE_LONG);
    json[L"stock"][L"bidho2"] = web::json::value::number(bidho2);
    // data.stock.offerrem2: 매도호가수량2
    long offerrem2 = GetLongData(pT1101OutBlock->offerrem2, sizeof(pT1101OutBlock->offerrem2), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem2"] = web::json::value::number(offerrem2);
    // data.stock.bidrem2: 매수호가수량2
    long bidrem2 = GetLongData(pT1101OutBlock->bidrem2, sizeof(pT1101OutBlock->bidrem2), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem2"] = web::json::value::number(bidrem2);
    // data.stock.preoffercha2: 직전매도대비수량2
    long preoffercha2 = GetLongData(pT1101OutBlock->preoffercha2, sizeof(pT1101OutBlock->preoffercha2), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha2"] = web::json::value::number(preoffercha2);
    // data.stock.prebidcha2: 직전매수대비수량2
    long prebidcha2 = GetLongData(pT1101OutBlock->prebidcha2, sizeof(pT1101OutBlock->prebidcha2), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha2"] = web::json::value::number(prebidcha2);
    // data.stock.offerho3: 매도호가3
    long offerho3 = GetLongData(pT1101OutBlock->offerho3, sizeof(pT1101OutBlock->offerho3), DATA_TYPE_LONG);
    json[L"stock"][L"offerho3"] = web::json::value::number(offerho3);
    // data.stock.bidho3: 매수호가3
    long bidho3 = GetLongData(pT1101OutBlock->bidho3, sizeof(pT1101OutBlock->bidho3), DATA_TYPE_LONG);
    json[L"stock"][L"bidho3"] = web::json::value::number(bidho3);
    // data.stock.offerrem3: 매도호가수량3
    long offerrem3 = GetLongData(pT1101OutBlock->offerrem3, sizeof(pT1101OutBlock->offerrem3), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem3"] = web::json::value::number(offerrem3);
    // data.stock.bidrem3: 매수호가수량3
    long bidrem3 = GetLongData(pT1101OutBlock->bidrem3, sizeof(pT1101OutBlock->bidrem3), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem3"] = web::json::value::number(bidrem3);
    // data.stock.preoffercha3: 직전매도대비수량3
    long preoffercha3 = GetLongData(pT1101OutBlock->preoffercha3, sizeof(pT1101OutBlock->preoffercha3), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha3"] = web::json::value::number(preoffercha3);
    // data.stock.prebidcha3: 직전매수대비수량3
    long prebidcha3 = GetLongData(pT1101OutBlock->prebidcha3, sizeof(pT1101OutBlock->prebidcha3), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha3"] = web::json::value::number(prebidcha3);
    // data.stock.offerho4: 매도호가4
    long offerho4 = GetLongData(pT1101OutBlock->offerho4, sizeof(pT1101OutBlock->offerho4), DATA_TYPE_LONG);
    json[L"stock"][L"offerho4"] = web::json::value::number(offerho4);
    // data.stock.bidho4: 매수호가4
    long bidho4 = GetLongData(pT1101OutBlock->bidho4, sizeof(pT1101OutBlock->bidho4), DATA_TYPE_LONG);
    json[L"stock"][L"bidho4"] = web::json::value::number(bidho4);
    // data.stock.offerrem4: 매도호가수량4
    long offerrem4 = GetLongData(pT1101OutBlock->offerrem4, sizeof(pT1101OutBlock->offerrem4), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem4"] = web::json::value::number(offerrem4);
    // data.stock.bidrem4: 매수호가수량4
    long bidrem4 = GetLongData(pT1101OutBlock->bidrem4, sizeof(pT1101OutBlock->bidrem4), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem4"] = web::json::value::number(bidrem4);
    // data.stock.preoffercha4: 직전매도대비수량4
    long preoffercha4 = GetLongData(pT1101OutBlock->preoffercha4, sizeof(pT1101OutBlock->preoffercha4), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha4"] = web::json::value::number(preoffercha4);
    // data.stock.prebidcha4: 직전매수대비수량4
    long prebidcha4 = GetLongData(pT1101OutBlock->prebidcha4, sizeof(pT1101OutBlock->prebidcha4), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha4"] = web::json::value::number(prebidcha4);
    // data.stock.offerho5: 매도호가5
    long offerho5 = GetLongData(pT1101OutBlock->offerho5, sizeof(pT1101OutBlock->offerho5), DATA_TYPE_LONG);
    json[L"stock"][L"offerho5"] = web::json::value::number(offerho5);
    // data.stock.bidho5: 매수호가5
    long bidho5 = GetLongData(pT1101OutBlock->bidho5, sizeof(pT1101OutBlock->bidho5), DATA_TYPE_LONG);
    json[L"stock"][L"bidho5"] = web::json::value::number(bidho5);
    // data.stock.offerrem5: 매도호가수량5
    long offerrem5 = GetLongData(pT1101OutBlock->offerrem5, sizeof(pT1101OutBlock->offerrem5), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem5"] = web::json::value::number(offerrem5);
    // data.stock.bidrem5: 매수호가수량5
    long bidrem5 = GetLongData(pT1101OutBlock->bidrem5, sizeof(pT1101OutBlock->bidrem5), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem5"] = web::json::value::number(bidrem5);
    // data.stock.preoffercha5: 직전매도대비수량5
    long preoffercha5 = GetLongData(pT1101OutBlock->preoffercha5, sizeof(pT1101OutBlock->preoffercha5), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha5"] = web::json::value::number(preoffercha5);
    // data.stock.prebidcha5: 직전매수대비수량5
    long prebidcha5 = GetLongData(pT1101OutBlock->prebidcha5, sizeof(pT1101OutBlock->prebidcha5), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha5"] = web::json::value::number(prebidcha5);
    // data.stock.offerho6: 매도호가6
    long offerho6 = GetLongData(pT1101OutBlock->offerho6, sizeof(pT1101OutBlock->offerho6), DATA_TYPE_LONG);
    json[L"stock"][L"offerho6"] = web::json::value::number(offerho6);
    // data.stock.bidho6: 매수호가6
    long bidho6 = GetLongData(pT1101OutBlock->bidho6, sizeof(pT1101OutBlock->bidho6), DATA_TYPE_LONG);
    json[L"stock"][L"bidho6"] = web::json::value::number(bidho6);
    // data.stock.offerrem6: 매도호가수량6
    long offerrem6 = GetLongData(pT1101OutBlock->offerrem6, sizeof(pT1101OutBlock->offerrem6), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem6"] = web::json::value::number(offerrem6);
    // data.stock.bidrem6: 매수호가수량6
    long bidrem6 = GetLongData(pT1101OutBlock->bidrem6, sizeof(pT1101OutBlock->bidrem6), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem6"] = web::json::value::number(bidrem6);
    // data.stock.preoffercha6: 직전매도대비수량6
    long preoffercha6 = GetLongData(pT1101OutBlock->preoffercha6, sizeof(pT1101OutBlock->preoffercha6), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha6"] = web::json::value::number(preoffercha6);
    // data.stock.prebidcha6: 직전매수대비수량6
    long prebidcha6 = GetLongData(pT1101OutBlock->prebidcha6, sizeof(pT1101OutBlock->prebidcha6), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha6"] = web::json::value::number(prebidcha6);
    // data.stock.offerho7: 매도호가7
    long offerho7 = GetLongData(pT1101OutBlock->offerho7, sizeof(pT1101OutBlock->offerho7), DATA_TYPE_LONG);
    json[L"stock"][L"offerho7"] = web::json::value::number(offerho7);
    // data.stock.bidho7: 매수호가7
    long bidho7 = GetLongData(pT1101OutBlock->bidho7, sizeof(pT1101OutBlock->bidho7), DATA_TYPE_LONG);
    json[L"stock"][L"bidho7"] = web::json::value::number(bidho7);
    // data.stock.offerrem7: 매도호가수량7
    long offerrem7 = GetLongData(pT1101OutBlock->offerrem7, sizeof(pT1101OutBlock->offerrem7), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem7"] = web::json::value::number(offerrem7);
    // data.stock.bidrem7: 매수호가수량7
    long bidrem7 = GetLongData(pT1101OutBlock->bidrem7, sizeof(pT1101OutBlock->bidrem7), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem7"] = web::json::value::number(bidrem7);
    // data.stock.preoffercha7: 직전매도대비수량7
    long preoffercha7 = GetLongData(pT1101OutBlock->preoffercha7, sizeof(pT1101OutBlock->preoffercha7), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha7"] = web::json::value::number(preoffercha7);
    // data.stock.prebidcha7: 직전매수대비수량7
    long prebidcha7 = GetLongData(pT1101OutBlock->prebidcha7, sizeof(pT1101OutBlock->prebidcha7), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha7"] = web::json::value::number(prebidcha7);
    // data.stock.offerho8: 매도호가8
    long offerho8 = GetLongData(pT1101OutBlock->offerho8, sizeof(pT1101OutBlock->offerho8), DATA_TYPE_LONG);
    json[L"stock"][L"offerho8"] = web::json::value::number(offerho8);
    // data.stock.bidho8: 매수호가8
    long bidho8 = GetLongData(pT1101OutBlock->bidho8, sizeof(pT1101OutBlock->bidho8), DATA_TYPE_LONG);
    json[L"stock"][L"bidho8"] = web::json::value::number(bidho8);
    // data.stock.offerrem8: 매도호가수량8
    long offerrem8 = GetLongData(pT1101OutBlock->offerrem8, sizeof(pT1101OutBlock->offerrem8), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem8"] = web::json::value::number(offerrem8);
    // data.stock.bidrem8: 매수호가수량8
    long bidrem8 = GetLongData(pT1101OutBlock->bidrem8, sizeof(pT1101OutBlock->bidrem8), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem8"] = web::json::value::number(bidrem8);
    // data.stock.preoffercha8: 직전매도대비수량8
    long preoffercha8 = GetLongData(pT1101OutBlock->preoffercha8, sizeof(pT1101OutBlock->preoffercha8), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha8"] = web::json::value::number(preoffercha8);
    // data.stock.prebidcha8: 직전매수대비수량8
    long prebidcha8 = GetLongData(pT1101OutBlock->prebidcha8, sizeof(pT1101OutBlock->prebidcha8), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha8"] = web::json::value::number(prebidcha8);
    // data.stock.offerho9: 매도호가9
    long offerho9 = GetLongData(pT1101OutBlock->offerho9, sizeof(pT1101OutBlock->offerho9), DATA_TYPE_LONG);
    json[L"stock"][L"offerho9"] = web::json::value::number(offerho9);
    // data.stock.bidho9: 매수호가9
    long bidho9 = GetLongData(pT1101OutBlock->bidho9, sizeof(pT1101OutBlock->bidho9), DATA_TYPE_LONG);
    json[L"stock"][L"bidho9"] = web::json::value::number(bidho9);
    // data.stock.offerrem9: 매도호가수량9
    long offerrem9 = GetLongData(pT1101OutBlock->offerrem9, sizeof(pT1101OutBlock->offerrem9), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem9"] = web::json::value::number(offerrem9);
    // data.stock.bidrem9: 매수호가수량9
    long bidrem9 = GetLongData(pT1101OutBlock->bidrem9, sizeof(pT1101OutBlock->bidrem9), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem9"] = web::json::value::number(bidrem9);
    // data.stock.preoffercha9: 직전매도대비수량9
    long preoffercha9 = GetLongData(pT1101OutBlock->preoffercha9, sizeof(pT1101OutBlock->preoffercha9), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha9"] = web::json::value::number(preoffercha9);
    // data.stock.prebidcha9: 직전매수대비수량9
    long prebidcha9 = GetLongData(pT1101OutBlock->prebidcha9, sizeof(pT1101OutBlock->prebidcha9), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha9"] = web::json::value::number(prebidcha9);
    // data.stock.offerho10: 매도호가10
    long offerho10 = GetLongData(pT1101OutBlock->offerho10, sizeof(pT1101OutBlock->offerho10), DATA_TYPE_LONG);
    json[L"stock"][L"offerho10"] = web::json::value::number(offerho10);
    // data.stock.bidho10: 매수호가10
    long bidho10 = GetLongData(pT1101OutBlock->bidho10, sizeof(pT1101OutBlock->bidho10), DATA_TYPE_LONG);
    json[L"stock"][L"bidho10"] = web::json::value::number(bidho10);
    // data.stock.offerrem10: 매도호가수량10
    long offerrem10 = GetLongData(pT1101OutBlock->offerrem10, sizeof(pT1101OutBlock->offerrem10), DATA_TYPE_LONG);
    json[L"stock"][L"offerrem10"] = web::json::value::number(offerrem10);
    // data.stock.bidrem10: 매수호가수량10
    long bidrem10 = GetLongData(pT1101OutBlock->bidrem10, sizeof(pT1101OutBlock->bidrem10), DATA_TYPE_LONG);
    json[L"stock"][L"bidrem10"] = web::json::value::number(bidrem10);
    // data.stock.preoffercha10: 직전매도대비수량10
    long preoffercha10 = GetLongData(pT1101OutBlock->preoffercha10, sizeof(pT1101OutBlock->preoffercha10), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha10"] = web::json::value::number(preoffercha10);
    // data.stock.prebidcha10: 직전매수대비수량10
    long prebidcha10 = GetLongData(pT1101OutBlock->prebidcha10, sizeof(pT1101OutBlock->prebidcha10), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha10"] = web::json::value::number(prebidcha10);
    // data.stock.offer: 매도호가수량합
    long offer = GetLongData(pT1101OutBlock->offer, sizeof(pT1101OutBlock->offer), DATA_TYPE_LONG);
    json[L"stock"][L"offer"] = web::json::value::number(offer);
    // data.stock.bid: 매수호가수량합
    long bid = GetLongData(pT1101OutBlock->bid, sizeof(pT1101OutBlock->bid), DATA_TYPE_LONG);
    json[L"stock"][L"bid"] = web::json::value::number(bid);
    // data.stock.preoffercha: 직전매도대비수량합
    long preoffercha = GetLongData(pT1101OutBlock->preoffercha, sizeof(pT1101OutBlock->preoffercha), DATA_TYPE_LONG);
    json[L"stock"][L"preoffercha"] = web::json::value::number(preoffercha);
    // data.stock.prebidcha: 직전매수대비수량합
    long prebidcha = GetLongData(pT1101OutBlock->prebidcha, sizeof(pT1101OutBlock->prebidcha), DATA_TYPE_LONG);
    json[L"stock"][L"prebidcha"] = web::json::value::number(prebidcha);
    // data.stock.hotime: 수신시간
    long hotime = GetLongData(pT1101OutBlock->hotime, sizeof(pT1101OutBlock->hotime), DATA_TYPE_LONG);
    json[L"stock"][L"hotime"] = web::json::value::number(hotime);
    // data.stock.yeprice: 예상체결가격
    long yeprice = GetLongData(pT1101OutBlock->yeprice, sizeof(pT1101OutBlock->yeprice), DATA_TYPE_LONG);
    json[L"stock"][L"yeprice"] = web::json::value::number(yeprice);
    // data.stock.yevolume: 예상체결수량
    long yevolume = GetLongData(pT1101OutBlock->yevolume, sizeof(pT1101OutBlock->yevolume), DATA_TYPE_LONG);
    json[L"stock"][L"yevolume"] = web::json::value::number(yevolume);
    // data.stock.yesign: 예상체결전일구분
    CStringW yesign = GetStringWData(pT1101OutBlock->yesign, sizeof(pT1101OutBlock->yesign), DATA_TYPE_STRING);
    json[L"stock"][L"yesign"] = web::json::value::string(yesign.GetString());
    // data.stock.yechange: 예상체결전일대비
    long yechange = GetLongData(pT1101OutBlock->yechange, sizeof(pT1101OutBlock->yechange), DATA_TYPE_LONG);
    json[L"stock"][L"yechange"] = web::json::value::number(yechange);
    // data.stock.yediff: 예상체결등락율
    float yediff = GetFloatData(pT1101OutBlock->yediff, sizeof(pT1101OutBlock->yediff), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"yediff"] = web::json::value::number(yediff);
    // data.stock.tmoffer: 시간외매도잔량
    long tmoffer = GetLongData(pT1101OutBlock->tmoffer, sizeof(pT1101OutBlock->tmoffer), DATA_TYPE_LONG);
    json[L"stock"][L"tmoffer"] = web::json::value::number(tmoffer);
    // data.stock.tmbid: 시간외매수잔량
    long tmbid = GetLongData(pT1101OutBlock->tmbid, sizeof(pT1101OutBlock->tmbid), DATA_TYPE_LONG);
    json[L"stock"][L"tmbid"] = web::json::value::number(tmbid);
    // data.stock.hostatus: 동시구분
    CStringW hostatus = GetStringWData(pT1101OutBlock->ho_status, sizeof(pT1101OutBlock->ho_status), DATA_TYPE_STRING);
    json[L"stock"][L"hostatus"] = web::json::value::string(hostatus.GetString());
    // data.stock.shcode: 단축코드
    CStringW shcode = GetStringWData(pT1101OutBlock->shcode, sizeof(pT1101OutBlock->shcode), DATA_TYPE_STRING);
    json[L"stock"][L"shcode"] = web::json::value::string(shcode.GetString());
    // data.stock.uplmtprice: 상한가
    long uplmtprice = GetLongData(pT1101OutBlock->uplmtprice, sizeof(pT1101OutBlock->uplmtprice), DATA_TYPE_LONG);
    json[L"stock"][L"uplmtprice"] = web::json::value::number(uplmtprice);
    // data.stock.dnlmtprice: 하한가
    long dnlmtprice = GetLongData(pT1101OutBlock->dnlmtprice, sizeof(pT1101OutBlock->dnlmtprice), DATA_TYPE_LONG);
    json[L"stock"][L"dnlmtprice"] = web::json::value::number(dnlmtprice);
    // data.stock.open: 시가
    long open = GetLongData(pT1101OutBlock->open, sizeof(pT1101OutBlock->open), DATA_TYPE_LONG);
    json[L"stock"][L"open"] = web::json::value::number(open);
    // data.stock.high: 고가
    long high = GetLongData(pT1101OutBlock->high, sizeof(pT1101OutBlock->high), DATA_TYPE_LONG);
    json[L"stock"][L"high"] = web::json::value::number(high);
    // data.stock.low: 저가
    long low = GetLongData(pT1101OutBlock->low, sizeof(pT1101OutBlock->low), DATA_TYPE_LONG);
    json[L"stock"][L"low"] = web::json::value::number(low);

    return json;
}
web::json::value CResponseSupporter::GetStockCurrentMarketPriceByCode(LPt1102OutBlock pT1102OutBlock)
{
    web::json::value json = web::json::value::object();

    // data.stock.hname: 한글명
    CStringW hname = GetStringWData(pT1102OutBlock->hname, sizeof(pT1102OutBlock->hname), DATA_TYPE_STRING);
    json[L"stock"][L"hname"] = web::json::value::string(hname.GetString());
    // data.stock.price: 현재가
    long price = GetLongData(pT1102OutBlock->price, sizeof(pT1102OutBlock->price), DATA_TYPE_LONG);
    json[L"stock"][L"price"] = web::json::value::number(price);
    // data.stock.sign: 전일대비구분
    CStringW sign = GetStringWData(pT1102OutBlock->sign, sizeof(pT1102OutBlock->sign), DATA_TYPE_STRING);
    json[L"stock"][L"sign"] = web::json::value::string(sign.GetString());
    // data.stock.change: 전일대비
    long change = GetLongData(pT1102OutBlock->change, sizeof(pT1102OutBlock->change), DATA_TYPE_LONG);
    json[L"stock"][L"change"] = web::json::value::number(change);
    // data.stock.diff: 등락율
    float diff = GetFloatData(pT1102OutBlock->diff, sizeof(pT1102OutBlock->diff), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"diff"] = web::json::value::number(diff);
    // data.stock.volume: 누적거래량
    long volume = GetLongData(pT1102OutBlock->volume, sizeof(pT1102OutBlock->volume), DATA_TYPE_LONG);
    json[L"stock"][L"volume"] = web::json::value::number(volume);
    // data.stock.recprice: 기준가(평가가격)
    long recprice = GetLongData(pT1102OutBlock->recprice, sizeof(pT1102OutBlock->recprice), DATA_TYPE_LONG);
    json[L"stock"][L"recprice"] = web::json::value::number(recprice);
    // data.stock.avg: 가중평균
    long avg = GetLongData(pT1102OutBlock->avg, sizeof(pT1102OutBlock->avg), DATA_TYPE_LONG);
    json[L"stock"][L"avg"] = web::json::value::number(avg);
    // data.stock.uplmtprice: 상한가(최고호가가격)
    long uplmtprice = GetLongData(pT1102OutBlock->uplmtprice, sizeof(pT1102OutBlock->uplmtprice), DATA_TYPE_LONG);
    json[L"stock"][L"uplmtprice"] = web::json::value::number(uplmtprice);
    // data.stock.dnlmtprice: 하한가(최저호가가격)
    long dnlmtprice = GetLongData(pT1102OutBlock->dnlmtprice, sizeof(pT1102OutBlock->dnlmtprice), DATA_TYPE_LONG);
    json[L"stock"][L"dnlmtprice"] = web::json::value::number(dnlmtprice);
    // data.stock.jnilvolume: 전일거래량
    long jnilvolume = GetLongData(pT1102OutBlock->jnilvolume, sizeof(pT1102OutBlock->jnilvolume), DATA_TYPE_LONG);
    json[L"stock"][L"jnilvolume"] = web::json::value::number(jnilvolume);
    // data.stock.volumediff: 거래량차
    long volumediff = GetLongData(pT1102OutBlock->volumediff, sizeof(pT1102OutBlock->volumediff), DATA_TYPE_LONG);
    json[L"stock"][L"volumediff"] = web::json::value::number(volumediff);
    // data.stock.open: 시가
    long open = GetLongData(pT1102OutBlock->open, sizeof(pT1102OutBlock->open), DATA_TYPE_LONG);
    json[L"stock"][L"open"] = web::json::value::number(open);
    // data.stock.opentime: 시가시간
    CStringW opentime = GetStringWData(pT1102OutBlock->opentime, sizeof(pT1102OutBlock->opentime), DATA_TYPE_STRING);
    json[L"stock"][L"opentime"] = web::json::value::string(opentime.GetString());
    // data.stock.high: 고가
    long high = GetLongData(pT1102OutBlock->high, sizeof(pT1102OutBlock->high), DATA_TYPE_LONG);
    json[L"stock"][L"high"] = web::json::value::number(high);
    // data.stock.hightime: 고가시간
    CStringW hightime = GetStringWData(pT1102OutBlock->hightime, sizeof(pT1102OutBlock->hightime), DATA_TYPE_STRING);
    json[L"stock"][L"hightime"] = web::json::value::string(hightime.GetString());
    // data.stock.low: 저가
    long low = GetLongData(pT1102OutBlock->low, sizeof(pT1102OutBlock->low), DATA_TYPE_LONG);
    json[L"stock"][L"low"] = web::json::value::number(low);
    // data.stock.lowtime: 저가시간
    CStringW lowtime = GetStringWData(pT1102OutBlock->lowtime, sizeof(pT1102OutBlock->lowtime), DATA_TYPE_STRING);
    json[L"stock"][L"lowtime"] = web::json::value::string(lowtime.GetString());
    // data.stock.high52w: 52최고가
    long high52w = GetLongData(pT1102OutBlock->high52w, sizeof(pT1102OutBlock->high52w), DATA_TYPE_LONG);
    json[L"stock"][L"high52w"] = web::json::value::number(high52w);
    // data.stock.high52wdate: 52최고가일
    CStringW high52wdate = GetStringWData(pT1102OutBlock->high52wdate, sizeof(pT1102OutBlock->high52wdate), DATA_TYPE_STRING);
    json[L"stock"][L"high52wdate"] = web::json::value::string(high52wdate.GetString());
    // data.stock.low52w: 52최저가
    long low52w = GetLongData(pT1102OutBlock->low52w, sizeof(pT1102OutBlock->low52w), DATA_TYPE_LONG);
    json[L"stock"][L"low52w"] = web::json::value::number(low52w);
    // data.stock.low52wdate: 52최저가일
    CStringW low52wdate = GetStringWData(pT1102OutBlock->low52wdate, sizeof(pT1102OutBlock->low52wdate), DATA_TYPE_STRING);
    json[L"stock"][L"low52wdate"] = web::json::value::string(low52wdate.GetString());
    // data.stock.diff: 소진율
    float exhratio = GetFloatData(pT1102OutBlock->exhratio, sizeof(pT1102OutBlock->exhratio), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"exhratio"] = web::json::value::number(exhratio);
    // data.stock.per: PER
    float per = GetFloatData(pT1102OutBlock->per, sizeof(pT1102OutBlock->per), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"per"] = web::json::value::number(per);
    // data.stock.pbrx: PBRX
    float pbrx = GetFloatData(pT1102OutBlock->pbrx, sizeof(pT1102OutBlock->pbrx), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"pbrx"] = web::json::value::number(pbrx);
    // data.stock.listing: 상장주식수(천)
    long listing = GetLongData(pT1102OutBlock->listing, sizeof(pT1102OutBlock->listing), DATA_TYPE_LONG);
    json[L"stock"][L"listing"] = web::json::value::number(listing);
    // data.stock.jkrate: 증거금율
    long jkrate = GetLongData(pT1102OutBlock->jkrate, sizeof(pT1102OutBlock->jkrate), DATA_TYPE_LONG);
    json[L"stock"][L"jkrate"] = web::json::value::number(jkrate);
    // data.stock.memedan: 수량단위
    CStringW memedan = GetStringWData(pT1102OutBlock->memedan, sizeof(pT1102OutBlock->memedan), DATA_TYPE_STRING);
    json[L"stock"][L"memedan"] = web::json::value::string(memedan.GetString());
    // data.stock.offernocd1: 매도증권사코드1
    CStringW offernocd1 = GetStringWData(pT1102OutBlock->offernocd1, sizeof(pT1102OutBlock->offernocd1), DATA_TYPE_STRING);
    json[L"stock"][L"offernocd1"] = web::json::value::string(offernocd1.GetString());
    // data.stock.bidnocd1: 매수증권사코드1
    CStringW bidnocd1 = GetStringWData(pT1102OutBlock->bidnocd1, sizeof(pT1102OutBlock->bidnocd1), DATA_TYPE_STRING);
    json[L"stock"][L"bidnocd1"] = web::json::value::string(bidnocd1.GetString());
    // data.stock.offerno1: 매도증권사명1
    CStringW offerno1 = GetStringWData(pT1102OutBlock->offerno1, sizeof(pT1102OutBlock->offerno1), DATA_TYPE_STRING);
    json[L"stock"][L"offerno1"] = web::json::value::string(offerno1.GetString());
    // data.stock.bidno1: 매수증권사명1
    CStringW bidno1 = GetStringWData(pT1102OutBlock->bidno1, sizeof(pT1102OutBlock->bidno1), DATA_TYPE_STRING);
    json[L"stock"][L"bidno1"] = web::json::value::string(bidno1.GetString());
    // data.stock.dvol1: 총매도수량1
    long dvol1 = GetLongData(pT1102OutBlock->dvol1, sizeof(pT1102OutBlock->dvol1), DATA_TYPE_LONG);
    json[L"stock"][L"dvol1"] = web::json::value::number(dvol1);
    // data.stock.svol1: 총매수수량1
    long svol1 = GetLongData(pT1102OutBlock->svol1, sizeof(pT1102OutBlock->svol1), DATA_TYPE_LONG);
    json[L"stock"][L"svol1"] = web::json::value::number(svol1);
    // data.stock.dcha1: 매도증감1
    long dcha1 = GetLongData(pT1102OutBlock->dcha1, sizeof(pT1102OutBlock->dcha1), DATA_TYPE_LONG);
    json[L"stock"][L"dcha1"] = web::json::value::number(dcha1);
    // data.stock.scha1: 매수증감1
    long scha1 = GetLongData(pT1102OutBlock->scha1, sizeof(pT1102OutBlock->scha1), DATA_TYPE_LONG);
    json[L"stock"][L"scha1"] = web::json::value::number(scha1);
    // data.stock.ddiff1: 매도비율1
    float ddiff1 = GetFloatData(pT1102OutBlock->ddiff1, sizeof(pT1102OutBlock->ddiff1), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"ddiff1"] = web::json::value::number(ddiff1);
    // data.stock.sdiff1: 매수비율1
    float sdiff1 = GetFloatData(pT1102OutBlock->sdiff1, sizeof(pT1102OutBlock->sdiff1), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"sdiff1"] = web::json::value::number(sdiff1);
    // data.stock.offernocd2: 매도증권사코드2
    CStringW offernocd2 = GetStringWData(pT1102OutBlock->offernocd2, sizeof(pT1102OutBlock->offernocd2), DATA_TYPE_STRING);
    json[L"stock"][L"offernocd2"] = web::json::value::string(offernocd2.GetString());
    // data.stock.bidnocd2: 매수증권사코드2
    CStringW bidnocd2 = GetStringWData(pT1102OutBlock->bidnocd2, sizeof(pT1102OutBlock->bidnocd2), DATA_TYPE_STRING);
    json[L"stock"][L"bidnocd2"] = web::json::value::string(bidnocd2.GetString());
    // data.stock.offerno2: 매도증권사명2
    CStringW offerno2 = GetStringWData(pT1102OutBlock->offerno2, sizeof(pT1102OutBlock->offerno2), DATA_TYPE_STRING);
    json[L"stock"][L"offerno2"] = web::json::value::string(offerno2.GetString());
    // data.stock.bidno2: 매수증권사명2
    CStringW bidno2 = GetStringWData(pT1102OutBlock->bidno2, sizeof(pT1102OutBlock->bidno2), DATA_TYPE_STRING);
    json[L"stock"][L"bidno2"] = web::json::value::string(bidno2.GetString());
    // data.stock.dvol2: 총매도수량2
    long dvol2 = GetLongData(pT1102OutBlock->dvol2, sizeof(pT1102OutBlock->dvol2), DATA_TYPE_LONG);
    json[L"stock"][L"dvol2"] = web::json::value::number(dvol2);
    // data.stock.svol2: 총매수수량2
    long svol2 = GetLongData(pT1102OutBlock->svol2, sizeof(pT1102OutBlock->svol2), DATA_TYPE_LONG);
    json[L"stock"][L"svol2"] = web::json::value::number(svol2);
    // data.stock.dcha2: 매도증감2
    long dcha2 = GetLongData(pT1102OutBlock->dcha2, sizeof(pT1102OutBlock->dcha2), DATA_TYPE_LONG);
    json[L"stock"][L"dcha2"] = web::json::value::number(dcha2);
    // data.stock.scha2: 매수증감2
    long scha2 = GetLongData(pT1102OutBlock->scha2, sizeof(pT1102OutBlock->scha2), DATA_TYPE_LONG);
    json[L"stock"][L"scha2"] = web::json::value::number(scha2);
    // data.stock.ddiff2: 매도비율2
    float ddiff2 = GetFloatData(pT1102OutBlock->ddiff2, sizeof(pT1102OutBlock->ddiff2), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"ddiff2"] = web::json::value::number(ddiff2);
    // data.stock.sdiff2: 매수비율2
    float sdiff2 = GetFloatData(pT1102OutBlock->sdiff2, sizeof(pT1102OutBlock->sdiff2), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"sdiff2"] = web::json::value::number(sdiff2);
    // data.stock.offernocd3: 매도증권사코드3
    CStringW offernocd3 = GetStringWData(pT1102OutBlock->offernocd3, sizeof(pT1102OutBlock->offernocd3), DATA_TYPE_STRING);
    json[L"stock"][L"offernocd3"] = web::json::value::string(offernocd3.GetString());
    // data.stock.bidnocd3: 매수증권사코드3
    CStringW bidnocd3 = GetStringWData(pT1102OutBlock->bidnocd3, sizeof(pT1102OutBlock->bidnocd3), DATA_TYPE_STRING);
    json[L"stock"][L"bidnocd3"] = web::json::value::string(bidnocd3.GetString());
    // data.stock.offerno3: 매도증권사명3
    CStringW offerno3 = GetStringWData(pT1102OutBlock->offerno3, sizeof(pT1102OutBlock->offerno3), DATA_TYPE_STRING);
    json[L"stock"][L"offerno3"] = web::json::value::string(offerno3.GetString());
    // data.stock.bidno3: 매수증권사명3
    CStringW bidno3 = GetStringWData(pT1102OutBlock->bidno3, sizeof(pT1102OutBlock->bidno3), DATA_TYPE_STRING);
    json[L"stock"][L"bidno3"] = web::json::value::string(bidno3.GetString());
    // data.stock.dvol3: 총매도수량3
    long dvol3 = GetLongData(pT1102OutBlock->dvol3, sizeof(pT1102OutBlock->dvol3), DATA_TYPE_LONG);
    json[L"stock"][L"dvol3"] = web::json::value::number(dvol3);
    // data.stock.svol3: 총매수수량3
    long svol3 = GetLongData(pT1102OutBlock->svol3, sizeof(pT1102OutBlock->svol3), DATA_TYPE_LONG);
    json[L"stock"][L"svol3"] = web::json::value::number(svol3);
    // data.stock.dcha3: 매도증감3
    long dcha3 = GetLongData(pT1102OutBlock->dcha3, sizeof(pT1102OutBlock->dcha3), DATA_TYPE_LONG);
    json[L"stock"][L"dcha3"] = web::json::value::number(dcha3);
    // data.stock.scha3: 매수증감3
    long scha3 = GetLongData(pT1102OutBlock->scha3, sizeof(pT1102OutBlock->scha3), DATA_TYPE_LONG);
    json[L"stock"][L"scha3"] = web::json::value::number(scha3);
    // data.stock.ddiff3: 매도비율3
    float ddiff3 = GetFloatData(pT1102OutBlock->ddiff3, sizeof(pT1102OutBlock->ddiff3), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"ddiff3"] = web::json::value::number(ddiff3);
    // data.stock.sdiff3: 매수비율3
    float sdiff3 = GetFloatData(pT1102OutBlock->sdiff3, sizeof(pT1102OutBlock->sdiff3), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"sdiff3"] = web::json::value::number(sdiff3);
    // data.stock.offernocd4: 매도증권사코드4
    CStringW offernocd4 = GetStringWData(pT1102OutBlock->offernocd4, sizeof(pT1102OutBlock->offernocd4), DATA_TYPE_STRING);
    json[L"stock"][L"offernocd4"] = web::json::value::string(offernocd4.GetString());
    // data.stock.bidnocd4: 매수증권사코드4
    CStringW bidnocd4 = GetStringWData(pT1102OutBlock->bidnocd4, sizeof(pT1102OutBlock->bidnocd4), DATA_TYPE_STRING);
    json[L"stock"][L"bidnocd4"] = web::json::value::string(bidnocd4.GetString());
    // data.stock.offerno4: 매도증권사명4
    CStringW offerno4 = GetStringWData(pT1102OutBlock->offerno4, sizeof(pT1102OutBlock->offerno4), DATA_TYPE_STRING);
    json[L"stock"][L"offerno4"] = web::json::value::string(offerno4.GetString());
    // data.stock.bidno4: 매수증권사명4
    CStringW bidno4 = GetStringWData(pT1102OutBlock->bidno4, sizeof(pT1102OutBlock->bidno4), DATA_TYPE_STRING);
    json[L"stock"][L"bidno4"] = web::json::value::string(bidno4.GetString());
    // data.stock.dvol4: 총매도수량4
    long dvol4 = GetLongData(pT1102OutBlock->dvol4, sizeof(pT1102OutBlock->dvol4), DATA_TYPE_LONG);
    json[L"stock"][L"dvol4"] = web::json::value::number(dvol4);
    // data.stock.svol4: 총매수수량4
    long svol4 = GetLongData(pT1102OutBlock->svol4, sizeof(pT1102OutBlock->svol4), DATA_TYPE_LONG);
    json[L"stock"][L"svol4"] = web::json::value::number(svol4);
    // data.stock.dcha4: 매도증감4
    long dcha4 = GetLongData(pT1102OutBlock->dcha4, sizeof(pT1102OutBlock->dcha4), DATA_TYPE_LONG);
    json[L"stock"][L"dcha4"] = web::json::value::number(dcha4);
    // data.stock.scha4: 매수증감4
    long scha4 = GetLongData(pT1102OutBlock->scha4, sizeof(pT1102OutBlock->scha4), DATA_TYPE_LONG);
    json[L"stock"][L"scha4"] = web::json::value::number(scha4);
    // data.stock.ddiff4: 매도비율4
    float ddiff4 = GetFloatData(pT1102OutBlock->ddiff4, sizeof(pT1102OutBlock->ddiff4), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"ddiff4"] = web::json::value::number(ddiff4);
    // data.stock.sdiff4: 매수비율4
    float sdiff4 = GetFloatData(pT1102OutBlock->sdiff4, sizeof(pT1102OutBlock->sdiff4), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"sdiff4"] = web::json::value::number(sdiff4);
    // data.stock.offernocd5: 매도증권사코드5
    CStringW offernocd5 = GetStringWData(pT1102OutBlock->offernocd5, sizeof(pT1102OutBlock->offernocd5), DATA_TYPE_STRING);
    json[L"stock"][L"offernocd5"] = web::json::value::string(offernocd5.GetString());
    // data.stock.bidnocd5: 매수증권사코드5
    CStringW bidnocd5 = GetStringWData(pT1102OutBlock->bidnocd5, sizeof(pT1102OutBlock->bidnocd5), DATA_TYPE_STRING);
    json[L"stock"][L"bidnocd5"] = web::json::value::string(bidnocd5.GetString());
    // data.stock.offerno5: 매도증권사명5
    CStringW offerno5 = GetStringWData(pT1102OutBlock->offerno5, sizeof(pT1102OutBlock->offerno5), DATA_TYPE_STRING);
    json[L"stock"][L"offerno5"] = web::json::value::string(offerno5.GetString());
    // data.stock.bidno5: 매수증권사명5
    CStringW bidno5 = GetStringWData(pT1102OutBlock->bidno5, sizeof(pT1102OutBlock->bidno5), DATA_TYPE_STRING);
    json[L"stock"][L"bidno5"] = web::json::value::string(bidno5.GetString());
    // data.stock.dvol5: 총매도수량5
    long dvol5 = GetLongData(pT1102OutBlock->dvol5, sizeof(pT1102OutBlock->dvol5), DATA_TYPE_LONG);
    json[L"stock"][L"dvol5"] = web::json::value::number(dvol5);
    // data.stock.svol5: 총매수수량5
    long svol5 = GetLongData(pT1102OutBlock->svol5, sizeof(pT1102OutBlock->svol5), DATA_TYPE_LONG);
    json[L"stock"][L"svol5"] = web::json::value::number(svol5);
    // data.stock.dcha5: 매도증감5
    long dcha5 = GetLongData(pT1102OutBlock->dcha5, sizeof(pT1102OutBlock->dcha5), DATA_TYPE_LONG);
    json[L"stock"][L"dcha5"] = web::json::value::number(dcha5);
    // data.stock.scha5: 매수증감5
    long scha5 = GetLongData(pT1102OutBlock->scha5, sizeof(pT1102OutBlock->scha5), DATA_TYPE_LONG);
    json[L"stock"][L"scha5"] = web::json::value::number(scha5);
    // data.stock.ddiff5: 매도비율5
    float ddiff5 = GetFloatData(pT1102OutBlock->ddiff5, sizeof(pT1102OutBlock->ddiff5), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"ddiff5"] = web::json::value::number(ddiff5);
    // data.stock.sdiff5: 매수비율5
    float sdiff5 = GetFloatData(pT1102OutBlock->sdiff5, sizeof(pT1102OutBlock->sdiff5), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"sdiff5"] = web::json::value::number(sdiff5);
    // data.stock.fwdvl: 외국계매도합계수량
    long fwdvl = GetLongData(pT1102OutBlock->fwdvl, sizeof(pT1102OutBlock->fwdvl), DATA_TYPE_LONG);
    json[L"stock"][L"fwdvl"] = web::json::value::number(fwdvl);
    // data.stock.ftradmdcha: 외국계매도직전대비
    long ftradmdcha = GetLongData(pT1102OutBlock->ftradmdcha, sizeof(pT1102OutBlock->ftradmdcha), DATA_TYPE_LONG);
    json[L"stock"][L"ftradmdcha"] = web::json::value::number(ftradmdcha);
    // data.stock.ftradmddiff: 외국계매도비율
    float ftradmddiff = GetFloatData(pT1102OutBlock->ftradmddiff, sizeof(pT1102OutBlock->ftradmddiff), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"ftradmddiff"] = web::json::value::number(ftradmddiff);
    // data.stock.fwsvl: 외국계매수합계수량
    long fwsvl = GetLongData(pT1102OutBlock->fwsvl, sizeof(pT1102OutBlock->fwsvl), DATA_TYPE_LONG);
    json[L"stock"][L"fwsvl"] = web::json::value::number(fwsvl);
    // data.stock.ftradmscha: 외국계매수직전대비
    long ftradmscha = GetLongData(pT1102OutBlock->ftradmscha, sizeof(pT1102OutBlock->ftradmscha), DATA_TYPE_LONG);
    json[L"stock"][L"ftradmscha"] = web::json::value::number(ftradmscha);
    // data.stock.ftradmddiff: 외국계매수비율
    float ftradmsdiff = GetFloatData(pT1102OutBlock->ftradmsdiff, sizeof(pT1102OutBlock->ftradmsdiff), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"ftradmsdiff"] = web::json::value::number(ftradmsdiff);
    // data.stock.vol: 회전율
    float vol = GetFloatData(pT1102OutBlock->vol, sizeof(pT1102OutBlock->vol), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"vol"] = web::json::value::number(vol);
    // data.stock.shcode: 단축코드
    CStringW shcode = GetStringWData(pT1102OutBlock->shcode, sizeof(pT1102OutBlock->shcode), DATA_TYPE_STRING);
    json[L"stock"][L"shcode"] = web::json::value::string(shcode.GetString());
    // data.stock.value: 누적거래대금
    long value = GetLongData(pT1102OutBlock->value, sizeof(pT1102OutBlock->value), DATA_TYPE_LONG);
    json[L"stock"][L"value"] = web::json::value::number(value);
    // data.stock.jvolume: 전일동시간거래량
    long jvolume = GetLongData(pT1102OutBlock->jvolume, sizeof(pT1102OutBlock->jvolume), DATA_TYPE_LONG);
    json[L"stock"][L"jvolume"] = web::json::value::number(jvolume);
    // data.stock.highyear: 연중최고가
    long highyear = GetLongData(pT1102OutBlock->highyear, sizeof(pT1102OutBlock->highyear), DATA_TYPE_LONG);
    json[L"stock"][L"highyear"] = web::json::value::number(highyear);
    // data.stock.highyeardate: 연중최고일자
    CStringW highyeardate = GetStringWData(pT1102OutBlock->highyeardate, sizeof(pT1102OutBlock->highyeardate), DATA_TYPE_STRING);
    json[L"stock"][L"highyeardate"] = web::json::value::string(highyeardate.GetString());
    // data.stock.lowyear: 연중최저가
    long lowyear = GetLongData(pT1102OutBlock->lowyear, sizeof(pT1102OutBlock->lowyear), DATA_TYPE_LONG);
    json[L"stock"][L"lowyear"] = web::json::value::number(lowyear);
    // data.stock.lowyeardate: 연중최저일자
    CStringW lowyeardate = GetStringWData(pT1102OutBlock->lowyeardate, sizeof(pT1102OutBlock->lowyeardate), DATA_TYPE_STRING);
    json[L"stock"][L"lowyeardate"] = web::json::value::string(lowyeardate.GetString());
    // data.stock.target: 목표가
    long target = GetLongData(pT1102OutBlock->target, sizeof(pT1102OutBlock->target), DATA_TYPE_LONG);
    json[L"stock"][L"target"] = web::json::value::number(target);
    // data.stock.capital: 자본금
    long capital = GetLongData(pT1102OutBlock->capital, sizeof(pT1102OutBlock->capital), DATA_TYPE_LONG);
    json[L"stock"][L"capital"] = web::json::value::number(capital);
    // data.stock.abscnt: 유동주식수
    long abscnt = GetLongData(pT1102OutBlock->abscnt, sizeof(pT1102OutBlock->abscnt), DATA_TYPE_LONG);
    json[L"stock"][L"abscnt"] = web::json::value::number(abscnt);
    // data.stock.parprice: 액면가
    long parprice = GetLongData(pT1102OutBlock->parprice, sizeof(pT1102OutBlock->parprice), DATA_TYPE_LONG);
    json[L"stock"][L"parprice"] = web::json::value::number(parprice);
    // data.stock.gsmm: 결산월
    CStringW gsmm = GetStringWData(pT1102OutBlock->gsmm, sizeof(pT1102OutBlock->gsmm), DATA_TYPE_STRING);
    json[L"stock"][L"gsmm"] = web::json::value::string(gsmm.GetString());
    // data.stock.subprice: 대용가
    long subprice = GetLongData(pT1102OutBlock->subprice, sizeof(pT1102OutBlock->subprice), DATA_TYPE_LONG);
    json[L"stock"][L"subprice"] = web::json::value::number(subprice);
    // data.stock.total: 시가총액
    long total = GetLongData(pT1102OutBlock->total, sizeof(pT1102OutBlock->total), DATA_TYPE_LONG);
    json[L"stock"][L"total"] = web::json::value::number(total);
    // data.stock.listdate: 상장일
    CStringW listdate = GetStringWData(pT1102OutBlock->listdate, sizeof(pT1102OutBlock->listdate), DATA_TYPE_STRING);
    json[L"stock"][L"listdate"] = web::json::value::string(listdate.GetString());
    // data.stock.name: 전분기명
    CStringW name = GetStringWData(pT1102OutBlock->name, sizeof(pT1102OutBlock->name), DATA_TYPE_STRING);
    json[L"stock"][L"name"] = web::json::value::string(name.GetString());
    // data.stock.bfsales: 전분기매출액
    long bfsales = GetLongData(pT1102OutBlock->bfsales, sizeof(pT1102OutBlock->bfsales), DATA_TYPE_LONG);
    json[L"stock"][L"bfsales"] = web::json::value::number(bfsales);
    // data.stock.bfoperatingincome: 전분기영업이익
    long bfoperatingincome = GetLongData(pT1102OutBlock->bfoperatingincome, sizeof(pT1102OutBlock->bfoperatingincome), DATA_TYPE_LONG);
    json[L"stock"][L"bfoperatingincome"] = web::json::value::number(bfoperatingincome);
    // data.stock.bfordinaryincome: 전분기경상이익
    long bfordinaryincome = GetLongData(pT1102OutBlock->bfordinaryincome, sizeof(pT1102OutBlock->bfordinaryincome), DATA_TYPE_LONG);
    json[L"stock"][L"bfordinaryincome"] = web::json::value::number(bfordinaryincome);
    // data.stock.bfnetincome: 전분기순이익
    long bfnetincome = GetLongData(pT1102OutBlock->bfnetincome, sizeof(pT1102OutBlock->bfnetincome), DATA_TYPE_LONG);
    json[L"stock"][L"bfnetincome"] = web::json::value::number(bfnetincome);
    // data.stock.bfeps: 전분기EPS
    float bfeps = GetFloatData(pT1102OutBlock->bfeps, sizeof(pT1102OutBlock->bfeps), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"bfeps"] = web::json::value::number(bfeps);
    // data.stock.name2: 전전분기명
    CStringW name2 = GetStringWData(pT1102OutBlock->name2, sizeof(pT1102OutBlock->name2), DATA_TYPE_STRING);
    json[L"stock"][L"name2"] = web::json::value::string(name2.GetString());
    // data.stock.bfsales2: 전전분기매출액
    long bfsales2 = GetLongData(pT1102OutBlock->bfsales2, sizeof(pT1102OutBlock->bfsales2), DATA_TYPE_LONG);
    json[L"stock"][L"bfsales2"] = web::json::value::number(bfsales2);
    // data.stock.bfoperatingincome2: 전전분기영업이익
    long bfoperatingincome2 = GetLongData(pT1102OutBlock->bfoperatingincome2, sizeof(pT1102OutBlock->bfoperatingincome2), DATA_TYPE_LONG);
    json[L"stock"][L"bfoperatingincome2"] = web::json::value::number(bfoperatingincome2);
    // data.stock.bfordinaryincome2: 전전분기경상이익
    long bfordinaryincome2 = GetLongData(pT1102OutBlock->bfordinaryincome2, sizeof(pT1102OutBlock->bfordinaryincome2), DATA_TYPE_LONG);
    json[L"stock"][L"bfordinaryincome2"] = web::json::value::number(bfordinaryincome2);
    // data.stock.bfnetincome2: 전전분기순이익
    long bfnetincome2 = GetLongData(pT1102OutBlock->bfnetincome2, sizeof(pT1102OutBlock->bfnetincome2), DATA_TYPE_LONG);
    json[L"stock"][L"bfnetincome2"] = web::json::value::number(bfnetincome2);
    // data.stock.bfeps2: 전전분기EPS
    float bfeps2 = GetFloatData(pT1102OutBlock->bfeps2, sizeof(pT1102OutBlock->bfeps2), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"bfeps2"] = web::json::value::number(bfeps2);
    // data.stock.salert: 전년대비매출액
    float salert = GetFloatData(pT1102OutBlock->salert, sizeof(pT1102OutBlock->salert), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"salert"] = web::json::value::number(salert);
    // data.stock.opert: 전년대비영업이익
    float opert = GetFloatData(pT1102OutBlock->opert, sizeof(pT1102OutBlock->opert), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"opert"] = web::json::value::number(opert);
    // data.stock.ordrt: 전년대비경상이익
    float ordrt = GetFloatData(pT1102OutBlock->ordrt, sizeof(pT1102OutBlock->ordrt), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"ordrt"] = web::json::value::number(ordrt);
    // data.stock.netrt: 전년대비순이익
    float netrt = GetFloatData(pT1102OutBlock->netrt, sizeof(pT1102OutBlock->netrt), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"netrt"] = web::json::value::number(netrt);
    // data.stock.epsrt: 전년대비EPS
    float epsrt = GetFloatData(pT1102OutBlock->epsrt, sizeof(pT1102OutBlock->epsrt), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"epsrt"] = web::json::value::number(epsrt);
    // data.stock.info1: 락구분
    CStringW info1 = GetStringWData(pT1102OutBlock->info1, sizeof(pT1102OutBlock->info1), DATA_TYPE_STRING);
    json[L"stock"][L"info1"] = web::json::value::string(info1.GetString());
    // data.stock.info2: 관리/급등구분
    CStringW info2 = GetStringWData(pT1102OutBlock->info2, sizeof(pT1102OutBlock->info2), DATA_TYPE_STRING);
    json[L"stock"][L"info2"] = web::json::value::string(info2.GetString());
    // data.stock.info3: 정지/연장구분
    CStringW info3 = GetStringWData(pT1102OutBlock->info3, sizeof(pT1102OutBlock->info3), DATA_TYPE_STRING);
    json[L"stock"][L"info3"] = web::json::value::string(info3.GetString());
    // data.stock.info4: 투자/불성실구분
    CStringW info4 = GetStringWData(pT1102OutBlock->info4, sizeof(pT1102OutBlock->info4), DATA_TYPE_STRING);
    json[L"stock"][L"info4"] = web::json::value::string(info4.GetString());
    // data.stock.janginfo: 장구분
    CStringW janginfo = GetStringWData(pT1102OutBlock->janginfo, sizeof(pT1102OutBlock->janginfo), DATA_TYPE_STRING);
    json[L"stock"][L"janginfo"] = web::json::value::string(janginfo.GetString());
    // data.stock.tper: T.PER
    float tper = GetFloatData(pT1102OutBlock->t_per, sizeof(pT1102OutBlock->t_per), DATA_TYPE_FLOAT, 2);
    json[L"stock"][L"tper"] = web::json::value::number(tper);
    // data.stock.tonghwa: 통화ISO코드
    CStringW tonghwa = GetStringWData(pT1102OutBlock->tonghwa, sizeof(pT1102OutBlock->tonghwa), DATA_TYPE_STRING);
    json[L"stock"][L"tonghwa"] = web::json::value::string(tonghwa.GetString());
    // data.stock.dval1: 총매도대금1
    long dval1 = GetLongData(pT1102OutBlock->dval1, sizeof(pT1102OutBlock->dval1), DATA_TYPE_LONG);
    json[L"stock"][L"dval1"] = web::json::value::number(dval1);
    // data.stock.sval1: 총매수대금1
    long sval1 = GetLongData(pT1102OutBlock->sval1, sizeof(pT1102OutBlock->sval1), DATA_TYPE_LONG);
    json[L"stock"][L"sval1"] = web::json::value::number(sval1);
    // data.stock.dval2: 총매도대금2
    long dval2 = GetLongData(pT1102OutBlock->dval2, sizeof(pT1102OutBlock->dval2), DATA_TYPE_LONG);
    json[L"stock"][L"dval2"] = web::json::value::number(dval2);
    // data.stock.sval2: 총매수대금2
    long sval2 = GetLongData(pT1102OutBlock->sval2, sizeof(pT1102OutBlock->sval2), DATA_TYPE_LONG);
    json[L"stock"][L"sval2"] = web::json::value::number(sval2);
    // data.stock.dval3: 총매도대금3
    long dval3 = GetLongData(pT1102OutBlock->dval3, sizeof(pT1102OutBlock->dval3), DATA_TYPE_LONG);
    json[L"stock"][L"dval3"] = web::json::value::number(dval3);
    // data.stock.sval3: 총매수대금3
    long sval3 = GetLongData(pT1102OutBlock->sval3, sizeof(pT1102OutBlock->sval3), DATA_TYPE_LONG);
    json[L"stock"][L"sval3"] = web::json::value::number(sval3);
    // data.stock.dval4: 총매도대금4
    long dval4 = GetLongData(pT1102OutBlock->dval4, sizeof(pT1102OutBlock->dval4), DATA_TYPE_LONG);
    json[L"stock"][L"dval4"] = web::json::value::number(dval4);
    // data.stock.sval4: 총매수대금4
    long sval4 = GetLongData(pT1102OutBlock->sval4, sizeof(pT1102OutBlock->sval4), DATA_TYPE_LONG);
    json[L"stock"][L"sval4"] = web::json::value::number(sval4);
    // data.stock.dval5: 총매도대금5
    long dval5 = GetLongData(pT1102OutBlock->dval5, sizeof(pT1102OutBlock->dval5), DATA_TYPE_LONG);
    json[L"stock"][L"dval5"] = web::json::value::number(dval5);
    // data.stock.sval5: 총매수대금5
    long sval5 = GetLongData(pT1102OutBlock->sval5, sizeof(pT1102OutBlock->sval5), DATA_TYPE_LONG);
    json[L"stock"][L"sval5"] = web::json::value::number(sval5);
    // data.stock.davg1: 총매도평단가1
    long davg1 = GetLongData(pT1102OutBlock->davg1, sizeof(pT1102OutBlock->davg1), DATA_TYPE_LONG);
    json[L"stock"][L"davg1"] = web::json::value::number(davg1);
    // data.stock.savg1: 총매수평단가1
    long savg1 = GetLongData(pT1102OutBlock->savg1, sizeof(pT1102OutBlock->savg1), DATA_TYPE_LONG);
    json[L"stock"][L"savg1"] = web::json::value::number(savg1);
    // data.stock.davg2: 총매도평단가2
    long davg2 = GetLongData(pT1102OutBlock->davg2, sizeof(pT1102OutBlock->davg2), DATA_TYPE_LONG);
    json[L"stock"][L"davg2"] = web::json::value::number(davg2);
    // data.stock.savg2: 총매수평단가2
    long savg2 = GetLongData(pT1102OutBlock->savg2, sizeof(pT1102OutBlock->savg2), DATA_TYPE_LONG);
    json[L"stock"][L"savg2"] = web::json::value::number(savg2);
    // data.stock.davg3: 총매도평단가3
    long davg3 = GetLongData(pT1102OutBlock->davg3, sizeof(pT1102OutBlock->davg3), DATA_TYPE_LONG);
    json[L"stock"][L"davg3"] = web::json::value::number(davg3);
    // data.stock.savg3: 총매수평단가3
    long savg3 = GetLongData(pT1102OutBlock->savg3, sizeof(pT1102OutBlock->savg3), DATA_TYPE_LONG);
    json[L"stock"][L"savg3"] = web::json::value::number(savg3);
    // data.stock.davg4: 총매도평단가4
    long davg4 = GetLongData(pT1102OutBlock->davg4, sizeof(pT1102OutBlock->davg4), DATA_TYPE_LONG);
    json[L"stock"][L"davg4"] = web::json::value::number(davg4);
    // data.stock.savg4: 총매수평단가4
    long savg4 = GetLongData(pT1102OutBlock->savg4, sizeof(pT1102OutBlock->savg4), DATA_TYPE_LONG);
    json[L"stock"][L"savg4"] = web::json::value::number(savg4);
    // data.stock.davg5: 총매도평단가5
    long davg5 = GetLongData(pT1102OutBlock->davg5, sizeof(pT1102OutBlock->davg5), DATA_TYPE_LONG);
    json[L"stock"][L"davg5"] = web::json::value::number(davg5);
    // data.stock.savg5: 총매수평단가5
    long savg5 = GetLongData(pT1102OutBlock->savg5, sizeof(pT1102OutBlock->savg5), DATA_TYPE_LONG);
    json[L"stock"][L"savg5"] = web::json::value::number(savg5);
    // data.stock.ftradmdval: 외국계매도대금
    long ftradmdval = GetLongData(pT1102OutBlock->ftradmdval, sizeof(pT1102OutBlock->ftradmdval), DATA_TYPE_LONG);
    json[L"stock"][L"ftradmdval"] = web::json::value::number(ftradmdval);
    // data.stock.ftradmsval: 외국계매수대금
    long ftradmsval = GetLongData(pT1102OutBlock->ftradmsval, sizeof(pT1102OutBlock->ftradmsval), DATA_TYPE_LONG);
    json[L"stock"][L"ftradmsval"] = web::json::value::number(ftradmsval);
    // data.stock.ftradmdavg: 외국계매도평단가
    long ftradmdavg = GetLongData(pT1102OutBlock->ftradmdavg, sizeof(pT1102OutBlock->ftradmdavg), DATA_TYPE_LONG);
    json[L"stock"][L"ftradmdavg"] = web::json::value::number(ftradmdavg);
    // data.stock.ftradmsavg: 외국계매수평단가
    long ftradmsavg = GetLongData(pT1102OutBlock->ftradmsavg, sizeof(pT1102OutBlock->ftradmsavg), DATA_TYPE_LONG);
    json[L"stock"][L"ftradmsavg"] = web::json::value::number(ftradmsavg);
    // data.stock.info5: 투자주의환기
    CStringW info5 = GetStringWData(pT1102OutBlock->info5, sizeof(pT1102OutBlock->info5), DATA_TYPE_STRING);
    json[L"stock"][L"info5"] = web::json::value::string(info5.GetString());
    // data.stock.spacgubun: 기업인수목적회사여부
    CStringW spacgubun = GetStringWData(pT1102OutBlock->spac_gubun, sizeof(pT1102OutBlock->spac_gubun), DATA_TYPE_STRING);
    json[L"stock"][L"spacgubun"] = web::json::value::string(spacgubun.GetString());
    // data.stock.issueprice: 발행가격
    long issueprice = GetLongData(pT1102OutBlock->issueprice, sizeof(pT1102OutBlock->issueprice), DATA_TYPE_LONG);
    json[L"stock"][L"issueprice"] = web::json::value::number(issueprice);
    // data.stock.: 배분적용구분코드(1:배분발생2:배
    CStringW allocgubun = GetStringWData(pT1102OutBlock->alloc_gubun, sizeof(pT1102OutBlock->alloc_gubun), DATA_TYPE_STRING);
    json[L"stock"][L"allocgubun"] = web::json::value::string(allocgubun.GetString());
    // data.stock.alloctext: 배분적용구분
    CStringW alloctext = GetStringWData(pT1102OutBlock->alloc_text, sizeof(pT1102OutBlock->alloc_text), DATA_TYPE_STRING);
    json[L"stock"][L"alloctext"] = web::json::value::string(alloctext.GetString());
    // data.stock.shtermtext: 단기과열/VI발동
    CStringW shtermtext = GetStringWData(pT1102OutBlock->shterm_text, sizeof(pT1102OutBlock->shterm_text), DATA_TYPE_STRING);
    json[L"stock"][L"shtermtext"] = web::json::value::string(shtermtext.GetString());
    // data.stock.sviuplmtprice: 정적VI상한가
    long sviuplmtprice = GetLongData(pT1102OutBlock->svi_uplmtprice, sizeof(pT1102OutBlock->svi_uplmtprice), DATA_TYPE_LONG);
    json[L"stock"][L"sviuplmtprice"] = web::json::value::number(sviuplmtprice);
    // data.stock.svidnlmtprice: 정적VI하한가
    long svidnlmtprice = GetLongData(pT1102OutBlock->svi_dnlmtprice, sizeof(pT1102OutBlock->svi_dnlmtprice), DATA_TYPE_LONG);
    json[L"stock"][L"svidnlmtprice"] = web::json::value::number(svidnlmtprice);
    // data.stock.lowlqdtgu: 저유동성종목여부
    CStringW lowlqdtgu = GetStringWData(pT1102OutBlock->low_lqdt_gu, sizeof(pT1102OutBlock->low_lqdt_gu), DATA_TYPE_STRING);
    json[L"stock"][L"lowlqdtgu"] = web::json::value::string(lowlqdtgu.GetString());
    // data.stock.abnormalrisegu: 이상급등종목여부
    CStringW abnormalrisegu = GetStringWData(pT1102OutBlock->abnormal_rise_gu, sizeof(pT1102OutBlock->abnormal_rise_gu), DATA_TYPE_STRING);
    json[L"stock"][L"abnormalrisegu"] = web::json::value::string(abnormalrisegu.GetString());
    // data.stock.lendtext: 대차불가표시
    CStringW lendtext = GetStringWData(pT1102OutBlock->lend_text, sizeof(pT1102OutBlock->lend_text), DATA_TYPE_STRING);
    json[L"stock"][L"lendtext"] = web::json::value::string(lendtext.GetString());
    // data.stock.tytext: ETF/ETN투자유의
    CStringW tytext = GetStringWData(pT1102OutBlock->ty_text, sizeof(pT1102OutBlock->ty_text), DATA_TYPE_STRING);
    json[L"stock"][L"tytext"] = web::json::value::string(tytext.GetString());

    return json;
}
