#include <cpprest/json.h>

#include "core/framework.h"
#include "util/string_util.h"
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
        int nErrorCode = m_xingAPI.GetLastError();
        CStringW strMsg(m_xingAPI.GetErrorMessage(nErrorCode));
        MessageBox(hWnd, strMsg, L"서버접속실패", MB_ICONSTOP);
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
        int nErrorCode = m_xingAPI.GetLastError();
        CStringW strMsg(m_xingAPI.GetErrorMessage(nErrorCode));
        MessageBox(hWnd, strMsg, L"로그인 실패", MB_ICONSTOP);
        return FALSE;
    }
    return TRUE;
}

void CXingMsgSender::Disconnect()
{
    //m_wmca.Disconnect();
}

BOOL CXingMsgSender::IsConnected()
{
    //return m_wmca.IsConnected();
    return false;
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
