// wmca_intf.h: interface for the CWmcaIntf class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>

const DWORD CA_WMCAEVENT        = WM_USER + 8400;

const DWORD CA_CONNECTED        = WM_USER + 110; //로그인 성공
const DWORD CA_DISCONNECTED     = WM_USER + 120; //접속 끊김
const DWORD CA_SOCKETERROR      = WM_USER + 130; //통신 오류 발생
const DWORD CA_RECEIVEDATA      = WM_USER + 210; //서비스 응답 수신(TR)
const DWORD CA_RECEIVESISE      = WM_USER + 220; //실시간 데이터 수신(BC)
const DWORD CA_RECEIVEMESSAGE   = WM_USER + 230; //상태 메시지 수신 (입력값이 잘못되었을 경우 문자열형태로 설명이 수신됨)
const DWORD CA_RECEIVECOMPLETE  = WM_USER + 240; //서비스 처리 완료
const DWORD CA_RECEIVEERROR     = WM_USER + 250; //서비스 처리중 오류 발생 (입력값 오류등)

typedef BOOL (__stdcall TLoad                   )();
typedef BOOL (__stdcall TFree                   )();
typedef BOOL (__stdcall TSetServer              )(const char* szServer);
typedef BOOL (__stdcall TSetPort                )(const int nPort);
typedef BOOL (__stdcall TIsConnected            )();
typedef BOOL (__stdcall TConnect                )(HWND hWnd,DWORD dwMsg,char cMediaType,char cUserType,const char* pszID,const char* pszPassword,const char* pszSignPassword);
typedef BOOL (__stdcall TDisconnect             )();
typedef BOOL (__stdcall TTransact               )(HWND hWnd,int nTransactionID,const char* pszTrCode,const char* pszInputData,int nInputDataSize,int nHeaderType,int nAccountIndex);
typedef BOOL (__stdcall TQuery                  )(HWND hWnd,int nTransactionID,const char* pszTrCode,const char* pszInputData,int nInputDataSize,int nAccountIndex);
typedef BOOL (__stdcall TRequest                )(HWND hWnd,int nTransactionID,const char* pszTrCode,const char* pszInputData,int nInputDataSize,const char* pszOpenBranchCode);
typedef BOOL (__stdcall TAttach                 )(HWND hWnd,const char* pszSiseName,const char* pszInputCode,int nInputCodeSize,int nInputCodeTotalSize);
typedef BOOL (__stdcall TDetach                 )(HWND hWnd,const char* pszSiseName,const char* pszInputCode,int nInputCodeSize,int nInputCodeTotalSize);
typedef BOOL (__stdcall TDetachWindow           )(HWND hWnd);
typedef BOOL (__stdcall TDetachAll              )();
typedef BOOL (__stdcall TSetOption              )(const char* szKey,const char* szValue);
typedef BOOL (__stdcall TSetAccountIndexPwd     )(const char* pszHashOut,int nAccountIndex,const char* pszPassword); 
typedef BOOL (__stdcall TSetOrderPwd            )(const char* pszHashOut,const char* pszPassword);
typedef BOOL (__stdcall TSetHashPwd             )(const char* pszHashOut,const char* pszKey,const char* pszPassword);
typedef BOOL (__stdcall TSetAccountNoPwd        )(const char* pszHashOut,const char* pszAccountNo,const char* pszPassword);
typedef BOOL (__stdcall TSetAccountNoByIndex    )(const char* pszHashOut,int nAccountIndex);

//----------------------------------------------------------------------//
// WMCA_CONNECTED 로그인 구조체
//----------------------------------------------------------------------//
typedef struct {
    char szAccountNo[11];       //계좌번호
    char szAccountName[40];     //계좌명
    char act_pdt_cdz3[3];       //상품코드
    char amn_tab_cdz4[4];       //관리점코드
    char expr_datez8[8];        //위임만기일
    char granted;               //일괄주문 허용계좌(G:허용)
    char filler[189];           //filler
}ACCOUNTINFO;

typedef struct {
    char szDate[14];                // 접속시각
    char szServerName[15];          // 접속서버
    char szUserID [8];              // 접속자ID
    char szAccountCount[3];         // 계좌수
    ACCOUNTINFO accountlist[999];   // 계좌목록
}LOGININFO;

typedef struct{
    int TrIndex;
    LOGININFO *pLoginInfo;
}LOGINBLOCK;

//----------------------------------------------------------------------//
// WMCA 문자message 구조체
//----------------------------------------------------------------------//
typedef struct  {
    char msg_cd[5];     //00000:정상, 기타:비정상(해당 코드값을 이용하여 코딩하지 마세요. 코드값은 언제든지 변경될 수 있습니다.)
    char user_msg[80];
} MSGHEADER;

         
//----------------------------------------------------------------------//
// WMCA TR 응답 구조체
//----------------------------------------------------------------------//

typedef struct {
    char* szBlockName;
    char* szData;
    int nLen;
} RECEIVED;

typedef struct {
    int TrIndex;
    RECEIVED* pData;
} OUTDATABLOCK;

//----------------------------------------------------------------------//
// wmca.dll wrapping functions
//----------------------------------------------------------------------//
class CWmcaIntf
{
private:
    HINSTANCE       m_hDll;

    TLoad                   *m_pLoad;
    TFree                   *m_pFree;
    TSetServer              *m_pSetServer;
    TSetPort                *m_pSetPort;
    TIsConnected            *m_pIsConnected;
    TConnect                *m_pConnect;
    TDisconnect             *m_pDisconnect;
    TTransact               *m_pTransact;
    TQuery                  *m_pQuery;   
    TRequest                *m_pRequest;
    TAttach                 *m_pAttach;
    TDetach                 *m_pDetach;
    TDetachWindow           *m_pDetachWindow;
    TDetachAll              *m_pDetachAll;
    TSetOption              *m_pSetOption;
    TSetAccountIndexPwd     *m_pSetAccountIndexPwd;
    TSetOrderPwd            *m_pSetOrderPwd;
    TSetHashPwd             *m_pSetHashPwd;
    TSetAccountNoPwd        *m_pSetAccountNoPwd;
    TSetAccountNoByIndex    *m_pSetAccountNoByIndex;

public:
    CWmcaIntf();
    virtual ~CWmcaIntf();
public:
    BOOL Load               ();
    BOOL Free               ();
    BOOL Connect            (HWND hWnd, DWORD msg, char MediaType,char UserType,const char* szID,const char* szPW, const char* szCertPW);				//사용자 인증 및 접속
    BOOL Disconnect         ();
    BOOL SetServer          (const char* szServer);																										//접속서버 지정(필요시)
    BOOL SetPort            (int port);																													//접속포트 지정(필요시)
    BOOL IsConnected        ();
    BOOL Transact           (HWND hWnd, int nTRID, const char* szTRCode, const char* szInput, int nInputLen, int nHeadType=0, int nAccountIndex=0);		//실수를 방지하려면 Transact()대신
    BOOL Query              (HWND hWnd, int nTRID, const char* szTRCode, const char* szInput, int nInputLen, int nAccountIndex=0);						//Query()함수를 사용하세요.
    BOOL Request            (HWND hWnd, int nTRID, const char* szTRCode, const char* szInput, int nInputLen, const char* szOpenBranchCode=NULL);
    BOOL Attach             (HWND hWnd, const char* szBCType, const char* szInput, int nCodeLen, int nInputLen);	//실시간 시세 요청
    BOOL Detach             (HWND hWnd, const char* szBCType, const char* szInput, int nCodeLen, int nInputLen);	//실시간 시세 취소
    BOOL DetachWindow       (HWND hWnd);																			//실시간 시세 일괄취소(윈도우단위)
    BOOL DetachAll          ();																						//실시간 시세 일괄취소(전체)
    BOOL SetOption          (const char* szKey,const char* szValue);
    BOOL SetAccountIndexPwd (const char* pszHashOut,int nAccountIndex,const char* pszPassword);						//계좌인덱스 및 비밀번호 입력
    BOOL SetOrderPwd        (const char* pszHashOut,const char* pszPassword);										//거래비밀번호 입력
    BOOL SetAccountNoPwd    (const char* pszHashOut,const char* pszAccountNo,const char* pszPassword);				//계좌번호 및 비밀번호 입력
    BOOL SetHashPwd         (const char* pszHashOut,const char* pszKey,const char* pszPassword);		
    BOOL SetAccountNoByIndex(const char* pszHashOut,int nAccountIndex);						//계좌인덱스에 해당하는 계좌번호
};