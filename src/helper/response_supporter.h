#pragma once

#include <cpprest/json.h>

class CResponseSupporter
{
public:
    static web::json::value GetConnectedData(LOGINBLOCK* pLogin);
    static web::json::value GetCurrentPriceInformationData(Tc1101OutBlock* pc1101Outblock);
    static web::json::value GetCurrentPriceVariableVolumeData(Tc1101OutBlock2* pc1101Outblock2);
    static web::json::value GetCurrentPriceSimultaneousQuoteData(Tc1101OutBlock3* pc1101Outblock3);
    static web::json::value GetConnectedStatusData(BOOL isConnected);
};

