#pragma once

#include <cpprest/json.h>

class CXingMsgReceiver
{
public:
    void LoginEvent(int nCode, LPCSTR pszMsg);
    void IsConnectedEvent(bool isConnected);
    void ErrorEvent(int nErrorCode, LPCSTR pszErrorMsg);
private:
    web::json::value resJson = web::json::value::object();
    bool onlyDisconnect = true;

    void processMessage(std::function<void()> generateMessage);
    void clearResponseJson();
};

