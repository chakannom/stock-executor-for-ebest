#define _UNICODE
#include <cpprest/json.h>

#include "core/framework.h"
#include "util/string_util.h"
#include "trio_def.h"
#include "trio_inv.h"
#include "wmca_msg_sender.h"

void CWmcaMsgSender::Connect(HWND hWnd)
{
    web::json::value reqJson = web::json::value::parse(m_strData);
    CStringA strId(reqJson.at(L"id").as_string().c_str());
    CStringA strPw(reqJson.at(L"pw").as_string().c_str());
    CStringA strCertPw(reqJson.at(L"certPw").as_string().c_str());

    //접속 및 로그인
    //매체코드는 특별한 경우를 제외하고 항상 아래 기본값을 사용하시기 바랍니다.
    m_wmca.Connect(hWnd, CA_WMCAEVENT, 'T', 'W', strId, strPw, strCertPw);	//Namuh OpenAPI 사용자용

    m_strData.clear();
}

void CWmcaMsgSender::Disconnect()
{
    m_wmca.Disconnect();
}

BOOL CWmcaMsgSender::IsConnected()
{
    return m_wmca.IsConnected();
}

void CWmcaMsgSender::InquireCurrentPrice(HWND hWnd)
{
    web::json::value reqJson = web::json::value::parse(m_strData);
    //종목코드를 가져옵니다
    CStringA strCode(reqJson.at(L"code").as_string().c_str());

    //주식현재가조회 서비스에서 요구하는 입력값을 저장할 구조체 변수입니다.
    Tc1101InBlock c1101Inblock;
    memset(&c1101Inblock, 0x20, sizeof Tc1101InBlock);

    //각 입력 필드에서 요구하는 값들에 대한 정의는 *.doc 문서를 통해 확인할 수 있습니다.
    SMOVE_A(c1101Inblock.formlang, "k");
    SMOVE_A(c1101Inblock.code, strCode);

    //주식 현재가 조회
    m_wmca.Query(
        hWnd,                   //이 윈도우로 응답 메시지를 받겠습니다.
        TRID_c1101,             //이 서비스에 대해서 TRID_c1101(5) 식별자를 붙이겠다는 의미이며 반드시 상수일 필요는 없습니다.
        "c1101",                //호출하려는 서비스 코드는 'c1101' 입니다.
        (char*)&c1101Inblock,   //c1101에서 요구하는 입력 구조체 포인터를 지정합니다
        sizeof Tc1101InBlock    //입력 구조체 크기입니다
                                //현재가를 포함한 투자정보 조회는 계좌번호와 무관하므로 계좌번호 인덱스를 지정하지 않습니다.
    );

    m_strData.clear();
}
