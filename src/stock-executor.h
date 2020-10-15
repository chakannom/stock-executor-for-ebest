
// stock-executor.h: PROJECT_NAME 애플리케이션에 대한 주 헤더 파일입니다.
//

#pragma once

#include "core/resource.h"		// 주 기호입니다.


// CStockExecutorApp:
// 이 클래스의 구현에 대해서는 stock-executor.cpp을(를) 참조하세요.
//

class CStockExecutorApp : public CWinApp
{
public:
    CStockExecutorApp();

// 재정의입니다.
public:
    virtual BOOL InitInstance();

// 구현입니다.

    DECLARE_MESSAGE_MAP()
private:
    BOOL PreventRunning();
};

extern CStockExecutorApp theApp;
