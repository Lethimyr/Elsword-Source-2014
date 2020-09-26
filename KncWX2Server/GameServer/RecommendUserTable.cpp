#include ".\recommendusertable.h"


ImplementRefreshSingleton( KRecommendUserTable );

KRecommendUserTable::KRecommendUserTable(void)
{
}

KRecommendUserTable::~KRecommendUserTable(void)
{
}

ImplToStringW( KRecommendUserTable )
{
	stm_	<< L"----------[ Recommend User Table ]----------" << std::endl
			<< TOSTRINGW( m_mapRecommendData.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KRecommendUserTable )
{
	lua_tinker::class_add<KRecommendUserTable>( GetLuaState(), "KRecommendUserTable" );
	lua_tinker::class_def<KRecommendUserTable>( GetLuaState(), "AddRecommendUserData",	&KRecommendUserTable::AddRecommendUserData_LUA );
	lua_tinker::class_def<KRecommendUserTable>( GetLuaState(), "dump",					&KRecommendUserTable::Dump );

	//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
	lua_tinker::class_def<KRecommendUserTable>( GetLuaState(), "AddRecommendEventReward",	&KRecommendUserTable::AddRecommendEventReward_LUA );
	lua_tinker::class_def<KRecommendUserTable>( GetLuaState(), "AddRecommendedEventReward1",	&KRecommendUserTable::AddRecommendedEventReward1_LUA );
	lua_tinker::class_def<KRecommendUserTable>( GetLuaState(), "AddRecommendedEventReward2",	&KRecommendUserTable::AddRecommendedEventReward2_LUA );
	lua_tinker::class_def<KRecommendUserTable>( GetLuaState(), "AddRecommendUserEventReward",	&KRecommendUserTable::AddRecommendUserEventReward_LUA );
#endif SERV_RECOMMEND_LIST_EVENT
	//}

	lua_tinker::decl( GetLuaState(), "g_pKRecommendUserTable", this );
}

int KRecommendUserTable::GetRecommendUserReward( IN u_char ucLevel ) const
{
	std::map< u_char, int >::const_iterator mit;
	mit = m_mapRecommendData.find( ucLevel );
	if( mit == m_mapRecommendData.end() )
		return 0;

	return mit->second;
}

bool KRecommendUserTable::AddRecommendUserData_LUA( u_char ucLevel, int iRewardID )
{
	if( ucLevel <= 1 || iRewardID <= 0 )
	{
		START_LOG( cerr, L"추천인 테이블 정보 파싱 실패!" )
			<< BUILD_LOG( ucLevel )
			<< BUILD_LOG( iRewardID )
			<< END_LOG;

		return false;
	}

	m_mapRecommendData.insert( std::make_pair( ucLevel, iRewardID ) );
	return true;
}

//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
bool KRecommendUserTable::AddRecommendEventReward_LUA( int iRewardID )
{
	if( iRewardID <= 0 )
	{
		START_LOG( cerr, L"event 추천인 테이블 정보 파싱 실패!" )
			<< BUILD_LOG( iRewardID )
			<< END_LOG;

		return false;
	}

	m_iRecommendEventReward = iRewardID;
	return true;
}

bool KRecommendUserTable::AddRecommendedEventReward1_LUA( int iRewardID )
{
	if( iRewardID <= 0 )
	{
		START_LOG( cerr, L"event 추천인 테이블 정보 파싱 실패!" )
			<< BUILD_LOG( iRewardID )
			<< END_LOG;

		return false;
	}

	m_iRecommendedEventReward1 = iRewardID;
	return true;
}

bool KRecommendUserTable::AddRecommendedEventReward2_LUA( int iRewardID )
{
	if( iRewardID <= 0 )
	{
		START_LOG( cerr, L"event 추천인 테이블 정보 파싱 실패!" )
			<< BUILD_LOG( iRewardID )
			<< END_LOG;

		return false;
	}

	m_iRecommendedEventReward2 = iRewardID;
	return true;
}

int KRecommendUserTable::GetRecommendUserEventReward( u_char ucLevel )
{
	std::map< u_char, int >::const_iterator mit;
	mit = m_mapRecommendEventData.find( ucLevel );
	if( mit == m_mapRecommendEventData.end() )
		return 0;

	return mit->second;
}

bool KRecommendUserTable::AddRecommendUserEventReward_LUA( u_char ucLevel, int iRewardID )
{
	if( ucLevel <= 1 || iRewardID <= 0 )
	{
		START_LOG( cerr, L"추천인 테이블 정보 파싱 실패!" )
			<< BUILD_LOG( ucLevel )
			<< BUILD_LOG( iRewardID )
			<< END_LOG;

		return false;
	}

	m_mapRecommendEventData.insert( std::make_pair( ucLevel, iRewardID ) );
	return true;
}
#endif SERV_RECOMMEND_LIST_EVENT
//}