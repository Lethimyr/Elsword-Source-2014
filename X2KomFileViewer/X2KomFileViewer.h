// X2KomFileViewer.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호
#include "X2KomFileManager.h"


// CX2KomFileViewerApp:
// 이 클래스의 구현에 대해서는 X2KomFileViewer.cpp을 참조하십시오.
//

class CX2KomFileViewerApp : public CWinApp
{
public:
	CX2KomFileViewerApp();

// 재정의
	public:
	virtual BOOL InitInstance();

// 구현

	DECLARE_MESSAGE_MAP()

private:
	bool CommandLineExecute();

public:
	CX2KomFileManager				m_komFileManager;	
};

extern CX2KomFileViewerApp theApp;
