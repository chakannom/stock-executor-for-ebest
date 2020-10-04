#define _UNICODE
#include "core/framework.h"
#include "util/string_util.h"
#include "trio_def.h"
#include "trio_inv.h"
#include "wmca_intf.h"
#include "response_supporter.h"
#include "wmca_msg_receiver.h"

void CWmcaMsgReceiver::Connected(LOGINBLOCK* pLogin)
{
    auto generateMessage = [this, pLogin]() {
        resJson[L"code"] = web::json::value::string(L"00000");
        resJson[L"message"] = web::json::value::string(L"Connected.");
        resJson[L"data"] = CResponseSupporter::GetConnectedData(pLogin);
    };

    processMessage(generateMessage);
}

void CWmcaMsgReceiver::Disconnected()
{
    auto generateMessage = [this]() {
        if (onlyDisconnect) {
            resJson[L"code"] = web::json::value::string(L"00000");
            resJson[L"message"] = web::json::value::string(L"Disconnected.");
        }
    };

    processMessage(generateMessage);
    onlyDisconnect = true;
}

void CWmcaMsgReceiver::SocketError(int socketErrorCode)
{

}

void CWmcaMsgReceiver::ReceiveData(OUTDATABLOCK* pOutData)
{
    switch (pOutData->TrIndex) {
    case TRID_c1101:
        //주식 현재가/종목정보	
        if (strcmp(pOutData->pData->szBlockName, "c1101OutBlock") == 0) {
            Tc1101OutBlock* pc1101Outblock = (Tc1101OutBlock*)pOutData->pData->szData;
            resJson[L"data"][L"information"] = CResponseSupporter::GetCurrentPriceInformationData(pc1101Outblock);
        }

        //주식 현재가/변동거래량
        if (strcmp(pOutData->pData->szBlockName, "c1101OutBlock2") == 0) {
            Tc1101OutBlock2* pc1101Outblock2 = (Tc1101OutBlock2*)pOutData->pData->szData;
            resJson[L"data"][L"variableVolume"] = web::json::value::array();
            //실제 데이터에 따라 수신 데이터 행의 수가 가변적이므로
            //수신자료 크기를 구조체 크기로 나누어 몇 번 반복되는지 계산함
            int nOccursCount = pOutData->pData->nLen / sizeof(Tc1101OutBlock2);    //반복 회수를 구함

            for (int i = 0; i < nOccursCount; i++) { //회수만큼 반복하면서 데이터 구함
                resJson[L"data"][L"variableVolume"][i] = CResponseSupporter::GetCurrentPriceVariableVolumeData(pc1101Outblock2);

                pc1101Outblock2++;    //다음행으로 포인터 이동
            }
        }

        //주식 현재가/동시호가	
        if (strcmp(pOutData->pData->szBlockName, "c1101OutBlock3") == 0) {
            Tc1101OutBlock3* pc1101Outblock3 = (Tc1101OutBlock3*)pOutData->pData->szData;
            resJson[L"data"][L"simultaneousQuote"] = CResponseSupporter::GetCurrentPriceSimultaneousQuoteData(pc1101Outblock3);
        }
        break;
    }
}

void CWmcaMsgReceiver::ReceiveSise(OUTDATABLOCK* pSiseData)
{

}

void CWmcaMsgReceiver::ReceiveMessage(OUTDATABLOCK* pMessage)
{
    // 현재 상태를 문자열 형태로 출력함
    MSGHEADER* pMsgHeader = (MSGHEADER*)pMessage->pData->szData;
    CStringA code = SCOPY_A(pMsgHeader->msg_cd);
    if (code.Compare("00001") == 0) {
        resJson[L"code"] = web::json::value::string(L"00001");
        resJson[L"message"] = web::json::value::string(L"The id and password are incorrect.");
        onlyDisconnect = false;
    }
    else if (code.Compare("90002") == 0) {
        resJson[L"code"] = web::json::value::string(L"90002");
        resJson[L"message"] = web::json::value::string(L"The certificate password is incorrect.");
        onlyDisconnect = false;
    }
}

void CWmcaMsgReceiver::ReceiveComplete(OUTDATABLOCK* pOutData) 
{
    // TR 호출 완료시점에 처리하는 부분
    auto generateMessage = [this, pOutData]() {
        resJson[L"code"] = web::json::value::string(L"00000");

        switch (pOutData->TrIndex)
        {
        case TRID_c1101:
            // 주식 현재가 조회 완료
            resJson[L"message"] = web::json::value::string(L"An inquiry of current price completed.");
            break;
        case TRID_c8201:
            // 계좌 잔고 조회 완료
            resJson[L"message"] = web::json::value::string(L"An inquiry of Account balance completed.");
            break;
        }
    };

    processMessage(generateMessage);
}

void CWmcaMsgReceiver::ReceiveError(OUTDATABLOCK* pError)
{

}

void CWmcaMsgReceiver::ConnectedStatus(BOOL isConnected)
{
    auto generateMessage = [this, isConnected]() {
        resJson[L"code"] = web::json::value::string(L"00000");
        resJson[L"message"] = web::json::value::string(L"It checked the status.");
        resJson[L"data"] = CResponseSupporter::GetConnectedStatusData(isConnected);
    };

    processMessage(generateMessage);
}

void CWmcaMsgReceiver::processMessage(std::function<void()> generateMessage)
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

void CWmcaMsgReceiver::clearResponseJson()
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