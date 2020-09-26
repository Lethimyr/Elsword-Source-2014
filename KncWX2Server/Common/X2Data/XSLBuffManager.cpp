#include ".\xslbuffmanager.h"
#include "NetError.h"

ImplementRefreshSingleton( CXSLBuffManager );

CXSLBuffManager::CXSLBuffManager(void)
{
	
}

CXSLBuffManager::~CXSLBuffManager(void)
{
}

ImplToStringW( CXSLBuffManager )
{
	stm_	<< L"----------[ Buff Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapBuffList.size() );
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLBuffManager )
{
	lua_tinker::class_add<CXSLBuffManager>( GetLuaState(), "CXSLBuffManager" );
	lua_tinker::class_def<CXSLBuffManager>( GetLuaState(), "AddBuffInfo",				&CXSLBuffManager::AddBuffInfo_LUA );

	lua_tinker::class_def<CXSLBuffManager>( GetLuaState(), "dump",						&CXSLBuffManager::Dump );
	
	lua_tinker::decl( GetLuaState(), "BuffManager", this );
}

bool CXSLBuffManager::AddBuffInfo_LUA()
{
	KLuaManager luaManager( GetLuaState() );

	BuffTemplet kBuffInfo;
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eBuffID",				kBuffInfo.m_eBuffID,					BUFF_TEMPLET_ID,		BTI_NONE,		goto end_proc );	
	//LUA_GET_VALUE(			luaManager, L"m_TitleName",				kBuffInfo.m_wstrBuffName,				L"" );
	LUA_GET_VALUE(				luaManager, L"m_fBuffPeriodSecond",		kBuffInfo.m_fBuffPeriodSecond,			0.0 );

	//{{ 2012. 05. 6	¹Ú¼¼ÈÆ	¾îµÒÀÇ ¹® °³Æí
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	LUA_GET_VALUE( luaManager, L"m_bEndBuffAtTheLeaveDungeonRoom",	kBuffInfo.m_bEndBuffAtTheLeaveDungeonRoom,	false );
	LUA_GET_VALUE( luaManager, L"m_bEndBuffAtTheEndDungeonGame",	kBuffInfo.m_bEndBuffAtTheEndDungeonGame,	false );

	//{{ 2012. 07. 11	¹Ú¼¼ÈÆ	2012 ´ëÀü ½ÃÁð2
#ifdef SERV_2012_PVP_SEASON2_2
	LUA_GET_VALUE( luaManager, L"m_bEndBuffAtTheLeavePvpRoom",		kBuffInfo.m_bEndBuffAtTheLeavePvpRoom,		false );
	LUA_GET_VALUE( luaManager, L"m_bEndBuffAtTheEndPvpGame",		kBuffInfo.m_bEndBuffAtTheEndPvpGame,		false );
#endif SERV_2012_PVP_SEASON2_2
	//}}

	LUA_GET_VALUE( luaManager, L"m_bEndBuffAtTheUserDied",			kBuffInfo.m_bEndBuffAtTheUserDied,			false );
	LUA_GET_VALUE( luaManager, L"m_bKeepBuffAtTheUserLogOut",		kBuffInfo.m_bKeepBuffAtTheUserLogOut,		false );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

#ifdef SERV_SERVER_BUFF_SYSTEM// ÀÛ¾÷³¯Â¥: 2013-05-14	// ¹Ú¼¼ÈÆ
	LUA_GET_VALUE( luaManager, L"m_bEndBuffAtTheLeaveBattleFieldRoom",	kBuffInfo.m_bEndBuffAtTheLeaveBattleFieldRoom,	false );
#endif // SERV_SERVER_BUFF_SYSTEM

	if( luaManager.BeginTable( L"SOCKET_OPTION" ) == S_OK )
	{
		int index = 1;
		int iSocketOption	= -1;
		while( luaManager.GetValue( index, iSocketOption ) == S_OK )
		{
			if( iSocketOption > 0 )
			{
				kBuffInfo.m_vecOption.push_back( iSocketOption );
			}
			++index;
		}
		luaManager.EndTable();
	}

	//{{ ¹öÇÁ °ü¸® ½Ã½ºÅÛ - ±è¹Î¼º
#ifdef SERV_SERVER_BUFF_SYSTEM
	if( luaManager.BeginTable( L"BUFF_FACTOR" ) == S_OK )
	{
		int index = 1;
		int iFactorID	= -1;

		if( luaManager.GetValue( index, iFactorID ) == S_OK )
		{
			if( iFactorID > 0 )
			{
				kBuffInfo.m_iFactorID = iFactorID;
			}
		}
		luaManager.EndTable();
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}
	
	//{{ 2012. 04. 18	¹Ú¼¼ÈÆ	¾îµÒÀÇ ¹® °³Æí
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	if( luaManager.BeginTable( L"USING_BUFF_ROOM_TYPE" ) == S_OK )
	{
		int index = 1;
		int iUsingBuffRoomType	= -1;
		while( luaManager.GetValue( index, iUsingBuffRoomType ) == S_OK )
		{
			if( 0 <= iUsingBuffRoomType )
			{
				kBuffInfo.m_vecRoomType.push_back( iUsingBuffRoomType );
			}
			++index;
		}
		luaManager.EndTable();
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	m_mapBuffList.insert( std::make_pair( kBuffInfo.m_eBuffID, kBuffInfo ) );
	return true;

end_proc:
	START_LOG( cerr, L"¹öÇÁ ½ºÅ©¸³Æ® ÆÄ½Ì ½ÇÆÐ.!" )
		<< BUILD_LOG( kBuffInfo.m_eBuffID )
		<< END_LOG;

	return false;
}

const CXSLBuffManager::BuffTemplet* CXSLBuffManager::GetBuffTemplet( IN const int iBuffID ) const
{
	std::map< BUFF_TEMPLET_ID, BuffTemplet >::const_iterator mit;
	mit = m_mapBuffList.find( static_cast<BUFF_TEMPLET_ID>(iBuffID) );
	if( mit == m_mapBuffList.end() )
		return NULL;

	return &mit->second;
}

//{{ 2012. 04. 17	¹Ú¼¼ÈÆ	¾îµÒÀÇ ¹® °³Æí
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
bool CXSLBuffManager::BuffTemplet::CheckRoomType( IN const int& iCheckRoomType ) const
{
	BOOST_TEST_FOREACH( int, iRoomType, m_vecRoomType )
	{
		if( iCheckRoomType == iRoomType )
		{
			return true;
		}
	}
	return false;
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

