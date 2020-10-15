#include "core/framework.h"
#include "packet/t1101.h"
#include "packet/t1102.h"
#include "packet/t8436.h"
#include "response_supporter.h"
#include "xing_msg_receiver.h"

BOOL CXingMsgReceiver::LoginEvent(int nCode, LPCSTR pszMsg)
{
    if (nCode == 0) {
        // 로그인 성공
        auto generateMessage = [this, nCode, pszMsg]() {
            resJson[L"code"] = web::json::value::string(L"00000");
            resJson[L"message"] = web::json::value::string(L"Login was successful.");
            resJson[L"data"] = CResponseSupporter::GetLoginInformation();
        };

        processMessage(generateMessage);

        return TRUE;
    }
    else {
        // 로그인 실패
        ErrorEvent(nCode, pszMsg);

        return FALSE;
    }
}

BOOL CXingMsgReceiver::LogoutEvent()
{
    // 로그아웃 성공
    auto generateMessage = [this]() {
        resJson[L"code"] = web::json::value::string(L"00000");
        resJson[L"message"] = web::json::value::string(L"Logout.");
    };

    processMessage(generateMessage);

    return TRUE;
}

void CXingMsgReceiver::IsLoginEvent(bool isLogin)
{
    auto generateMessage = [this, isLogin]() {
        resJson[L"code"] = web::json::value::string(L"00000");
        resJson[L"message"] = web::json::value::string(L"It checked the status.");
        resJson[L"data"] = CResponseSupporter::GetLoginStatus(isLogin);
    };

    processMessage(generateMessage);
}

void CXingMsgReceiver::ReceiveRequestDataEvent(LPRECV_PACKET pRpData)
{
    if (strcmp(pRpData->szTrCode, NAME_t1101) == 0) {
        LPt1101OutBlock pT1101OutBlock = (LPt1101OutBlock)pRpData->lpData;
        resJson[L"data"] = CResponseSupporter::GetStockCurrentAskingPriceByCode(pT1101OutBlock);
    }
    if (strcmp(pRpData->szTrCode, NAME_t1102) == 0) {
        LPt1102OutBlock pT1102OutBlock = (LPt1102OutBlock)pRpData->lpData;
        resJson[L"data"] = CResponseSupporter::GetStockCurrentMarketPriceByCode(pT1102OutBlock);
    }
    else if (strcmp(pRpData->szTrCode, NAME_t8436) == 0) {
        int nOutBlockCount = pRpData->nDataLength / sizeof(t8436OutBlock);
        LPt8436OutBlock pT8436OutBlock = (LPt8436OutBlock)pRpData->lpData;
        resJson[L"data"] = CResponseSupporter::GetStocksByGubun(pT8436OutBlock, nOutBlockCount);
    }
}

void CXingMsgReceiver::ReceiveMessageDataEvent(LPMSG_PACKET pMsg)
{
    resJson[L"message"] = web::json::value::string(L"completed.");
}

void CXingMsgReceiver::ReceiveSystemErrorDataEvent(LPMSG_PACKET pErrorMsg)
{
}

void CXingMsgReceiver::ReceiveReleaseDataEvent(int nRequestID)
{
    // TimeoutDateEvent에서도 호출함**

    // 호출 완료시점에 처리하는 부분
    auto generateMessage = [this, nRequestID]() {
        resJson[L"code"] = web::json::value::string(L"00000");
    };

    processMessage(generateMessage);
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