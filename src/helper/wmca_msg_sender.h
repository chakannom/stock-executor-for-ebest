// wmca_msg_sender.h: interface for the CWmcaMsgSender class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "wmca_intf.h"

class CWmcaMsgSender
{
public:
    HWND m_hWnd;
    std::wstring m_strData;

    void Connect(HWND hWnd);
    void Disconnect();
    BOOL IsConnected();
    void InquireCurrentPrice(HWND hWnd);
private:
    CWmcaIntf m_wmca;
};

