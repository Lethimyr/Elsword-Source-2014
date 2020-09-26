#pragma once
#include "ServerDefine.h"

#include "RefreshSingleton.h"
#include <map>
#include <vector>
//#include "ServerDefine.h"

//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
//#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP

class KCompanyIPTable
{
	DeclareRefreshSingleton( KCompanyIPTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:
	KCompanyIPTable(void);
	~KCompanyIPTable(void);

	// for lua
	bool AddCompanyIP_LUA( const char* szAddIP );


    bool CheckIP( const char* szIP );


private:
	
	std::vector< std::string >  m_vecCompanyIPData;

};

DefRefreshSingletonInline( KCompanyIPTable );

//#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
//}}
