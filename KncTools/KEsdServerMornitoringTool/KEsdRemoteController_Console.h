// KEsdRemoteController_Console.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호	

#include "CommonDefine.h"
#include "KEsdRemoteController_ConsoleImp.h"

// CKEsdRemoteController_ConsoleApp:
// 이 클래스의 구현에 대해서는 KEsdRemoteController_Console.cpp을 참조하십시오.
//

class CKEsdRemoteController_ConsoleApp : public CWinApp
{
public:
	CKEsdRemoteController_ConsoleApp();

// 재정의
	public:
	virtual BOOL InitInstance();

// 구현

	DECLARE_MESSAGE_MAP()

public:
	KEsdRemoteController_ConsoleImp m_kImp;

};

extern CKEsdRemoteController_ConsoleApp theApp;
