#pragma once

#include <cpprest/json.h>

class CXingMsgReceiver
{
public:
    void LoginEvent(LPCSTR pszCode, LPCSTR pszMsg);

private:
    web::json::value resJson = web::json::value::object();
    bool onlyDisconnect = true;

    void processMessage(std::function<void()> generateMessage);
    void clearResponseJson();
};

