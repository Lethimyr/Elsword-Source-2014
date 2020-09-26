// X2EncryptFileViewer.h : X2EncryptFileViewer 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호
#include <vector>


// CX2EncryptFileViewerApp:
// 이 클래스의 구현에 대해서는 X2EncryptFileViewer.cpp을 참조하십시오.
//

class CX2EncryptFileViewerApp : public CWinApp
{
public:
	CX2EncryptFileViewerApp();
	std::vector< int >& GetEncryptionKeyList() { return m_vecEncryptionKey; }

// 재정의
public:
	virtual BOOL InitInstance();
	

// 구현
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()



private:
	void LoadConfig();
	bool AddEncryptionKey_Lua();

	PCHAR* CommandLineToArgvA( PCHAR CmdLine, int* _argc );
	
	std::vector< int > m_vecEncryptionKey;
};

extern CX2EncryptFileViewerApp theApp;
