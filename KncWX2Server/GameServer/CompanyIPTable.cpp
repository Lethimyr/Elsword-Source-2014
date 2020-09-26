#include "CompanyIPTable.h"

//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
//#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP

ImplementRefreshSingleton( KCompanyIPTable );

KCompanyIPTable::KCompanyIPTable(void)
{
}

KCompanyIPTable::~KCompanyIPTable(void)
{
}

ImplToStringW( KCompanyIPTable )
{
	stm_	<< L"----------[ CompanyIPTable ]----------" << std::endl
			<< TOSTRINGW( m_vecCompanyIPData.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KCompanyIPTable )
{
	lua_tinker::class_add<KCompanyIPTable>( GetLuaState(), "KCompanyIPTable" );

	lua_tinker::class_def<KCompanyIPTable>( GetLuaState(), "AddCompanyIP",		&KCompanyIPTable::AddCompanyIP_LUA );
	lua_tinker::class_def<KCompanyIPTable>( GetLuaState(), "dump",				&KCompanyIPTable::Dump );
	
	lua_tinker::decl( GetLuaState(), "g_pKCompanyIPTable", this );
}

bool KCompanyIPTable::AddCompanyIP_LUA( const char* szAddIP )
{
	
	if( szAddIP == NULL )
	{
		START_LOG( cerr, L"[운영자 치트 사용 환경]IP 스트링 이상." )
			<< END_LOG;

		return false;
	}

	std::string strIP = szAddIP;
	if( strIP.size() < 1 || strIP.size() > 15 )
	{
		START_LOG( cerr, L"[운영자 치트 사용 환경]추가하려는 IP 이상." )
			<< BUILD_LOG( strIP.size() )
			<< END_LOG;

		return false;
	}

	START_LOG( cout, L"=== [운영자 치트 사용 환경] ADD IP ===" )
		<< BUILD_LOG( strIP.c_str() )
		<< BUILD_LOG( strIP.size() )
		<< L"================"
		<< dbg::endl;

	m_vecCompanyIPData.push_back( strIP );


	return true;
}

bool KCompanyIPTable::CheckIP( const char* szIP )
{
	//////////////////////////////////////////////////////////////////////////
	// 운영자 치트 사용 권한이 있는 IP 인지 확인한다.
	// 권한이 있는 IP 라면 true
	//////////////////////////////////////////////////////////////////////////

	if( szIP == NULL )
	{
		START_LOG( cerr, L"IP가 없다고?? 없어도 되는 건가?" )
			<< END_LOG;

		return false;
	}

	std::string strIP = szIP;
	if( strIP.size() < 1 || strIP.size() > 15 )
	{
		START_LOG( cerr, L"이상한 IP 입니다." )
			<< BUILD_LOG( strIP.size() )
			<< END_LOG;

		return false;
	}

	std::vector< std::string >::iterator vit = m_vecCompanyIPData.begin();
	for( ; vit != m_vecCompanyIPData.end() ; ++vit )
	{
		if( strIP.compare( (*vit) ) == 0 )
		{
			return true;
		}
	}

	START_LOG( cerr, L"운영자 치트 사용 권한이 없는 IP 입니다." )
		<< BUILD_LOG( strIP )
		<< END_LOG;

	return false;
}

//#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
//}}