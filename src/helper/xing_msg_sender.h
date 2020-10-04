// wmca_msg_sender.h: interface for the CWmcaMsgSender class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include "xing_api.h"

class CXingMsgSender
{
public:
    HWND m_hWnd;
    std::wstring m_strData;

    CXingMsgSender();
    ~CXingMsgSender() = default;

    BOOL Connect(HWND hWnd);
    BOOL Login(HWND hWnd);
    void Disconnect();
    BOOL IsConnected();
    void InquireCurrentPrice(HWND hWnd);
private:
    IXingAPI m_xingAPI;
};

