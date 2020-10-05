#include "core/framework.h"
#include "util/string_util.h"
#include "response_supporter.h"
#include "xing_msg_receiver.h"

void CXingMsgReceiver::LoginEvent(int nCode, LPCSTR pszMsg)
{
    if (nCode == 0) {
        // 로그인 성공
        CStringW strMsg;
        strMsg.Format(L"[%d] %s", nCode, CStringW(pszMsg));
        MessageBox(NULL, strMsg, L"Login", MB_ICONINFORMATION);
    }
    else {
        // 로그인 실패
        ErrorEvent(nCode, pszMsg);
    }
}

void CXingMsgReceiver::IsConnectedEvent(bool isConnected)
{
    CStringW strMsg = isConnected ? L"TRUE" : L"FALSE";
    MessageBox(NULL, strMsg, L"IsConnected", MB_ICONINFORMATION);
}

void CXingMsgReceiver::ErrorEvent(int nErrorCode, LPCSTR pszErrorMsg)
{
    CStringW strMsg;
    strMsg.Format(L"[%d] %s", nErrorCode, CStringW(pszErrorMsg));

    MessageBox(NULL, strMsg, L"Error", MB_ICONSTOP);
}

void CXingMsgReceiver::processMessage(std::function<void()> generateMessage)
{
#ifndef _DEBUG
    HWND hWnd = FindWindow(0, L"STOCK-AGENT_STOCK-SERVICE-HELPER");
    if (hWnd == NULL) return;
#endif

    generateMessage();

    std::wstring jsonString = resJson.serialize();

#ifndef _DEBUG
    COPYDATASTRUCT cds;
    cds.dwData = WM_USER + 2001;
    cds.cbData = jsonString.size() * sizeof(wchar_t);
    cds.lpData = (PVOID)jsonString.c_str();
    SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cds);
#endif

    clearResponseJson();
}

void CXingMsgReceiver::clearResponseJson()
{
    if (resJson.has_field(L"code")) {
        resJson.erase(L"code");
    }
    if (resJson.has_field(L"message")) {
        resJson.erase(L"message");
    }
    if (resJson.has_field(L"data")) {
        resJson.erase(L"data");
    }
}