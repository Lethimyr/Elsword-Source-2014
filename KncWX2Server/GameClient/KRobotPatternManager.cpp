#include "KncRobotManager.h"
#include "KRobotPatternManager.h"
#include "../Common/X2Data/XSLMapData.h"

ImplementSingleton( KRobotPatternManager );


KRobotPatternManager::KRobotPatternManager(void)
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();

	lua_tinker::class_add<KRobotPatternManager>( spLua.get(), "KRobotPatternManager" );
	lua_tinker::class_def<KRobotPatternManager>( spLua.get(), "AddRobotPattern",		&KRobotPatternManager::AddRobotPattern_LUA );
	lua_tinker::decl( spLua.get(), "RobotManager", this );
}

KRobotPatternManager::~KRobotPatternManager(void)
{
}

bool KRobotPatternManager::OpenScriptFile( const char* pFileName )
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();

	if( 0 != luaL_dofile( spLua.get(), pFileName ) ) return false;

	return true;
}

bool KRobotPatternManager::AddRobotPattern_LUA()
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();
	KLuaManager kLuamanager( spLua.get() );
	
	int iPatternIndex = 0;
	KRobotPattern kRobotPattern;

	LUA_GET_VALUE_RETURN( kLuamanager,	L"m_iMapID",		kRobotPattern.m_MapID,	0,			return false; );
	LUA_GET_VALUE(		  kLuamanager,	L"m_iPatternIndex",	iPatternIndex,			0						  );

	std::map< int, KPatternList >::iterator mit;
	mit = m_mapFieldPatternList.find( kRobotPattern.m_MapID );
	if( mit == m_mapFieldPatternList.end() )
	{
        KPatternList kPatternList;
		kPatternList.push_back( iPatternIndex );
		m_mapFieldPatternList.insert( std::make_pair( kRobotPattern.m_MapID, kPatternList ) );
	}
	else
	{
		mit->second.push_back( iPatternIndex );
	}

	LUA_GET_VALUE(		  kLuamanager,	L"m_bIsRight",		kRobotPattern.m_bIsRight,	false		  );
	LUA_GET_VALUE(		  kLuamanager,	L"m_vPos_X",		kRobotPattern.m_vPos.x,		0.0f		  );
	LUA_GET_VALUE(		  kLuamanager,	L"m_vPos_Y",		kRobotPattern.m_vPos.y,		0.0f		  );
	LUA_GET_VALUE(		  kLuamanager,	L"m_vPos_Z",		kRobotPattern.m_vPos.z,		0.0f		  );

	if( kLuamanager.BeginTable( "FrameTable" ) == S_OK )
	{
		int index = 1;
		int startPosId = 0;
		while( kLuamanager.BeginTable( index ) == S_OK )
		{
			int iFrameCount = 0;
			int iStateID = 0;
			KFieldFrameInfo kFrameInfo;
			
			LUA_GET_VALUE( kLuamanager, L"m_FrameCount", iFrameCount,	0 );
			LUA_GET_VALUE( kLuamanager, L"m_StateID",	 iStateID,		0 );

			kFrameInfo.m_FrameCount = iFrameCount;
			kFrameInfo.m_StateID = iStateID;

			kLuamanager.EndTable();
			++index;

			kRobotPattern.m_vecFieldFrame.push_back( kFrameInfo );
		}

		kLuamanager.EndTable();
	}

	std::map< int, std::vector< KRobotPattern > >::iterator mitPT;
	mitPT = m_mapPattern.find( iPatternIndex );
	if( mitPT == m_mapPattern.end() )
	{
		std::vector< KRobotPattern > vecPatternVector;
		vecPatternVector.push_back( kRobotPattern );
		m_mapPattern.insert( std::make_pair( iPatternIndex, vecPatternVector ) );
	}
	else
	{
		mitPT->second.push_back( kRobotPattern );
	}	
	return true;
}

bool KRobotPatternManager::GetRandomFieldRobotPattern( IN int iMapID, OUT std::list< KRobotPattern >& listPattern )
{
	listPattern.clear();

	std::map< int, KPatternList >::const_iterator mit;
	mit = m_mapFieldPatternList.find( iMapID );
	if( mit == m_mapFieldPatternList.end() )
	{
		START_LOG( cerr, L"랜덤으로 로봇패턴 얻기 실패!" )
			<< END_LOG;
		return false;
	}

	const KPatternList& kPatternList = mit->second;
	if( kPatternList.size() <= 0 )
	{
		START_LOG( cerr, L"패턴리스트가 이상하다!" )
			<< BUILD_LOG( iMapID )
			<< BUILD_LOG( kPatternList.size() )
			<< END_LOG;
		return false;
	}
		
	int iPatternIndex = rand() % kPatternList.size();

	// 패턴 얻기
	std::map< int, std::vector< KRobotPattern > >::const_iterator mitPT;
	mitPT = m_mapPattern.find( kPatternList[iPatternIndex] );
	if( mitPT == m_mapPattern.end() )
	{
		START_LOG( cerr, L"존재하지않는 Pattern Index다!" )
			<< BUILD_LOG( kPatternList[iPatternIndex] )
			<< END_LOG;
		return false;
	}
	
	std::vector< KRobotPattern >::const_iterator vit;
	for( vit = mitPT->second.begin(); vit != mitPT->second.end(); ++vit )
	{
		listPattern.push_back( *vit );
	}

	// 끝부분 잘라먹기
	int iDelCnt = rand() & 3;
	for( int i = 0; i < iDelCnt; ++i )
	{
        listPattern.pop_back();
	}

	return true;
}

int	KRobotPatternManager::GetRandomMapID()
{
	int iMapID = m_kMapLottery.Decision();
	if( iMapID == KLottery::CASE_BLANK )
		iMapID = CXSLMapData::MI_RUBEN;

	return iMapID;
}


