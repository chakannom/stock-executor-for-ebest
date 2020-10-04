#include "core/framework.h"
#include "util/string_util.h"
#include "response_supporter.h"
#include "xing_msg_receiver.h"

void CXingMsgReceiver::LoginEvent(LPCSTR pszCode, LPCSTR pszMsg)
{
    CStringW strMsg;
    strMsg.Format(L"[%s] %s", CStringW(pszCode), CStringW(pszMsg));

    if (atoi(pszCode) == 0) {
        // 로그인 성공
        MessageBox(NULL, strMsg, L"로그인 성공", MB_ICONINFORMATION);
    }
    else {
        // 로그인 실패
        MessageBox(NULL, strMsg, L"로그인 실패", MB_ICONSTOP);
    }
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