#pragma once

#include <cpprest/json.h>

class CWmcaMsgReceiver
{
public:
    void Connected(LOGINBLOCK* pLogin);
    void Disconnected();
    void SocketError(int socketErrorCode);
    void ReceiveData(OUTDATABLOCK* pOutData);
    void ReceiveSise(OUTDATABLOCK* pSiseData);
    void ReceiveMessage(OUTDATABLOCK* pMessage);
    void ReceiveComplete(OUTDATABLOCK* pOutData);
    void ReceiveError(OUTDATABLOCK* pError);
    void ConnectedStatus(BOOL isConnected);
private:
    web::json::value resJson = web::json::value::object();
    bool onlyDisconnect = true;

    void processMessage(std::function<void()> generateMessage);
    void clearResponseJson();
};

