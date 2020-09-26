#include "DailyGiftBoxManager.h"

//{{ 2012. 06. 05	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX

ImplementRefreshSingleton( KDailyGiftBoxManager );

KDailyGiftBoxManager::KDailyGiftBoxManager()
{

}

KDailyGiftBoxManager::~KDailyGiftBoxManager()
{

}

ImplToStringW( KDailyGiftBoxManager )
{
	stm_	<< L"----------[ DailyGiftBoxManager ]----------" << std::endl
		<< TOSTRINGW( m_setItemID.size() )
		<< TOSTRINGW( m_mapPackageInfo.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KDailyGiftBoxManager )
{
	lua_tinker::class_add<KDailyGiftBoxManager>( GetLuaState(), "KDailyGiftBoxManager" );

	lua_tinker::class_def<KDailyGiftBoxManager>( GetLuaState(), "AddPackageInfo",	&KDailyGiftBoxManager::AddPackageInfo_Lua );

	lua_tinker::decl( GetLuaState(), "g_pKDailyGiftBoxManager", this );
}

void KDailyGiftBoxManager::AddPackageInfo_Lua( IN int iItemID, IN int iScriptID )
{
	std::pair<std::map<int, int>::iterator, bool> result = m_mapPackageInfo.insert( std::map<int, int>::value_type( iScriptID, iItemID ) );
	if( result.second == false )
	{
		START_LOG( cerr, L"해당 ScriptID는 이미 사용 중인 정보입니다.")
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( result.first->second )
			<< BUILD_LOG( iScriptID )
			<< BUILD_LOG( result.first->first )
			<< END_LOG;
		return;
	}
	m_setItemID.insert( iItemID );
}

bool KDailyGiftBoxManager::IsDailyGiftBoxItemID( IN const int& iItemID ) const
{
	return ( m_setItemID.find( iItemID ) != m_setItemID.end() ) ? true : false;
}

bool KDailyGiftBoxManager::IsDailyGiftBoxScriptID( IN const int& iScriptID, OUT int& iItemID ) const
{
	std::map<int /*iScriptID*/, int /*iItemID*/>::const_iterator it = m_mapPackageInfo.find( iScriptID );
	if( it != m_mapPackageInfo.end() )
	{
		iItemID = it->second;
		return true;
	}
	return false;
}
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}
