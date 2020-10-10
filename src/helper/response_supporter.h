#pragma once

#include <cpprest/json.h>

class CResponseSupporter
{
public:
    static web::json::value GetLoginInformation();
    static web::json::value GetConnectedStatus(BOOL isConnected);
    static web::json::value GetStocksByGubun(LPt8436OutBlock pT8436OutBlock, int nBlockCount);
    static web::json::value GetStockCurrentAskingPriceByCode(LPt1101OutBlock pT1101OutBlock);
    static web::json::value GetStockCurrentMarketPriceByCode(LPt1102OutBlock pT1102OutBlock);
};

