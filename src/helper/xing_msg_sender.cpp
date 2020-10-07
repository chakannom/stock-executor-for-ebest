#include <cpprest/json.h>

#include "core/framework.h"
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

    // Request ID가 0보다 작을 경우에는 에러이다.
    if (bResult < 0) {
        ErrorMessage(hWnd);
        //MessageBox("조회실패", "에러", MB_ICONSTOP);
    }
}

void CXingMsgSender::InquireCurrentPrice(HWND hWnd)
{
    web::json::value reqJson = web::json::value::parse(m_strData);
    //종목코드를 가져옵니다
    CStringA strCode(reqJson.at(L"code").as_string().c_str());

    //주식현재가조회 서비스에서 요구하는 입력값을 저장할 구조체 변수입니다.
    //Tc1101InBlock c1101Inblock;
    //memset(&c1101Inblock, 0x20, sizeof Tc1101InBlock);

    //각 입력 필드에서 요구하는 값들에 대한 정의는 *.doc 문서를 통해 확인할 수 있습니다.
    //SMOVE_A(c1101Inblock.formlang, "k");
    //SMOVE_A(c1101Inblock.code, strCode);

    //주식 현재가 조회
    //m_wmca.Query(
    //    hWnd,                   //이 윈도우로 응답 메시지를 받겠습니다.
    //    TRID_c1101,             //이 서비스에 대해서 TRID_c1101(5) 식별자를 붙이겠다는 의미이며 반드시 상수일 필요는 없습니다.
    //    "c1101",                //호출하려는 서비스 코드는 'c1101' 입니다.
    //    (char*)&c1101Inblock,   //c1101에서 요구하는 입력 구조체 포인터를 지정합니다
    //    sizeof Tc1101InBlock    //입력 구조체 크기입니다
    //                            //현재가를 포함한 투자정보 조회는 계좌번호와 무관하므로 계좌번호 인덱스를 지정하지 않습니다.
    //);
}

void CXingMsgSender::ErrorMessage(HWND hWnd)
{
    int nErrorCode = m_xingAPI.GetLastError();
    CStringA strMsg = m_xingAPI.GetErrorMessage(nErrorCode);
    SendMessageW(hWnd, WM_USER + XM_CM_ERROR, (WPARAM)nErrorCode, (LPARAM)strMsg.GetString());
}