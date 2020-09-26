// X2ItemTool.h : X2ItemTool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호


// CX2ItemToolApp:
// 이 클래스의 구현에 대해서는 X2ItemTool.cpp을 참조하십시오.
//

class CX2ItemToolApp : public CWinApp
{
public:
	CX2ItemToolApp();


// 재정의
public:
	virtual BOOL InitInstance();

// 구현
	COleTemplateServer m_server;
		// 문서 만들기에 대한 서버 개체입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CX2ItemToolApp theApp;
