// stock-executor-dlg.cpp: 구현 파일
//
#include <string>
#include <cpprest/json.h>

#include "core/framework.h"
#include "stock-executor.h"
#include "about-dlg.h"
#include "stock-executor-dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD WM_STOCK_EXECUTOR_SETSTRINGVARIABLE = WM_USER + 1001;

// CStockExecutorDlg 대화 상자
BEGIN_DHTML_EVENT_MAP(CStockExecutorDlg)
#ifdef _DEBUG
    // for debugging
    DHTML_EVENT_ONCLICK(_T("ButtonLogin"), OnButtonLogin)
    DHTML_EVENT_ONCLICK(_T("ButtonLogout"), OnButtonLogout)
    DHTML_EVENT_ONCLICK(_T("ButtonIsLogin"), OnButtonIsLogin)
    DHTML_EVENT_ONCLICK(_T("ButtonStocksByGubun"), OnButtonStocksByGubun)
    DHTML_EVENT_ONCLICK(_T("ButtonStockCurrentAskingPriceByCode"), OnButtonStockCurrentAskingPriceByCode)
    DHTML_EVENT_ONCLICK(_T("ButtonStockCurrentMarketPriceByCode"), OnButtonStockCurrentMarketPriceByCode)
    DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
    DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
#endif
END_DHTML_EVENT_MAP()


CStockExecutorDlg::CStockExecutorDlg(CWnd* pParent /*=nullptr*/)
    : CDHtmlDialog(IDD_STOCKEXECUTOR_DIALOG, IDR_HTML_STOCKEXECUTOR_DIALOG, pParent), g_bLogin(FALSE)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStockExecutorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStockExecutorDlg, CDHtmlDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_COPYDATA()

    // for Executor
    ON_BN_CLICKED(IDC_BTN_LOGIN, OnLogin)
    ON_BN_CLICKED(IDC_BTN_LOGOUT, OnLogout)
    ON_BN_CLICKED(IDC_BTN_ISLOGIN, OnIsLogin)
    ON_BN_CLICKED(IDC_BTN_STOCKSBYGUBUN, OnStocksByGubun)
    ON_BN_CLICKED(IDC_BTN_STOCKCURRENTASKINGPRICE, OnStockCurrentAskingPriceByCode)
    ON_BN_CLICKED(IDC_BTN_STOCKCURRENTMARKETPRICE, OnStockCurrentMarketPriceByCode)

    // for XingAPIEvent
    ON_MESSAGE(WM_USER + XM_LOGIN, OnWmLoginEvent)
    ON_MESSAGE(WM_USER + XM_CM_ISLOGIN, OnWmIsLoginEvent)
    ON_MESSAGE(WM_USER + XM_RECEIVE_DATA, OnWmReceiveDataEvent)
    ON_MESSAGE(WM_USER + XM_TIMEOUT_DATA, OnWmTimeoutDataEvent)
    ON_MESSAGE(WM_USER + XM_CM_ERROR, OnWmErrorEvent)

END_MESSAGE_MAP()


// CStockExecutorDlg 메시지 처리기

BOOL CStockExecutorDlg::OnInitDialog()
{
    CDHtmlDialog::OnInitDialog();

    // 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

    // IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
    //  프레임워크가 이 작업을 자동으로 수행합니다.
    SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
    SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

#ifndef _DEBUG
    // 숨김처리 (최소화후 숨겨야 화면에 나타나지 않음)
    ShowWindow(SW_SHOWMINIMIZED);
    PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
#endif

    // TODO: 여기에 추가 초기화 작업을 추가합니다.

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CStockExecutorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDHtmlDialog::OnSysCommand(nID, lParam);
    }
}

// This is a function to receive data from another application.
BOOL CStockExecutorDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
    switch (pCopyDataStruct->dwData) {
    case WM_STOCK_EXECUTOR_SETSTRINGVARIABLE:
    {
        m_xingMsgSender.m_strData.clear();
        m_xingMsgSender.m_strData = (LPCWSTR)pCopyDataStruct->lpData;
        break;
    }
    default:
        break;
    }

    return CDHtmlDialog::OnCopyData(pWnd, pCopyDataStruct);

}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CStockExecutorDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 아이콘을 그립니다.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDHtmlDialog::OnPaint();
    }
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CStockExecutorDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

#ifdef _DEBUG
HRESULT CStockExecutorDlg::OnButtonLogin(IHTMLElement* /*pElement*/)
{
    web::json::value requestJson;
    requestJson[L"id"] = web::json::value::string(L"id");
    requestJson[L"pw"] = web::json::value::string(L"pw");
    requestJson[L"certPw"] = web::json::value::string(L"certPw");
    //requestJson[L"serverType"] = web::json::value::string(L"serverType"); Optional
    std::wstring jsonString = requestJson.serialize();

    COPYDATASTRUCT cds;
    cds.dwData = WM_STOCK_EXECUTOR_SETSTRINGVARIABLE;
    cds.cbData = jsonString.size();
    cds.lpData = (PVOID)jsonString.c_str();

    SendMessage(WM_COPYDATA, 0, (LPARAM)&cds);
    SendMessage(WM_COMMAND, IDC_BTN_LOGIN, 0);
    return S_OK;
}

HRESULT CStockExecutorDlg::OnButtonLogout(IHTMLElement* /*pElement*/)
{
    SendMessage(WM_COMMAND, IDC_BTN_LOGOUT, 0);
    return S_OK;
}

HRESULT CStockExecutorDlg::OnButtonIsLogin(IHTMLElement* /*pElement*/)
{
    SendMessage(WM_COMMAND, IDC_BTN_ISLOGIN, 0);
    return S_OK;
}

HRESULT CStockExecutorDlg::OnButtonStocksByGubun(IHTMLElement* /*pElement*/)
{
    web::json::value requestJson;
    requestJson[L"gubun"] = web::json::value::string(L"1"); // ALL: "0", KOSPI: "1" | KOSDAQ: "2"
    std::wstring jsonString = requestJson.serialize();

    COPYDATASTRUCT cds;
    cds.dwData = WM_STOCK_EXECUTOR_SETSTRINGVARIABLE;
    cds.cbData = jsonString.size();
    cds.lpData = (PVOID)jsonString.c_str();

    SendMessage(WM_COPYDATA, 0, (LPARAM)&cds);
    SendMessage(WM_COMMAND, IDC_BTN_STOCKSBYGUBUN, 0);
    return S_OK;
}

HRESULT CStockExecutorDlg::OnButtonStockCurrentAskingPriceByCode(IHTMLElement* pElement)
{
    web::json::value requestJson;
    requestJson[L"code"] = web::json::value::string(L"078020"); // 이베스트투자증권 코드(078020)
    std::wstring jsonString = requestJson.serialize();

    COPYDATASTRUCT cds;
    cds.dwData = WM_STOCK_EXECUTOR_SETSTRINGVARIABLE;
    cds.cbData = jsonString.size();
    cds.lpData = (PVOID)jsonString.c_str();

    SendMessage(WM_COPYDATA, 0, (LPARAM)&cds);
    SendMessage(WM_COMMAND, IDC_BTN_STOCKCURRENTASKINGPRICE, 0);
    return S_OK;
}

HRESULT CStockExecutorDlg::OnButtonStockCurrentMarketPriceByCode(IHTMLElement* pElement)
{
    web::json::value requestJson;
    requestJson[L"code"] = web::json::value::string(L"078020"); // 이베스트투자증권 코드(078020)
    std::wstring jsonString = requestJson.serialize();

    COPYDATASTRUCT cds;
    cds.dwData = WM_STOCK_EXECUTOR_SETSTRINGVARIABLE;
    cds.cbData = jsonString.size();
    cds.lpData = (PVOID)jsonString.c_str();

    SendMessage(WM_COPYDATA, 0, (LPARAM)&cds);
    SendMessage(WM_COMMAND, IDC_BTN_STOCKCURRENTMARKETPRICE, 0);
    return S_OK;
}

HRESULT CStockExecutorDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
    OnOK();
    return S_OK;
}

HRESULT CStockExecutorDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
    OnCancel();
    return S_OK;
}
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 로그인 시작
void CStockExecutorDlg::OnLogin()
{
    // 서버접속
    if (m_xingMsgSender.Connect(GetSafeHwnd()) == FALSE) {
        return;
    }
    // 로그인
    m_xingMsgSender.Login(GetSafeHwnd());
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStockExecutorDlg::OnLogout()
{
    m_xingMsgSender.Logout(GetSafeHwnd());
    m_xingMsgSender.Disconnect();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStockExecutorDlg::OnIsLogin()
{
    m_xingMsgSender.IsLogin(GetSafeHwnd(), g_bLogin);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStockExecutorDlg::OnStocksByGubun()
{
    m_xingMsgSender.StocksByGubun(GetSafeHwnd());
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStockExecutorDlg::OnStockCurrentAskingPriceByCode()
{
    m_xingMsgSender.StockCurrentAskingPriceByCode(GetSafeHwnd());
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStockExecutorDlg::OnStockCurrentMarketPriceByCode()
{
    m_xingMsgSender.StockCurrentMarketPriceByCode(GetSafeHwnd());
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	XingAPI.dll로 부터 수신한 윈도우 메시지를 통해 각 이벤트 핸들러
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
LRESULT CStockExecutorDlg::OnWmLoginEvent(WPARAM wParam, LPARAM lParam)
{
    g_bLogin = m_xingMsgReceiver.LoginEvent(atoi((LPCSTR)wParam), (LPCSTR)lParam);
    return 0L;
}

LRESULT CStockExecutorDlg::OnWmIsLoginEvent(WPARAM wParam, LPARAM lParam)
{
    m_xingMsgReceiver.IsLoginEvent(((BOOL)wParam) == TRUE);
    return 0L;
}

LRESULT CStockExecutorDlg::OnWmReceiveDataEvent(WPARAM wParam, LPARAM lParam)
{
    if (wParam == REQUEST_DATA) {
        // Data를 받음
        m_xingMsgReceiver.ReceiveRequestDataEvent((LPRECV_PACKET)lParam);
    }
    else if (wParam == MESSAGE_DATA)  {
        // 메시지를 받음
        m_xingMsgReceiver.ReceiveMessageDataEvent((LPMSG_PACKET)lParam);
    }
    else if (wParam == SYSTEM_ERROR_DATA) {
        // System Error를 받음
        m_xingMsgReceiver.ReceiveSystemErrorDataEvent((LPMSG_PACKET)lParam);
    }
    else if (wParam == RELEASE_DATA) {
        // Release Data를 받음
        m_xingMsgReceiver.ReceiveReleaseDataEvent((int)lParam);
    }

    return 0L;
}

LRESULT CStockExecutorDlg::OnWmTimeoutDataEvent(WPARAM wParam, LPARAM lParam)
{
    m_xingMsgReceiver.ReceiveReleaseDataEvent((int)lParam);
    return 0L;
}

LRESULT CStockExecutorDlg::OnWmErrorEvent(WPARAM wParam, LPARAM lParam)
{
    m_xingMsgReceiver.ErrorEvent((int)wParam, (LPCSTR)lParam);
    return 0L;
}