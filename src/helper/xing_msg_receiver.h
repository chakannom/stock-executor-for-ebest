#pragma once

#include <cpprest/json.h>
#include "xing_api.h"

class CXingMsgReceiver
{
public:
    BOOL LoginEvent(int nCode, LPCSTR pszMsg);
    void IsLoginEvent(bool isLogin);
    void ReceiveRequestDataEvent(LPRECV_PACKET pRpData);
    void ReceiveMessageDataEvent(LPMSG_PACKET pMsg);
    void ReceiveSystemErrorDataEvent(LPMSG_PACKET pErrorMsg);
    void ReceiveReleaseDataEvent(int nRequestID);
    void ErrorEvent(int nErrorCode, LPCSTR pszErrorMsg);
private:
    web::json::value resJson = web::json::value::object();
    bool onlyDisconnect = true;

    void processMessage(std::function<void()> generateMessage);
    void clearResponseJson();
};

