#include "core/framework.h"
#include "packet/t8436.h"
#include "response_supporter.h"

web::json::value CResponseSupporter::GetConnectedStatus(BOOL isConnected)
{
    web::json::value json = web::json::value::object();

    json[L"status"] = web::json::value::boolean(isConnected != FALSE);

    return json;
}

web::json::value CResponseSupporter::GetStocksByGubun(LPt8436OutBlock pT8436OutBlock, int nBlockCount)
{
    web::json::value json = web::json::value::array();

    for (int i = 0; i < nBlockCount; i++) {
        // data.stocks[].hname: 종목명
        CStringW hname = GetStringWData(pT8436OutBlock[i].hname, sizeof(pT8436OutBlock[i].hname), DATA_TYPE_STRING);
        json[i][L"hname"] = web::json::value::string(hname.GetString());
        // data.stocks[].shcode: 단축코드
        CStringW shcode = GetStringWData(pT8436OutBlock[i].shcode, sizeof(pT8436OutBlock[i].shcode), DATA_TYPE_STRING);
        json[i][L"shcode"] = web::json::value::string(shcode.GetString());
        // data.stocks[].expcode: 확장코드
        CStringW expcode = GetStringWData(pT8436OutBlock[i].expcode, sizeof(pT8436OutBlock[i].expcode), DATA_TYPE_STRING);
        json[i][L"expcode"] = web::json::value::string(expcode.GetString());
        // data.stocks[].etfgubun: ETF구분(1:ETF2:ETN)
        CStringW etfgubun = GetStringWData(pT8436OutBlock[i].etfgubun, sizeof(pT8436OutBlock[i].etfgubun), DATA_TYPE_STRING);
        json[i][L"etfgubun"] = web::json::value::string(etfgubun.GetString());
        // data.stocks[].uplmtprice: 상한가
        long uplmtprice = GetLongData(pT8436OutBlock[i].uplmtprice, sizeof(pT8436OutBlock[i].uplmtprice), DATA_TYPE_LONG);
        json[i][L"uplmtprice"] = web::json::value::number(uplmtprice);
        // data.stocks[].dnlmtprice: 하한가
        long dnlmtprice = GetLongData(pT8436OutBlock[i].dnlmtprice, sizeof(pT8436OutBlock[i].dnlmtprice), DATA_TYPE_LONG);
        json[i][L"dnlmtprice"] = web::json::value::number(dnlmtprice);
        // data.stocks[].jnilclose: 전일가
        long jnilclose = GetLongData(pT8436OutBlock[i].jnilclose, sizeof(pT8436OutBlock[i].jnilclose), DATA_TYPE_LONG);
        json[i][L"jnilclose"] = web::json::value::number(jnilclose);
        // data.stocks[].memedan: 주문수량단위
        CStringW memedan = GetStringWData(pT8436OutBlock[i].memedan, sizeof(pT8436OutBlock[i].memedan), DATA_TYPE_STRING);
        json[i][L"memedan"] = web::json::value::string(memedan.GetString());
        // data.stocks[].recprice: 기준가
        long recprice = GetLongData(pT8436OutBlock[i].recprice, sizeof(pT8436OutBlock[i].recprice), DATA_TYPE_LONG);
        json[i][L"recprice"] = web::json::value::number(recprice);
        // data.stocks[].gubun: 구분(1:코스피2:코스닥)
        CStringW gubun = GetStringWData(pT8436OutBlock[i].gubun, sizeof(pT8436OutBlock[i].gubun), DATA_TYPE_STRING);
        json[i][L"gubun"] = web::json::value::string(gubun.GetString());
        // data.stocks[].bu12gubun: 증권그룹
        CStringW bu12gubun = GetStringWData(pT8436OutBlock[i].bu12gubun, sizeof(pT8436OutBlock[i].bu12gubun), DATA_TYPE_STRING);
        json[i][L"bu12gubun"] = web::json::value::string(bu12gubun.GetString());
        // data.stocks[].spacGubun: 기업인수목적회사여부(Y/N)
        CStringW spacgubun = GetStringWData(pT8436OutBlock[i].spac_gubun, sizeof(pT8436OutBlock[i].spac_gubun), DATA_TYPE_STRING);
        json[i][L"spacgubun"] = web::json::value::string(spacgubun.GetString());
        // data.stocks[].filler: filler(미사용)
        CStringW filler = GetStringWData(pT8436OutBlock[i].filler, sizeof(pT8436OutBlock[i].filler), DATA_TYPE_STRING);
        //json[i][L"filler"] = web::json::value::string(filler.GetString());
    }

    return json;
}
/*
web::json::value CResponseSupporter::GetConnectedData(LOGINBLOCK* pLogin)
{
    web::json::value json = web::json::value::object();

    // data.connectedDate: 접속시간
    time_t connectedDate = GetEpochMilli(SCOPY_A(pLogin->pLoginInfo->szDate));
    json[L"connectedDate"] = web::json::value::number(connectedDate);

    // data.username: 접속자ID
    CStringW username(SCOPY_A(pLogin->pLoginInfo->szUserID).TrimRight());
    json[L"username"] = web::json::value::string(username.GetString());

    // data.accounts: 계좌목록
    json[L"accounts"] = web::json::value::array();
    int	nAccountCount = atoi(SCOPY_A(pLogin->pLoginInfo->szAccountCount));
    for (int i = 0; i < nAccountCount; i++) {
        json[L"accounts"][i] = web::json::value::object();
        ACCOUNTINFO* pAccountInfo = &(pLogin->pLoginInfo->accountlist[i]);
        // data.accounts.name: 계좌명
        CStringW accountName(SCOPY_A(pAccountInfo->szAccountName).TrimRight());
        json[L"accounts"][i][L"name"] = web::json::value::string(accountName.GetString());
        // data.accounts.name: 계좌번호
        CStringW accountNo(SCOPY_A(pAccountInfo->szAccountNo).TrimRight());
        json[L"accounts"][i][L"number"] = web::json::value::string(accountNo.GetString());
    }

    return json;
}

web::json::value CResponseSupporter::GetCurrentPriceInformationData(Tc1101OutBlock* pc1101Outblock)
{
    web::json::value json = web::json::value::object();

    // data.information.code: 종목코드
    CStringW code(SCOPY_A(pc1101Outblock->code));
    json[L"code"] = web::json::value::string(code.GetString());
    // data.information.name: 종목명
    CStringW name(SCOPY_A(pc1101Outblock->hname).TrimRight());
    json[L"name"] = web::json::value::string(name.GetString());
    // data.information.price: 현재가
    uint32_t price = strtoul(SCOPY_A(pc1101Outblock->price), nullptr, 10);
    json[L"price"] = web::json::value::number(price);
    // data.information.sign: 등락부호
    CStringW sign(SCOPY_A(pc1101Outblock->sign));
    json[L"sign"] = web::json::value::string(sign.GetString());
    // data.information.change: 등락폭
    uint32_t change = strtoul(SCOPY_A(pc1101Outblock->change), nullptr, 10);
    json[L"change"] = web::json::value::number(change);
    // data.information.chrate: 등락률
    float chrate = strtof(SCOPY_A(pc1101Outblock->chrate), nullptr);
    json[L"chrate"] = web::json::value::number(chrate);
    // data.information.offer: 매도호가
    uint32_t offer = strtoul(SCOPY_A(pc1101Outblock->offer), nullptr, 10);
    json[L"offer"] = web::json::value::number(offer);
    // data.information.bid: 매수호가
    uint32_t bid = strtoul(SCOPY_A(pc1101Outblock->bid), nullptr, 10);
    json[L"bid"] = web::json::value::number(bid);
    // data.information.volume: 거래량
    uint32_t volume = strtoul(SCOPY_A(pc1101Outblock->volume), nullptr, 10);
    json[L"volume"] = web::json::value::number(volume);
    // data.information.volrate: 거래비율
    float volrate = strtof(SCOPY_A(pc1101Outblock->volrate), nullptr);
    json[L"volrate"] = web::json::value::number(volrate);
    // data.information.yurate: 유동주회전율
    float yurate = strtof(SCOPY_A(pc1101Outblock->yurate), nullptr);
    json[L"yurate"] = web::json::value::number(yurate);
    // data.information.value: 거래대금
    uint32_t value = strtoul(SCOPY_A(pc1101Outblock->value), nullptr, 10);
    json[L"value"] = web::json::value::number(value);
    //////......
    // data.information.sosokz6: 코스피코스닥구분
    CStringW sosokz6(SCOPY_A(pc1101Outblock->sosokz6));
    json[L"sosokz6"] = web::json::value::string(sosokz6.GetString());
    // data.information.jisunamez18: 업종명
    CStringW jisunamez18(SCOPY_A(pc1101Outblock->jisunamez18).TrimRight());
    json[L"jisunamez18"] = web::json::value::string(jisunamez18.GetString());

    return json;
}

web::json::value CResponseSupporter::GetCurrentPriceVariableVolumeData(Tc1101OutBlock2* pc1101Outblock2)
{
    web::json::value json = web::json::value::object();

    // data.variableVolume.[].time: 시간
    CStringW time(SCOPY_A(pc1101Outblock2->time));
    json[L"time"] = web::json::value::string(time.GetString());
    // data.variableVolume.[].price: 현재가
    uint32_t price = strtoul(SCOPY_A(pc1101Outblock2->price), nullptr, 10);
    json[L"price"] = web::json::value::number(price);
    // data.variableVolume.[].sign: 등락부호
    CStringW sign(SCOPY_A(pc1101Outblock2->sign));
    json[L"sign"] = web::json::value::string(sign.GetString());
    // data.variableVolume.[].change: 등락폭
    uint32_t change = strtoul(SCOPY_A(pc1101Outblock2->change), nullptr, 10);
    json[L"change"] = web::json::value::number(change);
    // data.variableVolume.[].offer: 매도호가
    uint32_t offer = strtoul(SCOPY_A(pc1101Outblock2->offer), nullptr, 10);
    json[L"offer"] = web::json::value::number(offer);
    // data.variableVolume.[].bid: 매수호가
    uint32_t bid = strtoul(SCOPY_A(pc1101Outblock2->bid), nullptr, 10);
    json[L"bid"] = web::json::value::number(bid);
    // data.variableVolume.[].movolume: 변동거래량
    uint32_t movolume = strtoul(SCOPY_A(pc1101Outblock2->movolume), nullptr, 10);
    json[L"movolume"] = web::json::value::number(movolume);
    // data.variableVolume.[].volume: 거래량
    uint32_t volume = strtoul(SCOPY_A(pc1101Outblock2->volume), nullptr, 10);
    json[L"volume"] = web::json::value::number(volume);

    return json;
}

web::json::value CResponseSupporter::GetCurrentPriceSimultaneousQuoteData(Tc1101OutBlock3* pc1101Outblock3)
{
    web::json::value json = web::json::value::object();

    // data.simultaneousQuote.dongsi: 동시호가구분
    CStringW dongsi(SCOPY_A(pc1101Outblock3->dongsi));
    json[L"dongsi"] = web::json::value::string(dongsi.GetString());
    // data.simultaneousQuote.jeqprice: 예상체결가
    uint32_t jeqprice = strtoul(SCOPY_A(pc1101Outblock3->jeqprice), nullptr, 10);
    json[L"jeqprice"] = web::json::value::number(jeqprice);
    // data.simultaneousQuote.jeqsign: 예상체결부호
    CStringW jeqsign(SCOPY_A(pc1101Outblock3->jeqsign));
    json[L"jeqsign"] = web::json::value::string(jeqsign.GetString());
    // data.simultaneousQuote.jeqchange: 예상체결등락폭
    uint32_t jeqchange = strtoul(SCOPY_A(pc1101Outblock3->jeqchange), nullptr, 10);
    json[L"jeqchange"] = web::json::value::number(jeqchange);
    // data.simultaneousQuote.jeqchrate: 예상체결등락률
    float jeqchrate = strtof(SCOPY_A(pc1101Outblock3->jeqchrate), nullptr);
    json[L"jeqchrate"] = web::json::value::number(jeqchrate);
    // data.simultaneousQuote.jeqvol: 예상체결수량
    uint32_t jeqvol = strtoul(SCOPY_A(pc1101Outblock3->jeqvol), nullptr, 10);
    json[L"jeqvol"] = web::json::value::number(jeqvol);
    // data.simultaneousQuote.chkdataz1: ECN정보유무구분
    CStringW chkdataz1(SCOPY_A(pc1101Outblock3->chkdataz1));
    json[L"chkdataz1"] = web::json::value::string(chkdataz1.GetString());
    // data.simultaneousQuote.ecnPrice: ECN전일종가
    uint32_t ecnPrice = strtoul(SCOPY_A(pc1101Outblock3->ecn_price), nullptr, 10);
    json[L"ecnPrice"] = web::json::value::number(ecnPrice);
    // data.simultaneousQuote.ecnSign: ECN부호
    CStringW ecnSign(SCOPY_A(pc1101Outblock3->ecn_sign));
    json[L"ecnSign"] = web::json::value::string(ecnSign.GetString());
    // data.simultaneousQuote.ecnChange: ECN등락폭
    uint32_t ecnChange = strtoul(SCOPY_A(pc1101Outblock3->ecn_change), nullptr, 10);
    json[L"ecnChange"] = web::json::value::number(ecnChange);
    // data.simultaneousQuote.ecnChrate: ECN등락률
    float ecnChrate = strtof(SCOPY_A(pc1101Outblock3->ecn_chrate), nullptr);
    json[L"ecnChrate"] = web::json::value::number(ecnChrate);
    // data.simultaneousQuote.ecnVolume: ECN체결수량
    double ecnVolume = strtod(SCOPY_A(pc1101Outblock3->ecn_volume), nullptr);
    json[L"ecnVolume"] = web::json::value::number(ecnVolume);
    // data.simultaneousQuote.ecnJeqsign: ECN대비예상체결부호
    CStringW ecnJeqsign(SCOPY_A(pc1101Outblock3->ecn_jeqsign));
    json[L"ecnJeqsign"] = web::json::value::string(ecnJeqsign.GetString());
    // data.simultaneousQuote.ecnJeqchange: ECN대비예상체결등락폭
    uint32_t ecnJeqchange = strtoul(SCOPY_A(pc1101Outblock3->ecn_jeqchange), nullptr, 10);
    json[L"ecnJeqchange"] = web::json::value::number(ecnJeqchange);
    // data.simultaneousQuote.ecnJeqchrate: ECN대비예상체결등락률
    float ecnJeqchrate = strtof(SCOPY_A(pc1101Outblock3->ecn_jeqchrate), nullptr);
    json[L"ecnJeqchrate"] = web::json::value::number(ecnJeqchrate);

    return json;
}
*/