// stock-executor-dlg.h: 헤더 파일
//
#pragma once

#include <string>

#include "helper/xing_msg_sender.h"
#include "helper/xing_msg_receiver.h"

// CStockExecutorDlg 대화 상자
class CStockExecutorDlg : public CDHtmlDialog
{
// 생성입니다.
public:
    CStockExecutorDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_STOCKEXECUTOR_DIALOG, IDH = IDR_HTML_STOCKEXECUTOR_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

#ifdef _DEBUG
    // Action functions for debugging
    HRESULT OnButtonLogin(IHTMLElement* pElement);
    HRESULT OnButtonLogout(IHTMLElement* pElement);
    HRESULT OnButtonIsLogin(IHTMLElement* pElement);
    HRESULT OnButtonStocksByGubun(IHTMLElement* pElement);
    HRESULT OnButtonStockCurrentAskingPriceByCode(IHTMLElement* pElement);
    HRESULT OnButtonStockCurrentMarketPriceByCode(IHTMLElement* pElement);
    HRESULT OnButtonOK(IHTMLElement *pElement);
    HRESULT OnButtonCancel(IHTMLElement *pElement);
#endif

// 구현입니다.
protected:
    HICON m_hIcon;

    // 생성된 메시지 맵 함수
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // Executor functions
    afx_msg void OnLogin();
    afx_msg void OnLogout();
    afx_msg void OnIsLogin();
    afx_msg void OnStocksByGubun();
    afx_msg void OnStockCurrentAskingPriceByCode();
    afx_msg void OnStockCurrentMarketPriceByCode();

    // XingAPI functions
    afx_msg LRESULT OnWmDisconnectEvent(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWmLoginEvent(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWmLogoutEvent(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWmIsLoginEvent(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWmReceiveDataEvent(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWmTimeoutDataEvent(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWmErrorEvent(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
    DECLARE_DHTML_EVENT_MAP()

private:
    BOOL g_bLogin;
    CXingMsgSender m_xingMsgSender;
    CXingMsgReceiver m_xingMsgReceiver;
};
