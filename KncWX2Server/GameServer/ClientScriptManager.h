#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "KncSend.h"


class KClientScriptManager
{
	DeclareSingleton( KClientScriptManager );
	DeclDump;
	DeclToStringW;

public:
	KClientScriptManager();
	~KClientScriptManager();

	bool LoadClientScript( IN const char* pFileName, IN int iScriptEnum );
	bool GetClientScriptInfo( IN int iScriptEnum, OUT std::wstring& wstrScriptInfo );

private:
	std::map< int, std::wstring >	m_mapScriptInfo;
};

DefSingletonInline( KClientScriptManager );