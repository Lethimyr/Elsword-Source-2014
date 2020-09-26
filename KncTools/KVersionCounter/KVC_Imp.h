#pragma once

#include <Windows.h>
#include <sys/stat.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

class KVC_Imp
{

public:
	KVC_Imp();
	virtual ~KVC_Imp();

	void Inc_EsdPchPath_VersionByTag(std::string strTag, std::string& strChunk);
	void Inc_EsdCfgGs_VersionByTag(std::string strTag, std::string& strChunk);
	void Inc_EsdCfg_VersionByTag(std::string strTag, std::string& strChunk);
	void InitEsdVersion(std::string strVersion);

	void ProcessFile(char* pcFilename, std::string strExt);	
	std::string GetEsdVersion() { return m_strEsdVersion; }
	void SetEsdVersion( std::string val ) { m_strEsdVersion = val; }

protected:
	std::string m_strEsdVersion;	
};
