#include <cpprest/json.h>

#include "core/framework.h"
#include "packet/t1101.h"
#include "packet/t1102.h"
#include "packet/t8436.h"
#include "xing_msg_sender.h"

CXingMsgSender::CXingMsgSender()
{
    if (m_xingAPI.Init() == FALSE) {
        AfxMessageBox(L"XINGAPI.DLL 파일이 없습니다.\n이 파일이 경로에 있는지 확인하시기 바랍니다.");
        return;
    }
}

BOOL CXingMsgSender::Connect(HWND hWnd)
{
    web::json::value reqJson = web::json::value::parse(m_strData);
    // for ServerAddress
    CStringA strServerAddress = "hts.ebest.co.kr";
    if (reqJson.has_string_field(L"serverType")) {
        CStringA strServerType(reqJson.at(L"serverType").as_string().c_str());
        if (strServerType.CompareNoCase("DEMO") == 0) {
            strServerAddress = "demo.ebest.co.kr";
        }
    }

    // 서버접속
    BOOL bResult = m_xingAPI.Connect(
        hWnd,                           // Connect가 된 이후에 Disconnect 메시지를 받을 윈도우 Handle
        strServerAddress,               // 서버주소 (실서버(HTS): hts.ebest.co.kr, 모의투자서버(DEMO): demo.ebest.co.kr)
        20001,                          // 서버포트
        WM_USER,                        // XingAPI에서 사용하는 메시지의 시작번호
        30000,                          // 서버연결시간 TimeOut(Milliseconds(1/1000초))
        512                             // 보내어지는 Packet Size, -1 이면 기본값 사용
                                        //  -> 인터넷 공유기등에서는 특정 크기 이상의 데이터를 한번에 보내면 에러가 떨어지는 경우가 발생
                                        //  -> 이럴 경우에 한번에 보내는 Packet Size를 지정하여 그 이상 되는 Packet은 여러번에 걸쳐 전송
    );
    if (bResult == FALSE) {
        ErrorMessage(hWnd);
        return FALSE;
    }
    return TRUE;
}

BOOL CXingMsgSender::Login(HWND hWnd)
{
    web::json::value reqJson = web::json::value::parse(m_strData);
    CStringA strId(reqJson.at(L"id").as_string().c_str());
    CStringA strPw(reqJson.at(L"pw").as_string().c_str());
    CStringA strCertPw(reqJson.at(L"certPw").as_string().c_str());
    // for ServerType
    int nServerType = 0;
    if (reqJson.has_string_field(L"serverType")) {
        CStringA strServerType(reqJson.at(L"serverType").as_string().c_str());
        if (strServerType.CompareNoCase("DEMO") == 0) {
            nServerType = 1;
        }
    }

    //로그인
    BOOL bResult = m_xingAPI.Login(
        hWnd,                           // Login 성공여부 메시지를 받을 윈도우
        strId,                          // 사용자 ID
        strPw,                          // 사용자 비밀번호
        strCertPw,                      // 공인인증 비밀번호
        nServerType,                    // 0: 실서버, 1: 모의투자서버
        FALSE                           // 로그인 중에 공인인증 에러가 발생시 에러메시지 표시여부
    );
    if (bResult == FALSE) {
        ErrorMessage(hWnd);
        return FALSE;
    }
    return TRUE;
}

void CXingMsgSender::Logout(HWND hWnd)
{
    m_xingAPI.Logout(hWnd);
}

void CXingMsgSender::Disconnect()
{
    m_xingAPI.Disconnect();
}

BOOL CXingMsgSender::IsConnected(HWND hWnd)
{
    BOOL isConnected = m_xingAPI.IsConnected();
    SendMessageW(hWnd, WM_USER + XM_CM_ISCONNECTED, isConnected, NULL);
    return isConnected;
}

void CXingMsgSender::StocksByGubun(HWND hWnd)
{
    web::json::value reqJson = web::json::value::parse(m_strData);
    CStringA gubun(reqJson.at(L"gubun").as_string().c_str());

    // 주식종목조회 API(t8436) ( BLOCK,HEADTYPE=A )
    t8436InBlock pckT8436InBlock;
    FillMemory(&pckT8436InBlock, sizeof(pckT8436InBlock), ' ');

    // 데이터 입력
    SetPacketData(pckT8436InBlock.gubun, sizeof(pckT8436InBlock.gubun), gubun, DATA_TYPE_STRING);

    // 데이터 전송
    BOOL bResult = m_xingAPI.Request(
        hWnd,                       // 데이터를 받을 윈도우, XM_RECEIVE_DATA 으로 온다.
        NAME_t8436,                 // TR 번호
        &pckT8436InBlock,           // InBlock 데이터
        sizeof(pckT8436InBlock),    // InBlock 데이터 크기
        0,                          // 다음조회 여부 : 다음조회일 경우에 세팅한다.
        "",                         // 다음조회 Key : Header Type이 B 일 경우엔 이전 조회때 받은 Next Key를 넣어준다.
        30                          // Timeout(초) : 해당 시간(초)동안 데이터가 오지 않으면 Timeout에 발생한다. XM_TIMEOUT_DATA 메시지가 발생한다.
    );
    if (bResult < 0) {      // Request ID가 0보다 작을 경우에는 에러이다.
        ErrorMessage(hWnd);
        //MessageBox("조회실패", "에러", MB_ICONSTOP);
    }
}

void CXingMsgSender::StockCurrentAskingPriceByCode(HWND hWnd)
{
    web::json::value reqJson = web::json::value::parse(m_strData);
    CStringA code(reqJson.at(L"code").as_string().c_str());

    // 주식 현재가 호가 조회(t1101) ( ATTR,BLOCK,HEADTYPE=A )
    t1101InBlock pckT1101InBlock;
    FillMemory(&pckT1101InBlock, sizeof(pckT1101InBlock), ' ');

    // 데이터 입력
    SetPacketData(pckT1101InBlock.shcode, sizeof(pckT1101InBlock.shcode), code, DATA_TYPE_STRING);

    // 데이터 전송
    BOOL bResult = m_xingAPI.Request(hWnd, NAME_t1101, &pckT1101InBlock, sizeof(pckT1101InBlock), 0, "", 30);
    if (bResult < 0) {
        ErrorMessage(hWnd);
        //MessageBox("조회실패", "에러", MB_ICONSTOP);
    }
}

void CXingMsgSender::StockCurrentMarketPriceByCode(HWND hWnd)
{
    web::json::value reqJson = web::json::value::parse(m_strData);
    CStringA code(reqJson.at(L"code").as_string().c_str());

    // 주식 현재가(시세) 조회 ( ATTR,BLOCK,HEADTYPE=A )
    t1102InBlock pckT1102InBlock;
    FillMemory(&pckT1102InBlock, sizeof(pckT1102InBlock), ' ');

    // 데이터 입력
    SetPacketData(pckT1102InBlock.shcode, sizeof(pckT1102InBlock.shcode), code, DATA_TYPE_STRING);

    // 데이터 전송
    BOOL bResult = m_xingAPI.Request(hWnd, NAME_t1102, &pckT1102InBlock, sizeof(pckT1102InBlock), 0, "", 30);
    if (bResult < 0) {
        ErrorMessage(hWnd);
        //MessageBox("조회실패", "에러", MB_ICONSTOP);
    }
}

void CXingMsgSender::ErrorMessage(HWND hWnd)
{
    int nErrorCode = m_xingAPI.GetLastError();
    CStringA strMsg = m_xingAPI.GetErrorMessage(nErrorCode);
    SendMessageW(hWnd, WM_USER + XM_CM_ERROR, (WPARAM)nErrorCode, (LPARAM)strMsg.GetString());
}