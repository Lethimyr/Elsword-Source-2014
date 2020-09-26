// X2ChinaLauncher.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호

class TiXmlNode;

// CX2ChinaLauncherApp:
// 이 클래스의 구현에 대해서는 X2ChinaLauncher.cpp을 참조하십시오.
//

class CX2ChinaLauncherApp : public CWinApp
{
public:
	CX2ChinaLauncherApp();

// 재정의
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현

	DECLARE_MESSAGE_MAP()

private:
	struct Addr
	{
		std::string strValue;
		std::string strPath;
	};

	void InitLauncher();
	BOOL DeleteFileForce(const std::string filename);

#if 0
	BOOL LoadLauncherPatchServer();
	BOOL LoadAddrTag(TiXmlNode* pkNode);
	BOOL GetVersionInServer(const std::string& strAddr, const std::string& strPath, unsigned int& uiChecksum);
	BOOL IsNewVersion(unsigned int uiServerChecksum);
	BOOL PatchLauncher(const std::string& strAddr, const std::string& strPath, const std::string strLauncherName);
	BOOL ReRunLauncher();
#endif

	std::vector<Addr> m_vecAddr;

	std::string m_strLocalPath;
	std::string m_strLauncherName;

};

extern CX2ChinaLauncherApp theApp;
