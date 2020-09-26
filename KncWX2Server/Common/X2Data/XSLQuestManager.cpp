#include "xslquestmanager.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef SERV_ALLOW_EVENT_ERASE
#include "GameEvent/GameEventManager.h"
#endif //SERV_ALLOW_EVENT_ERASE

ImplementRefreshSingleton( CXSLQuestManager );

CXSLQuestManager::CXSLQuestManager(void)
{
}

CXSLQuestManager::~CXSLQuestManager(void)
{
}

ImplToStringW( CXSLQuestManager )
{
	stm_	<< L"----------[ Quest Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapQuestTemplet.size() )
			<< TOSTRINGW( m_mapSubQuestTemplet.size() )
#ifdef SERV_EPIC_QUEST
			<< TOSTRINGW( m_vecEpicQuestTemplet.size() );
#endif SERV_EPIC_QUEST
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLQuestManager )
{
	lua_tinker::class_add<CXSLQuestManager>( GetLuaState(), "CXSLQuestManager" );

	lua_tinker::class_def<CXSLQuestManager>( GetLuaState(), "AddQuestTemplet_LUA",		&CXSLQuestManager::AddQuestTemplet_LUA );
	lua_tinker::class_def<CXSLQuestManager>( GetLuaState(), "AddSubQuestTemplet_LUA",	&CXSLQuestManager::AddSubQuestTemplet_LUA );	

	//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	lua_tinker::class_def<CXSLQuestManager>( GetLuaState(), "AddAccountQuestTemplet_LUA",		&CXSLQuestManager::AddAccountQuestTemplet_LUA );
	//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	lua_tinker::class_def<CXSLQuestManager>( GetLuaState(), "AddAccountQuestDate",		&CXSLQuestManager::AddAccountQuestDate_LUA );
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	lua_tinker::decl( GetLuaState(), "g_pQuestManager", this );
}

bool CXSLQuestManager::AddQuestTemplet_LUA()
{
	QuestTemplet kQuestTemplet;
	KLuaManager luaManager( GetLuaState() );

	std::wstring wstrErrLua;

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_iID",				kQuestTemplet.m_iID,			0, wstrErrLua, goto LoadFail; );

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 12. 18  최육사	서버군 확장
	int iServerGroupID = 0;
	LUA_GET_VALUE(					luaManager, L"m_iServerGroupID",	iServerGroupID,					-1 );
#ifdef EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID != -1 )
#else EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID != SEnum::SGI_INVALID )
#endif EXTEND_SERVER_GROUP_MASK
	{
		if( SEnum::IsValidServerGroupID( iServerGroupID ) == false )
		{
			START_LOG( cerr, L"정상적인 서버군ID값이 아닙니다." )
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( kQuestTemplet.m_iID );
			return false;
		}

		if( KBaseServer::GetKObj()->GetServerGroupID() != iServerGroupID )
		{
			START_LOG( clog2, L"서버군이 다르므로 해당 퀘스트는 파싱하지 않습니다." )
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( kQuestTemplet.m_iID );
			return true;
		}
	}
	//}}
	//////////////////////////////////////////////////////////////////////////	

	LUA_GET_VALUE_RETURN_ENUM_ERR(	luaManager, L"m_eQuestType",		kQuestTemplet.m_eQuestType,		QUEST_TYPE,	QT_NORMAL, wstrErrLua, goto LoadFail; );

#ifdef EXCEPTION_EPIC_QUEST_TEMPLET
	if(kQuestTemplet.m_eQuestType >= 5)
	{
		return true;
	}

#endif EXCEPTION_EPIC_QUEST_TEMPLET

	LUA_GET_VALUE_RETURN_ENUM_ERR(	luaManager, L"m_eStartNPCID",		kQuestTemplet.m_eStartNPCID,	CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE,	wstrErrLua, goto LoadFail; );
	LUA_GET_VALUE_RETURN_ENUM_ERR(	luaManager, L"m_eEndNPCID",			kQuestTemplet.m_eEndNPCID,		CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE,	wstrErrLua, goto LoadFail; );

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_wstrTitle",			kQuestTemplet.m_wstrTitle,		L"",	wstrErrLua, goto LoadFail; );
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_iFairLevel",		kQuestTemplet.m_iFairLevel,		0,		wstrErrLua, goto LoadFail; );
	//{{ 2010. 04. 01  최육사	비밀던전 헬모드
#ifdef SERV_DAILY_QUEST
	LUA_GET_VALUE_RETURN_ENUM_ERR(	luaManager, L"m_eRepeatType",		kQuestTemplet.m_eRepeatType,	QUEST_REPEAT_TYPE,	QRT_NORMAL,	wstrErrLua, goto LoadFail; );
#else
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_bRepeat",			kQuestTemplet.m_bRepeat,		false,	wstrErrLua, goto LoadFail; );
#endif SERV_DAILY_QUEST
	//}}	
	//{{ 2010. 01. 29  최육사	일일퀘스트
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_bIsPcBang",			kQuestTemplet.m_bIsPcBang,		false,	wstrErrLua, goto LoadFail; );
	//}}

	//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_bIsTimeEvent",		kQuestTemplet.m_bIsTimeEvent,	false,	wstrErrLua, goto LoadFail; );
	//}}

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	if( luaManager.BeginTable( L"m_vecAcceptRewardItem" ) == S_OK )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == S_OK )
		{
			ITEM_DATA itemData;
			LUA_GET_VALUE(	luaManager, L"m_iItemID",	itemData.m_iItemID,		0 );
			LUA_GET_VALUE(	luaManager, L"m_iQuantity",	itemData.m_iQuantity,	0 );

			if( 0 != itemData.m_iItemID  &&  0 != itemData.m_iQuantity )
			{
				kQuestTemplet.m_mapAcceptRewardItem.insert( std::make_pair( itemData.m_iItemID, itemData.m_iQuantity ) );
			}

			index++;
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_wstrMainText",			kQuestTemplet.m_wstrMainText,			L"",	wstrErrLua, goto LoadFail; );
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_wstrThanksText",		kQuestTemplet.m_wstrThanksText,			L"",	wstrErrLua, goto LoadFail; );
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_wstrDissClearText",		kQuestTemplet.m_wstrDissClearText,		L"",	wstrErrLua, goto LoadFail; );

#ifdef SERV_EPIC_QUEST
	LUA_GET_VALUE(		luaManager, L"m_iEpisodeGroupID",	kQuestTemplet.m_iEpisodeGroupID,	0 );
	LUA_GET_VALUE(		luaManager, L"m_iPlayLevel",		kQuestTemplet.m_iPlayLevel,			0 );
	
	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	if( luaManager.BeginTable( L"m_iAfterQuestID" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
				kQuestTemplet.m_vecAfterQuestID.push_back( buf );
			index++;
		}

		luaManager.EndTable();
	}
#else
	LUA_GET_VALUE(		luaManager, L"m_iAfterQuestID",		kQuestTemplet.m_iAfterQuestID,		0 );
#endif SERV_REFORM_QUEST
	//}} 

	LUA_GET_VALUE(		luaManager, L"m_wstrEpisodeGroupTitle",		kQuestTemplet.m_wstrEpisodeGroupTitle,	L"" );
	LUA_GET_VALUE(		luaManager, L"m_wstrStartScene",			kQuestTemplet.m_wstrStartScene,			L"" );
	LUA_GET_VALUE(		luaManager, L"m_wstrEndScene",				kQuestTemplet.m_wstrEndScene,			L"" );
#endif SERV_EPIC_QUEST

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	LUA_GET_VALUE(		luaManager, L"m_iRandomGroupID",	kQuestTemplet.m_iRandomGroupID,		0 );
	LUA_GET_VALUE(		luaManager, L"m_fRandomRate",		kQuestTemplet.m_fRandomRate,		0.f );
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	// 여기는 일반 퀘스트 이므로 무조건 false로 함
	LUA_GET_VALUE(		luaManager, L"m_bIsAccount",	kQuestTemplet.m_bIsAccountQuest,		false );
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	if( LoadQuestCondition( luaManager, kQuestTemplet.m_Condition ) == false )
		goto LoadFail;

	if( luaManager.BeginTable( L"m_vecSubQuest" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
				kQuestTemplet.m_vecSubQuest.push_back( buf );
			index++;
		}

		luaManager.EndTable();
	}

	//{{ dmlee 2008.11.28 서브퀘스트가 없는 경우 예외 처리
	if( true == kQuestTemplet.m_vecSubQuest.empty() )
		goto LoadFail;
	//}} dmlee 2008.11.28 서브퀘스트가 없는 경우 예외 처리

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	if( luaManager.BeginTable( L"m_vecSubQuest_Group" ) == S_OK )
	{
		int index = 1; 
		while( luaManager.BeginTable( index ) == S_OK )
		{
			int i		= 1; 
			int buf		= -1;
			while( luaManager.GetValue( i, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					std::map< int, std::vector<int> >::iterator mit = kQuestTemplet.m_mapSubQuestGroup.find( index );
					if( mit == kQuestTemplet.m_mapSubQuestGroup.end() ) // 기존 그룹이 없다면
					{
						std::vector<int> vecTemp;
						vecTemp.push_back( buf );
						kQuestTemplet.m_mapSubQuestGroup.insert( std::make_pair( index, vecTemp ) );
					}
					else
					{
						BOOST_TEST_FOREACH( int, iSubQuestID, mit->second )
						{
							if( buf == iSubQuestID )
							{
								START_LOG( cerr, L"동일한 서브 퀘스트가 그룹핑 되어 있다." )
									<< BUILD_LOG( kQuestTemplet.m_iID )
									<< BUILD_LOG( iSubQuestID )
									<< BUILD_LOG( buf )
									<< END_LOG;
								goto LoadFail;
							}
						}
						mit->second.push_back( buf );
					}
				}					
				i++;
			}
			luaManager.EndTable();

			++index;
		}
		luaManager.EndTable();
	}
#endif SERV_REFORM_QUEST
	//}}

	if( LoadReward( luaManager, L"Reward", kQuestTemplet.m_Reward ) == false )
		goto LoadFail;

	if( luaManager.BeginTable( "SelectReward" ) == S_OK )
	{
		LUA_GET_VALUE(	luaManager, L"m_iSelectionCount",	kQuestTemplet.m_SelectReward.m_iSelectionCount,		1 );

		if( luaManager.BeginTable( L"m_vecSelectItem" ) == S_OK )
		{
			int index = 1; 
			while( luaManager.BeginTable( index ) == S_OK )
			{
				ITEM_DATA itemData;
				LUA_GET_VALUE(	luaManager, L"m_iItemID",	itemData.m_iItemID,		0 );
				LUA_GET_VALUE(	luaManager, L"m_iQuantity",	itemData.m_iQuantity,	0 );
				LUA_GET_VALUE(	luaManager, L"m_iPeriod",	itemData.m_iPeriod,		0 );
				//{{ 2008. 8. 25  최육사	퀘스트보상 소켓옵션
				//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
				LUA_GET_VALUE(  luaManager, L"m_sSocketOption", itemData.m_iSocketOption1, 0 );
#else
				LUA_GET_VALUE(  luaManager, L"m_sSocketOption", itemData.m_sSocketOption1, 0 );
#endif SERV_ITEM_OPTION_DATA_SIZE
				//}} 
				//}}

				//{{ 2012. 11. 04	박세훈	퀘스트 보상 아이템 강화 레벨 설정
#ifdef SERV_QUEST_REWARD_ITEM_ENCHANT
				{
					short temp;
					LUA_GET_VALUE(	luaManager, L"m_cEnchantLevel",	temp,	0 );
					itemData.m_cEnchantLevel = static_cast<char>( temp );
				}
#endif SERV_QUEST_REWARD_ITEM_ENCHANT
				//}}

				if( 0 != itemData.m_iItemID && 0 != itemData.m_iQuantity )
				{
					kQuestTemplet.m_SelectReward.m_vecSelectItem.push_back( itemData );
				}

				index++;
				luaManager.EndTable();
			}
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}

	// 퀘스트가 숨겨지는 아이템 리스트 얻기
	if( luaManager.BeginTable( "HIDE_QUEST_ITEM" ) == S_OK )
	{
		int index	= 1; 
		int iValue	= -1;
		while( luaManager.GetValue( index, iValue ) == S_OK )
		{
			if( iValue > 0 )
			{
				kQuestTemplet.m_setHideItemID.insert( iValue );
			}

			index++;
		}

		luaManager.EndTable();
	}

	if( m_mapQuestTemplet.find( kQuestTemplet.m_iID ) != m_mapQuestTemplet.end() )
		goto LoadFail;

	m_mapQuestTemplet.insert( std::make_pair( kQuestTemplet.m_iID, kQuestTemplet ) );

#ifdef SERV_EPIC_QUEST
	if( kQuestTemplet.m_eQuestType == QT_EPIC )
	{
		//# 에픽퀘스트 리스트 제공을 위해 담아 놓는다.
		std::map< int, QuestTemplet >::iterator mit;
		mit = m_mapQuestTemplet.find(kQuestTemplet.m_iID);

		if( mit != m_mapQuestTemplet.end() )
		{
			std::vector<QuestTemplet*>::iterator vit = m_vecEpicQuestTemplet.begin();

			//# 실시간 로딩시에 지장이 없기 위해서 있는 것은 지우고 새로 넣어줌
			while( vit != m_vecEpicQuestTemplet.end() )
			{
				if( kQuestTemplet.m_iID == (*vit)->m_iID )
				{
					m_vecEpicQuestTemplet.erase(vit);
					break;
				}
				++vit;
			}

			m_vecEpicQuestTemplet.push_back(&(mit->second));
		}
		else
		{
			START_LOG( cerr, L"[에픽관련] 방금넣은 에픽 데이터가 없음." )
				<< BUILD_LOG( kQuestTemplet.m_iID )
				<< END_LOG;
		}
	}
#endif SERV_EPIC_QUEST
	//{{ 2011. 07. 09	최육사	에픽 퀘스트 체크
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	if( kQuestTemplet.m_eQuestType == QT_EPIC )
	{
		//# 에픽퀘스트 리스트 제공을 위해 담아 놓는다.
		std::map< int, QuestTemplet >::iterator mit;
		mit = m_mapQuestTemplet.find(kQuestTemplet.m_iID);

		if( mit != m_mapQuestTemplet.end() )
		{
			std::vector<QuestTemplet*>::iterator vit = m_vecExistBeforeEpicQuestTemplet.begin();

			//# 실시간 로딩시에 지장이 없기 위해서 있는 것은 지우고 새로 넣어줌
			while( vit != m_vecExistBeforeEpicQuestTemplet.end() )
			{
				if( kQuestTemplet.m_iID == (*vit)->m_iID )
				{
					m_vecExistBeforeEpicQuestTemplet.erase(vit);
					break;
				}
				++vit;
			}

			if( kQuestTemplet.m_Condition.m_vecBeforeQuestID.empty() == false )
			{
				bool bIsExistBeforeQuest = false;
				BOOST_TEST_FOREACH( const int, iBeforeQuestID, kQuestTemplet.m_Condition.m_vecBeforeQuestID )
				{
					if( iBeforeQuestID != 0 )
					{
						bIsExistBeforeQuest = true;
					}
				}

				if( bIsExistBeforeQuest == true )
				{
					m_vecExistBeforeEpicQuestTemplet.push_back(&(mit->second));
				}
			}
		}
		else
		{
			START_LOG( cerr, L"[에픽관련] 방금넣은 에픽 데이터가 없음." )
				<< BUILD_LOG( kQuestTemplet.m_iID )
				<< END_LOG;
		}
	}
#endif SERV_EPIC_QUEST_CHECK_UPDATE
	//}}
	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	if( kQuestTemplet.m_iRandomGroupID > 0 )
	{
		// 이미 Random GroupID 데이터가 있다. 거기에 추가하자
		std::map< int, std::vector< int > >::iterator mitRandom = m_mapRandomQuest.find( kQuestTemplet.m_iRandomGroupID );
		if( mitRandom != m_mapRandomQuest.end() )
		{
			// QuestID 중복 체크
			std::vector< int >::iterator vitRandom = mitRandom->second.begin();
			for( ; vitRandom != mitRandom->second.end() ; ++vitRandom )
			{
				if( (*vitRandom) == kQuestTemplet.m_iID )
				{
					START_LOG( cerr, L"중복되는 랜덤 퀘스트가 있다." )
						<< BUILD_LOG( kQuestTemplet.m_iID )
						<< BUILD_LOG( kQuestTemplet.m_iRandomGroupID )
						<< END_LOG;
					break;
				}
			}
			// 중복 되는 것이 없으니 추가하자
			mitRandom->second.push_back( kQuestTemplet.m_iID );
		}
		// 새로운 Random GroupID 이다
		else
		{
			std::vector< int > vecRandomQuest;
			vecRandomQuest.push_back( kQuestTemplet.m_iID );
			m_mapRandomQuest.insert( std::make_pair( kQuestTemplet.m_iRandomGroupID, vecRandomQuest ) );
		}
	}
#endif SERV_RANDOM_DAY_QUEST
	//}}

	if( QT_NORMAL <= kQuestTemplet.m_eQuestType 
		&& kQuestTemplet.m_eQuestType < QT_END  )
	{
		m_mapQuestTempletByType[kQuestTemplet.m_eQuestType].insert( std::make_pair(kQuestTemplet.m_iID, kQuestTemplet) );
	}
	else
	{
		START_LOG( cerr, L"정의 되지 않은 퀘스트 타입입니다." )
			<< BUILD_LOG( kQuestTemplet.m_iID )
			<< BUILD_LOG( static_cast<int>(kQuestTemplet.m_eQuestType) )
			<< END_LOG;
	}

	return true;

LoadFail:
	START_LOG( cerr, L"::: Quest Templet Load Fail :::" )		
		<< BUILD_LOG( kQuestTemplet.m_iID )
		<< BUILD_LOG( kQuestTemplet.m_wstrTitle )
		<< BUILD_LOG( wstrErrLua )
		<< END_LOG;
	
	return false;
}

bool CXSLQuestManager::AddSubQuestTemplet_LUA()
{
	SubQuestTemplet kSubQuestTemplet;
	KLuaManager luaManager( GetLuaState() );

	LUA_GET_VALUE_RETURN(		luaManager, L"m_iID",					kSubQuestTemplet.m_iID,				0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_wstrDescription",		kSubQuestTemplet.m_wstrDescription,	L"",	goto LoadFail; );

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eClearType",			kSubQuestTemplet.m_eClearType,	SUB_QUEST_TYPE,	SQT_NONE,	goto LoadFail; );

	LUA_GET_VALUE_RETURN(		luaManager, L"m_bAutomaticDescription",	kSubQuestTemplet.m_bAutomaticDescription,	true,		goto LoadFail; );


	if( LoadClearCondition( luaManager, kSubQuestTemplet ) == false )
		goto LoadFail;

	m_mapSubQuestTemplet.insert( std::make_pair( kSubQuestTemplet.m_iID, kSubQuestTemplet ) );

	return true;

LoadFail:
	START_LOG( cerr, L"::: Sub Quest Templet Load Fail :::" )
		<< BUILD_LOG( kSubQuestTemplet.m_iID )
		<< BUILD_LOG( kSubQuestTemplet.m_wstrDescription )
		<< BUILD_LOG( kSubQuestTemplet.m_bAutomaticDescription )
		<< END_LOG;		
	
	return false;
}


const CXSLQuestManager::QuestTemplet*	CXSLQuestManager::GetQuestTemplet( int iQuestID )
{
	std::map< int, QuestTemplet >::const_iterator mit = m_mapQuestTemplet.find( iQuestID );
	if( mit != m_mapQuestTemplet.end() )
		return &mit->second;

	return NULL;		
}

const CXSLQuestManager::SubQuestTemplet* CXSLQuestManager::GetSubQuestTemplet( int iSubQuestID )
{
	std::map< int, SubQuestTemplet >::const_iterator mit = m_mapSubQuestTemplet.find( iSubQuestID );
	if( mit != m_mapSubQuestTemplet.end() )
		return &mit->second;

	return NULL;		
}

bool CXSLQuestManager::GetQuestConditionItemList( int iQuestID, std::map< int, int >& mapItem )
{
	mapItem.clear();

	const QuestTemplet* pQuestTemplet = GetQuestTemplet( iQuestID );

	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"QuestTemplet 포인터 이상.!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;

		return false; 
	}

	std::map< int, SubQuestTemplet >::iterator mit;
	int iSubQuestID = 0;
	for( int i = 0; i < (int)pQuestTemplet->m_vecSubQuest.size(); ++i )
	{
		iSubQuestID = pQuestTemplet->m_vecSubQuest[i];
		mit = m_mapSubQuestTemplet.find( iSubQuestID );

		if( mit != m_mapSubQuestTemplet.end() )
		{
			if( mit->second.m_eClearType == SQT_ITEM_COLLECTION ||
				mit->second.m_eClearType == SQT_QUEST_ITEM_COLLECTION )
			{
				mapItem.insert( std::make_pair( mit->second.m_ClearCondition.m_iItemID, mit->second.m_ClearCondition.m_iCollectionItemNum ) );
			}
		}
		else
		{
			START_LOG( cerr, L"SubQuestTemplet 포인터 이상.!" )
				<< BUILD_LOG( iSubQuestID )
				<< END_LOG;
		}
	}	

	return true;
}

bool CXSLQuestManager::IsSubItemCondition( int iQuestID )
{
	const QuestTemplet* pQuestTemplet = GetQuestTemplet( iQuestID );

	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"QuestTemplet 포인터 이상.!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;

		return false; 
	}

	std::map< int, SubQuestTemplet >::iterator mit;
	int iSubQuestID = 0;
	for( int i = 0; i < (int)pQuestTemplet->m_vecSubQuest.size(); ++i )
	{
		iSubQuestID = pQuestTemplet->m_vecSubQuest[i];
		mit = m_mapSubQuestTemplet.find( iSubQuestID );

		if( mit != m_mapSubQuestTemplet.end() )
		{
			if( mit->second.m_eClearType == SQT_ITEM_COLLECTION  ||
				mit->second.m_eClearType == SQT_QUEST_ITEM_COLLECTION )
			{
				return true;
			}
		}
		else
		{
			START_LOG( cerr, L"SubQuestTemplet 포인터 이상.!" )
				<< BUILD_LOG( iSubQuestID )
				<< END_LOG;
		}
	}	

	return false;
}

#ifdef SERV_EPIC_QUEST
bool CXSLQuestManager::IsEpicQuest( IN int iQuestID_ )
{
	const QuestTemplet* pQuestTemplet = GetQuestTemplet( iQuestID_ );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"QuestTemplet 포인터 이상.!" )
			<< BUILD_LOG( iQuestID_ )
			<< END_LOG;

		return false;
	}

	if( pQuestTemplet->m_eQuestType != QT_EPIC )
		return false;

	return true;
}

void CXSLQuestManager::GetEpicQuestTempletList( OUT std::vector<QuestTemplet*> &vecEpicQuestTemplet )
{
	vecEpicQuestTemplet = m_vecEpicQuestTemplet;
	//{{AFX
	//std::map< int, QuestTemplet >::iterator mit;

	//std::vector<int>::iterator vit = m_vecEpicID.begin();

	//// 에픽퀘스트를 다 검사하여 BeforeQuestID에 iClearID를 가지는 에픽을 찾아냄.
	//while( vit != m_vecEpicID.end() )
	//{
	//	mit = m_mapQuestTemplet.find(*vit);

	//	if( mit != m_mapQuestTemplet.end() )
	//	{
	//		QuestTemplet *pQuestTemplet = &(mit->second);

	//		if( NULL != pQuestTemplet && QT_EPIC == pQuestTemplet->m_eQuestType )
	//		{
	//			vecEpicQuestTemplet.push_back(&(mit->second));
	//		}
	//		else
	//		{
	//			START_LOG( cerr, L"[에픽관련] 타입이상." )
	//				<< BUILD_LOG( *vit )
	//				<< END_LOG;
	//		}
	//	}
	//	else
	//	{
	//		START_LOG( cerr, L"[에픽관련] 에픽아이디가 이상함." )
	//			<< BUILD_LOG( *vit )
	//			<< END_LOG;
	//	}

	//	++vit;
	//}
	//}}AFX
}
#endif SERV_EPIC_QUEST

//{{ 2011. 07. 09	최육사	에픽 퀘스트 체크
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
void CXSLQuestManager::GetExistBeforeEpicQuestTempletList( OUT std::vector<QuestTemplet*> &vecEpicQuestTemplet )
{
	vecEpicQuestTemplet = m_vecExistBeforeEpicQuestTemplet;
}
#endif SERV_EPIC_QUEST_CHECK_UPDATE
//}}

bool CXSLQuestManager::LoadQuestCondition( KLuaManager& luaManager, QuestCondition& questCondition )
{
	if( luaManager.BeginTable( L"QuestCondition" ) == S_OK )
	{
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		if( luaManager.BeginTable( L"m_iEnableVillage" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					questCondition.m_setEnableVillage.insert( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}
#else
	#ifdef SERV_EPIC_QUEST
		LUA_GET_VALUE( luaManager, L"m_iEnableVillage",	questCondition.m_iEnableVillage, 0 );
	#endif SERV_EPIC_QUEST
#endif SERV_REFORM_QUEST
		//}}

		LUA_GET_VALUE_RETURN(		luaManager,	L"m_iLevel",			questCondition.m_iLevel,			-1, return false; );
		LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eUnitType",			questCondition.m_eUnitType,			CXSLUnit::UNIT_TYPE,	CXSLUnit::UT_NONE, return false; );

		//////////////////////////////////////////////////////////////////////////		

		if( luaManager.BeginTable( L"m_eUnitClass" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( static_cast<CXSLUnit::UNIT_CLASS>(buf) > CXSLUnit::UC_NONE )
				{
					questCondition.m_setUnitClass.insert( static_cast<CXSLUnit::UNIT_CLASS>(buf) );
				}

				index++;
			}

			luaManager.EndTable();
		}

		//////////////////////////////////////////////////////////////////////////

		//LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eUnitClass",		questCondition.m_eUnitClass,		CXSLUnit::UNIT_CLASS,	CXSLUnit::UC_NONE, return false; );

		LUA_GET_VALUE_RETURN(		luaManager, L"m_iConditionItemID",	questCondition.m_iConditionItemID,	-1, return false; );
		LUA_GET_VALUE_RETURN(		luaManager, L"m_iConditionItemNum",	questCondition.m_iConditionItemNum,	-1, return false; );
		LUA_GET_VALUE(				luaManager, L"m_iNotBeforeQuestID",	questCondition.m_iNotBeforeQuestID,	-1 );

		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		LUA_GET_VALUE( luaManager, L"m_iLimitLevel",	questCondition.m_iLimitLevel,	-1 );
		if( luaManager.BeginTable( L"m_iEnableDungeon" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					questCondition.m_setEnableDungeon.insert( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}
		if( luaManager.BeginTable( L"m_iEnableBattleField" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					questCondition.m_setEnableBattleField.insert( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}
#endif SERV_REFORM_QUEST
		//}} 

		//if( questCondition.m_eUnitType != CXSLUnit::UT_NONE && questCondition.m_eUnitClass != CXSLUnit::UC_NONE )
		if( questCondition.m_eUnitType != CXSLUnit::UT_NONE && questCondition.m_setUnitClass.empty() == false )
		{
			START_LOG( cerr, L"Unit 식별값이 잘못되었음.!" )
				<< BUILD_LOG( questCondition.m_eUnitType )
				//<< BUILD_LOG( questCondition.m_eUnitClass )
				<< END_LOG;
		}

		if( luaManager.BeginTable( L"m_vecBeforeQuestID" ) == S_OK )
		{
			int index	= 1; 
			int buf		= -1;
			while( luaManager.GetValue( index, buf ) == S_OK )
			{
				if( buf >= 0 )
				{
					questCondition.m_vecBeforeQuestID.push_back( buf );
				}

				index++;
			}

			luaManager.EndTable();
		}

		luaManager.EndTable();
	}
	return true;
}

bool CXSLQuestManager::LoadReward( KLuaManager& luaManager, const WCHAR* pTableName, Reward& reward )
{
	if( luaManager.BeginTable( pTableName ) == S_OK )
	{
		LUA_GET_VALUE_RETURN(	luaManager, L"m_bEnable",		reward.m_bEnable,		true,	return false; );
		LUA_GET_VALUE(		luaManager, L"m_iEXP",			reward.m_iEXP,			0 );
		LUA_GET_VALUE(		luaManager, L"m_iED",			reward.m_iED,			0 );
		LUA_GET_VALUE(		luaManager, L"m_wstrName",		reward.m_wstrName,		L"" );
		LUA_GET_VALUE(		luaManager, L"m_iBuff",			reward.m_iBuff,			0 );
		LUA_GET_VALUE(		luaManager, L"m_iSP",			reward.m_iSP,			0 );
		LUA_GET_VALUE_ENUM( luaManager, L"m_eChangeUnitClass",	reward.m_eChangeUnitClass, CXSLUnit::UNIT_CLASS, CXSLUnit::UC_NONE );


		if( luaManager.BeginTable( L"m_vecItem" ) == S_OK )
		{
			int index = 1; 
			while( luaManager.BeginTable( index ) == S_OK )
			{
				ITEM_DATA itemData;
				LUA_GET_VALUE(	luaManager, L"m_iItemID",	itemData.m_iItemID,		0 );
				LUA_GET_VALUE(	luaManager, L"m_iQuantity",	itemData.m_iQuantity,	0 );
				LUA_GET_VALUE(	luaManager, L"m_iPeriod",	itemData.m_iPeriod,		0 );
				//{{ 2008. 8. 25  최육사	퀘스트보상 소켓옵션
				//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
				LUA_GET_VALUE(  luaManager, L"m_sSocketOption", itemData.m_iSocketOption1, 0 );
#else
				LUA_GET_VALUE(  luaManager, L"m_sSocketOption", itemData.m_sSocketOption1, 0 );
#endif SERV_ITEM_OPTION_DATA_SIZE
				//}} 
				//}}

				if( 0 != itemData.m_iItemID && 0 != itemData.m_iQuantity )
				{
					reward.m_vecItem.push_back( itemData );
				}

				index++;
				luaManager.EndTable();
			}
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}
	return true;
}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
bool CXSLQuestManager::LoadClearCondition( KLuaManager& luaManager, SubQuestTemplet& kSubQuestTemplet )
{
	if( luaManager.BeginTable( L"m_ClearCondition" ) == S_OK )
	{
		switch( kSubQuestTemplet.m_eClearType )
		{
		case SQT_NPC_TALK:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eTalkNPCID",		kSubQuestTemplet.m_ClearCondition.m_eTalkNPCID,			CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE, goto error_proc; );

				if( luaManager.BeginTable( L"m_eVillageID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setVillageMapID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
				// 없어도 에러 아님
			}
			break;

		case SQT_NPC_HUNT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false, goto error_proc; );

				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
				// 없어도 에러 아님

				if( luaManager.BeginTable( L"m_eFieldID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
				// 없어도 에러 아님

				if( luaManager.BeginTable( L"m_eKillNPCID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setKillNPCID.insert( static_cast<CXSLUnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iKillNum",			kSubQuestTemplet.m_ClearCondition.m_iKillNum,			0, goto error_proc; );
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID )
			}
			break;

		case SQT_ITEM_COLLECTION:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemNum",kSubQuestTemplet.m_ClearCondition.m_iCollectionItemNum,	0,		goto error_proc; );
			}
			break;

		case SQT_QUEST_ITEM_COLLECTION:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );

				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				} 
				// 없어도 에러 아님

				if( luaManager.BeginTable( L"m_eFieldID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
				// 없어도 에러 아님

				if( luaManager.BeginTable( L"m_eKillNPCID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setKillNPCID.insert( static_cast<CXSLUnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemNum",kSubQuestTemplet.m_ClearCondition.m_iCollectionItemNum,	0,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_fQuestItemDropRate",kSubQuestTemplet.m_ClearCondition.m_fQuestItemDropRate,	0.0f,	goto error_proc; );
			}
			break;

		case SQT_DUNGEON_TIME:
			{
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
				// 없어도 에러 아님

				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearTime",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearTime,	0,		goto error_proc; );

			}
			break;

		case SQT_DUNGEON_RANK:
			{
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
				// 없어도 에러 아님
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(	luaManager, L"m_iUseTitleID",		kSubQuestTemplet.m_ClearCondition.m_iUseTitleID,			-1);
#endif SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonRank",		kSubQuestTemplet.m_ClearCondition.m_eDungeonRank,		CXSLDungeon::RANK_TYPE,		CXSLDungeon::RT_NONE, goto error_proc; );

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
				LUA_GET_VALUE(				luaManager, L"m_iDungeonClearCount",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearCount,			1 );
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
			}
			break;

		case SQT_DUNGEON_DAMAGE:
			{
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}
					// 없어도 에러 아님

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonDamage",	kSubQuestTemplet.m_ClearCondition.m_iDungeonDamage,		-1,		goto error_proc; );
			}
			break;

		case SQT_DUNGEON_CLEAR_COUNT:
			//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
		case SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			//}}
			{
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
				// 없어도 에러 아님
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(	luaManager, L"m_iUseTitleID",		kSubQuestTemplet.m_ClearCondition.m_iUseTitleID,			-1);
				LUA_GET_VALUE_ENUM(	luaManager, L"m_eDungeonRank",		kSubQuestTemplet.m_ClearCondition.m_eDungeonRank,		CXSLDungeon::RANK_TYPE,		CXSLDungeon::RT_NONE);
#endif SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
			}
			break;

		case SQT_PVP_PLAY:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPPlay",	kSubQuestTemplet.m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PCC_NONE, goto error_proc; );
			}
			break;

		case SQT_PVP_WIN:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPWin",	kSubQuestTemplet.m_ClearCondition.m_iPVPWin,		-1, goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PCC_NONE, goto error_proc; );
			}
			break;

		case SQT_PVP_KILL:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPKill",	kSubQuestTemplet.m_ClearCondition.m_iPVPKill,		-1, goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PCC_NONE, goto error_proc; );
			}
			break;

		case SQT_WITH_DIF_SERV_USER:
			{
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
			}
			break;

		case SQT_ITEM_ENCHANT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iEnchantLevel",		kSubQuestTemplet.m_ClearCondition.m_iEnchantLevel,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0 );
			}
			break;

		case SQT_ITEM_SOCKET:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iSocketCount",		kSubQuestTemplet.m_ClearCondition.m_iSocketCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0 );
			}
			break;

		case SQT_ITEM_ATTRIB:
			{
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eAttribCountType",	kSubQuestTemplet.m_ClearCondition.m_eAttribCountType,	CXSLAttribEnchantItem::ATTRIB_COUNT_TYPE,	CXSLAttribEnchantItem::ACT_NONE );
				int iAttribEnchant = 0;
				LUA_GET_VALUE(				luaManager, L"m_eAttribEnchant0",	iAttribEnchant,											0 );
				kSubQuestTemplet.m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant0 = static_cast<char>(iAttribEnchant);
				LUA_GET_VALUE(				luaManager, L"m_eAttribEnchant1",	iAttribEnchant,											0 );
				kSubQuestTemplet.m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant1 = static_cast<char>(iAttribEnchant);
				LUA_GET_VALUE(				luaManager, L"m_eAttribEnchant2",	iAttribEnchant,											0 );
				kSubQuestTemplet.m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant2 = static_cast<char>(iAttribEnchant);

				LUA_GET_VALUE(				luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0 );
			}
			break;

		case SQT_ITEM_RESOLVE:
			{				
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iResolveCount",		kSubQuestTemplet.m_ClearCondition.m_iResolveCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0 );
			}
			break;

		case SQT_ITEM_EQUIP_DUNGEON_CLEAR:
			{
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0,		goto error_proc; );
			}
			break;

		case SQT_USE_SKILL_POINT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iSpUseCount",		kSubQuestTemplet.m_ClearCondition.m_iSpUseCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_iSkillID",			kSubQuestTemplet.m_ClearCondition.m_iSkillID,			0 );
			}
			break;

		case SQT_FEED_PET:
			{
				// 1. 아이템 등급
				// 2. 먹이 주는 횟수
			}
			break;

		case SQT_USER_DIE:
			{
				// 1. 죽는 타입
				// 2. 죽는 횟수
			}
			break;

		case SQT_PVP_NPC_HUNT:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPWin",	kSubQuestTemplet.m_ClearCondition.m_iPVPWin,		-1, goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PCC_NONE, goto error_proc; );
			}
			break;

		case SQT_VISIT_VILLAGE:
			{
				if( luaManager.BeginTable( L"m_eVillageID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setVillageMapID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}

				if( kSubQuestTemplet.m_ClearCondition.m_setVillageMapID.size() <= 0 )
				{
					START_LOG( cerr ,L"스크립트 파싱 실패 - SQT_VISIT_VILLAGE" )
						<< BUILD_LOG( static_cast<int>(kSubQuestTemplet.m_ClearCondition.m_setVillageMapID.size()) )
						<< END_LOG;

					goto error_proc;
				}
			}
			break;

		case SQT_VISIT_FIELD:
			{
				if( luaManager.BeginTable( L"m_eFieldID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
				// 없으면 에러
				if( kSubQuestTemplet.m_ClearCondition.m_setBattleFieldID.size() <= 0 )
				{
					START_LOG( cerr ,L"스크립트 파싱 실패 - SQT_VISIT_FIELD" )
						<< BUILD_LOG( static_cast<int>(kSubQuestTemplet.m_ClearCondition.m_setBattleFieldID.size()) )
						<< END_LOG;

					goto error_proc;
				}
				//없어도 에러아님
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(	luaManager, L"m_iUseTitleID",		kSubQuestTemplet.m_ClearCondition.m_iUseTitleID,			-1);
#endif SERV_EVENT_TITLE_SUBQUEST
			}
			break;

		case SQT_VISIT_DUNGEON:
			{
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
				// 없으면 에러
				if( kSubQuestTemplet.m_ClearCondition.m_setDungeonID.size() <= 0 )
				{
					START_LOG( cerr ,L"스크립트 파싱 실패 - SQT_VISIT_DUNGEON" )
						<< BUILD_LOG( static_cast<int>(kSubQuestTemplet.m_ClearCondition.m_setDungeonID.size()) )
						<< END_LOG;

					goto error_proc;
				}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
			}
			break;

		case SQT_FIND_NPC:
			{
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
				
				if( luaManager.BeginTable( L"m_eFieldID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setBattleFieldID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_iFindNPCID",	kSubQuestTemplet.m_ClearCondition.m_iFindNPCID, CXSLUnitManager::NPC_UNIT_ID, CXSLUnitManager::NUI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
			}
			break;

		case SQT_PVP_PLAY_ARRANGE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPPlay",	kSubQuestTemplet.m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
			}
			break;

			//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		case SQT_CHAR_LEVEL_UP:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCharLevel",	kSubQuestTemplet.m_ClearCondition.m_iCharLevel,		9999, goto error_proc; );
			}
			break;
#endif SERV_ACCOUNT_MISSION_SYSTEM
			//}}

#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND // 2013.02.07 lygan_조성욱 // 인도네시아 대전 이벤트 때문에 에픽 NPC에 대한 퀘스트 조건이 늘어 났음 기존에는 문제 있으면 에러를 찍지만 이젠 -1 값만 들어가게 되니 조심
		case SQT_PVP_HERO_NPC_KILL:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PVP_GAME_TYPE::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPKill",	kSubQuestTemplet.m_ClearCondition.m_iPVPKill,		-1, goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PVP_CHANNEL_CLASS::PCC_NONE, goto error_proc; );

			}
			break;
		case SQT_PVP_HERO_NPC_PLAY:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PVP_GAME_TYPE::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPPlay",	kSubQuestTemplet.m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PVP_CHANNEL_CLASS::PCC_NONE, goto error_proc; );

			}
			break;
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND
			//{{ 2012. 01. 20    박진웅		스킬 사용 서브퀘스트
#ifdef SERV_SKILL_USE_SUBQUEST
		case SQT_SKILL_USE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iUseSkillCount",	kSubQuestTemplet.m_ClearCondition.m_iUseSkillCount,		-1, goto error_proc; );

				if( luaManager.BeginTable( L"m_vecSkillID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setSkillID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
			}
			break;
#endif SERV_SKILL_USE_SUBQUEST
#ifdef SERV_SUB_QUEST_USE_ITEM
		case SQT_ITEM_USE:
			{
#ifdef SERV_SUB_QUEST_USE_ITEM_BUG_FIX
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
#else //SERV_SUB_QUEST_USE_ITEM_BUG_FIX
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
#endif //SERV_SUB_QUEST_USE_ITEM_BUG_FIX

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				if( luaManager.BeginTable( L"m_listUseItemID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_vecUseItemID.push_back(buf);
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN( luaManager, L"m_iUseItemCnt",kSubQuestTemplet.m_ClearCondition.m_iUseItemNum,	0,	goto error_proc; );
			}
			break;
#endif SERV_SUB_QUEST_USE_ITEM
			//}}
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
		case SQT_LEARN_NEW_SKILL:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iLearnNewSkillCount",	kSubQuestTemplet.m_ClearCondition.m_iLearnNewSkillCount,		-1, goto error_proc; );

				if( luaManager.BeginTable( L"m_vecSkillID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setSkillID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
			}
			break;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
		default:
			{
				START_LOG( cerr, L"Sub Quest ClearCondition Type 별 읽어 오기 실패.!" )
					<< BUILD_LOG( kSubQuestTemplet.m_iID )
					<< BUILD_LOG( kSubQuestTemplet.m_wstrDescription )
					<< BUILD_LOG( kSubQuestTemplet.m_eClearType )
					<< END_LOG;

				luaManager.EndTable();
				return false;
			}
		}


		luaManager.EndTable();
		return true;
	}

error_proc:

	START_LOG( cerr, L"Sub Quest ClearCondition 읽어 오기 실패.!" )
		<< BUILD_LOG( kSubQuestTemplet.m_iID )
		<< BUILD_LOG( kSubQuestTemplet.m_wstrDescription )
		<< BUILD_LOG( kSubQuestTemplet.m_eClearType )
		<< END_LOG;

	luaManager.EndTable();
	return false;
}

#else

bool CXSLQuestManager::LoadClearCondition( KLuaManager& luaManager, SubQuestTemplet& kSubQuestTemplet )
{
	int iDifficulty;

	if( luaManager.BeginTable( L"m_ClearCondition" ) == S_OK )
	{
		switch( kSubQuestTemplet.m_eClearType )
		{
		case SQT_NPC_TALK:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eTalkNPCID",		kSubQuestTemplet.m_ClearCondition.m_eTalkNPCID,			CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE, goto error_proc; );
				//LUA_GET_VALUE_RETURN(		luaManager, L"m_bTalkNPC",			pSubQuestTemplet->m_ClearCondition.m_bTalkNPC,			false, goto error_proc; );
			}
			break;

		case SQT_NPC_HUNT:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false, goto error_proc; );
				//}}
				//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
				LUA_GET_VALUE(				luaManager, L"m_eVillageID",		kSubQuestTemplet.m_ClearCondition.m_iVillageMapID,			0 );
				LUA_GET_VALUE(				luaManager, L"m_eFieldID",			kSubQuestTemplet.m_ClearCondition.m_iBattleFieldID,			0 ); 
#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}

				if( luaManager.BeginTable( L"m_eKillNPCID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setKillNPCID.insert( static_cast<CXSLUnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iKillNum",			kSubQuestTemplet.m_ClearCondition.m_iKillNum,			0, goto error_proc; );
			}
			break;

		case SQT_ITEM_COLLECTION:
			{
				//LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		pSubQuestTemplet->m_ClearCondition.m_eDungeonID,	SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				//LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				//pSubQuestTemplet->m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				//LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eKillNPCID",		pSubQuestTemplet->m_ClearCondition.m_eKillNPCID,	CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemNum",kSubQuestTemplet.m_ClearCondition.m_iCollectionItemNum,	0,		goto error_proc; );
			}
			break;

		case SQT_QUEST_ITEM_COLLECTION:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty ); 

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}
				//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
				LUA_GET_VALUE(				luaManager, L"m_eVillageID",		kSubQuestTemplet.m_ClearCondition.m_iVillageMapID,			0 );
				LUA_GET_VALUE(				luaManager, L"m_eFieldID",			kSubQuestTemplet.m_ClearCondition.m_iBattleFieldID,			0 );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}

				if( luaManager.BeginTable( L"m_eKillNPCID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setKillNPCID.insert( static_cast<CXSLUnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemNum",kSubQuestTemplet.m_ClearCondition.m_iCollectionItemNum,	0,		goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_fQuestItemDropRate",kSubQuestTemplet.m_ClearCondition.m_fQuestItemDropRate,	0.0f,	goto error_proc; );
			}
			break;

		case SQT_DUNGEON_TIME:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2011. 08. 30  김민성	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
#endif SERV_SECRET_HELL
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearTime",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearTime,	0,		goto error_proc; );

			}
			break;

		case SQT_DUNGEON_RANK:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2011. 08. 30  김민성	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
#endif SERV_SECRET_HELL
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(	luaManager, L"m_iUseTitleID",		kSubQuestTemplet.m_ClearCondition.m_iUseTitleID,			-1);
#endif SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonRank",		kSubQuestTemplet.m_ClearCondition.m_eDungeonRank,		CXSLDungeon::RANK_TYPE,		CXSLDungeon::RT_NONE, goto error_proc; );

			}
			break;

		case SQT_DUNGEON_DAMAGE:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonDamage",	kSubQuestTemplet.m_ClearCondition.m_iDungeonDamage,		-1,		goto error_proc; );
			}
			break;

		case SQT_DUNGEON_CLEAR_COUNT:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );	

				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
#endif SERV_SECRET_HELL
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(				luaManager, L"m_iUseTitleID",		kSubQuestTemplet.m_ClearCondition.m_iUseTitleID,			-1);
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonRank",		kSubQuestTemplet.m_ClearCondition.m_eDungeonRank,		CXSLDungeon::RANK_TYPE,		CXSLDungeon::RT_NONE);
#endif SERV_EVENT_TITLE_SUBQUEST
			}
			break;

		case SQT_PVP_PLAY:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PVP_GAME_TYPE::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPPlay",	kSubQuestTemplet.m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
				//{{ 2012. 09. 18   김민성   대전 플레이 퀘스트 조건 변경
#ifdef SERV_PVP_PLAY_QUEST
				LUA_GET_VALUE_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PVP_CHANNEL_CLASS::PCC_NONE );
#endif SERV_PVP_PLAY_QUEST
				//}}
			}
			break;

		case SQT_PVP_WIN:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PVP_GAME_TYPE::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPWin",	kSubQuestTemplet.m_ClearCondition.m_iPVPWin,		-1, goto error_proc; );
				//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PVP_CHANNEL_CLASS::PCC_NONE, goto error_proc; );
#endif SERV_NEW_PVP_QUEST
				//}} 
			}
			break;

		case SQT_PVP_KILL:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PVP_GAME_TYPE::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPKill",	kSubQuestTemplet.m_ClearCondition.m_iPVPKill,		-1, goto error_proc; );
				//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PVP_CHANNEL_CLASS::PCC_NONE, goto error_proc; );
#endif SERV_NEW_PVP_QUEST
				//}} 
			}
			break;

			//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		case SQT_WITH_DIF_SERV_USER:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
#endif SERV_SECRET_HELL
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
			}
			break;
#endif SERV_INTEGRATION
			//}}

			//{{ 2010. 10. 25	최육사	퀘스트 조건 추가
			//////////////////////////////////////////////////////////////////////////
#ifdef SERV_QUEST_CLEAR_EXPAND
		case SQT_ITEM_ENCHANT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iEnchantLevel",		kSubQuestTemplet.m_ClearCondition.m_iEnchantLevel,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0 );
			}
			break;

		case SQT_ITEM_SOCKET:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iSocketCount",		kSubQuestTemplet.m_ClearCondition.m_iSocketCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0 );
			}
			break;

		case SQT_ITEM_ATTRIB:
			{
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eAttribCountType",	kSubQuestTemplet.m_ClearCondition.m_eAttribCountType,	CXSLAttribEnchantItem::ATTRIB_COUNT_TYPE,	CXSLAttribEnchantItem::ACT_NONE );
				int iAttribEnchant = 0;
				LUA_GET_VALUE(				luaManager, L"m_eAttribEnchant0",	iAttribEnchant,											0 );
				kSubQuestTemplet.m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant0 = static_cast<char>(iAttribEnchant);
				LUA_GET_VALUE(				luaManager, L"m_eAttribEnchant1",	iAttribEnchant,											0 );
				kSubQuestTemplet.m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant1 = static_cast<char>(iAttribEnchant);
				LUA_GET_VALUE(				luaManager, L"m_eAttribEnchant2",	iAttribEnchant,											0 );
				kSubQuestTemplet.m_ClearCondition.m_kAttribEnchantInfo.m_cAttribEnchant2 = static_cast<char>(iAttribEnchant);

				LUA_GET_VALUE(				luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0 );
			}
			break;

		case SQT_ITEM_RESOLVE:
			{				
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iResolveCount",		kSubQuestTemplet.m_ClearCondition.m_iResolveCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0 );
			}
			break;

		case SQT_ITEM_EQUIP_DUNGEON_CLEAR:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2010. 05. 01  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
				LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonMode",		kSubQuestTemplet.m_ClearCondition.m_eDungeonMode,		CXSLDungeon::DUNGEON_MODE,	CXSLDungeon::DM_INVALID );				
#endif SERV_SECRET_HELL
				//}}
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				LUA_GET_VALUE_RETURN(		luaManager, L"m_iDungeonClearCount",	kSubQuestTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iCollectionItemID",	kSubQuestTemplet.m_ClearCondition.m_iItemID,			0,		goto error_proc; );
			}
			break;

		case SQT_USE_SKILL_POINT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iSpUseCount",		kSubQuestTemplet.m_ClearCondition.m_iSpUseCount,		0,		goto error_proc; );
				LUA_GET_VALUE(				luaManager, L"m_iSkillID",			kSubQuestTemplet.m_ClearCondition.m_iSkillID,			0 );
			}
			break;

		case SQT_FEED_PET:
			{
				// 1. 아이템 등급
				// 2. 먹이 주는 횟수
			}
			break;

		case SQT_USER_DIE:
			{
				// 1. 죽는 타입
				// 2. 죽는 횟수
			}
			break;
#endif SERV_QUEST_CLEAR_EXPAND
			//////////////////////////////////////////////////////////////////////////
			//}}

			//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		case SQT_PVP_NPC_HUNT:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePVPType",	kSubQuestTemplet.m_ClearCondition.m_ePVPType,		CXSLRoom::PVP_GAME_TYPE,		CXSLRoom::PVP_GAME_TYPE::PGT_TEAM, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPWin",	kSubQuestTemplet.m_ClearCondition.m_iPVPWin,		-1, goto error_proc; );
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ePvpChannelClass",	kSubQuestTemplet.m_ClearCondition.m_ePvpChannelClass, KPVPChannelInfo::PVP_CHANNEL_CLASS, KPVPChannelInfo::PVP_CHANNEL_CLASS::PCC_NONE, goto error_proc; );
			}
			break;
#endif SERV_NEW_PVP_QUEST
			//}} 

			//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		case SQT_VISIT_VILLAGE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_eVillageID",		kSubQuestTemplet.m_ClearCondition.m_iVillageMapID,			0, goto error_proc; );
			}
			break;

		case SQT_VISIT_FIELD:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_eFieldID",			kSubQuestTemplet.m_ClearCondition.m_iBattleFieldID,			0, goto error_proc; );
#ifdef SERV_EVENT_TITLE_SUBQUEST
				LUA_GET_VALUE(				luaManager, L"m_iUseTitleID",		kSubQuestTemplet.m_ClearCondition.m_iUseTitleID,			-1);
#endif SERV_EVENT_TITLE_SUBQUEST
			}
			break;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
			//}}
#ifdef SERV_SUB_QUEST_USE_ITEM
		case SQT_ITEM_USE:
			{	
#ifdef SERV_SUB_QUEST_USE_ITEM_BUG_FIX
				if( luaManager.BeginTable( L"m_eDungeonID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= 0;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setDungeonID.insert( static_cast<SEnum::DUNGEON_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
#else //SERV_SUB_QUEST_USE_ITEM_BUG_FIX
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_eDungeonID",		kSubQuestTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		CXSLDungeon::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, L"m_cDifficulty",		iDifficulty,											-1,		goto error_proc; );
				kSubQuestTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
#endif //SERV_SUB_QUEST_USE_ITEM_BUG_FIX

				//{{ 2009. 3. 31  최육사	특정 난이도 이상
				LUA_GET_VALUE_RETURN(		luaManager, L"m_bUpperDifficulty",	kSubQuestTemplet.m_ClearCondition.m_bUpperDifficulty,	false,	goto error_proc; );
				//}}

				if( luaManager.BeginTable( L"m_listUseItemID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_vecUseItemID.push_back(buf);
						index++;
					}

					luaManager.EndTable();
				}

				LUA_GET_VALUE_RETURN( luaManager, L"m_iUseItemCnt",kSubQuestTemplet.m_ClearCondition.m_iUseItemNum,	0,	goto error_proc; );
			}
			break;
#endif SERV_SUB_QUEST_USE_ITEM
#ifdef SERV_POINT_COUNT_SYSTEM
		case SQT_POINT_COUNT:
			{
				///여기에 서브 퀘스트 조건 받아오면 된다.
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPointCount", kSubQuestTemplet.m_ClearCondition.m_iPointCount, 0, goto error_proc; );
			}
			break;
#endif SERV_POINT_COUNT_SYSTEM
			//{{ 2012. 09. 18	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		case SQT_PVP_PLAY_ARRANGE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iPVPPlay",	kSubQuestTemplet.m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
			}
			break;
#endif SERV_2012_PVP_SEASON2
			//}}
			//{{ 2012. 01. 20    박진웅		스킬 사용 서브퀘스트
#ifdef SERV_SKILL_USE_SUBQUEST
		case SQT_SKILL_USE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, L"m_iUseSkillCount",	kSubQuestTemplet.m_ClearCondition.m_iUseSkillCount,		-1, goto error_proc; );

				if( luaManager.BeginTable( L"m_vecSkillID" ) == S_OK )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == S_OK )
					{
						if( buf > 0 )
							kSubQuestTemplet.m_ClearCondition.m_setSkillID.insert( buf );
						index++;
					}

					luaManager.EndTable();
				}
			}
			break;
#endif SERV_SKILL_USE_SUBQUEST
			//}}

		default:
			{
				START_LOG( cerr, L"Sub Quest ClearCondition Type 별 읽어 오기 실패.!" )
					<< BUILD_LOG( kSubQuestTemplet.m_iID )
					<< BUILD_LOG( kSubQuestTemplet.m_wstrDescription )
					<< BUILD_LOG( kSubQuestTemplet.m_eClearType )
					<< END_LOG;

				luaManager.EndTable();
				return false;
			}
		}


		luaManager.EndTable();
		return true;
	}

error_proc:

	START_LOG( cerr, L"Sub Quest ClearCondition 읽어 오기 실패.!" )
		<< BUILD_LOG( kSubQuestTemplet.m_iID )
		<< BUILD_LOG( kSubQuestTemplet.m_wstrDescription )
		<< BUILD_LOG( kSubQuestTemplet.m_eClearType )
		<< END_LOG;

	luaManager.EndTable();
	return false;
}
#endif SERV_REFORM_QUEST
//}}


//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
bool CXSLQuestManager::SelectTodayRandomQuest( IN bool bIsFirst )
{
	// 특정시간 6:00 에만 한번 초기화 되로록 한다.
	//////////////////////////////////////////////////////////////////////////
	if( bIsFirst == false )
	{
		CTime tCurr = CTime::GetCurrentTime();
#ifdef SERV_INT_ONLY
		CTime tToday = CTime( tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), QE_DAILY_REPEAT_HOUR, 0, 0 );
#else //SERV_INT_ONLY
		CTime tToday = CTime( tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), 6, 0, 0 );
#endif //SERV_INT_ONLY

		if( tToday <= tCurr && m_tLastSelectTime < tToday )
		{
#ifdef SERV_INT_ONLY
			m_tLastSelectTime = CTime( tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), QE_DAILY_REPEAT_HOUR, 0, 1 );
#else //SERV_INT_ONLY
			m_tLastSelectTime = CTime( tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), 6, 0, 1 );
#endif //SERV_INT_ONLY		
		}
		else
		{
			return false;
		}
	}
	else
	{
		m_tLastSelectTime = CTime::GetCurrentTime();
	}
	//////////////////////////////////////////////////////////////////////////

	m_mapTodayRandomQuest.clear();

	// 오늘의 랜덤 퀘스트 선정하기
	// 랜덤그룹 별로 퀘스트 선정
	std::map< int, std::vector< int > >::iterator mitRandomQuest = m_mapRandomQuest.begin();
	for( ; mitRandomQuest != m_mapRandomQuest.end() ; ++mitRandomQuest )
	{
		KLottery kLot;
		std::vector< int >::iterator vit = mitRandomQuest->second.begin();
		for(  ; vit != mitRandomQuest->second.end() ; ++vit )
		{
			if( (*vit) > 0 )
			{
				const QuestTemplet* pQuestTemplet = GetQuestTemplet( *vit );
				if( pQuestTemplet != NULL )
				{
					kLot.AddCase( pQuestTemplet->m_iID, pQuestTemplet->m_fRandomRate );
				}
			}
		}

		int iSelectQuestID = kLot.Decision();
		if( iSelectQuestID > 0)
		{
			// 선택된 랜덤 그룹별 퀘스트 저장
			m_mapTodayRandomQuest.insert( std::make_pair( mitRandomQuest->first, iSelectQuestID ) );
		}
	}

	return true;
}

int CXSLQuestManager::GetTodayRandomQuest( IN int iRandomGroupID )
{
	std::map< int, int >::iterator mit = m_mapTodayRandomQuest.find( iRandomGroupID );
	if( mit != m_mapTodayRandomQuest.end() )
	{
		return mit->second;
	}

	return 0;
}
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
void CXSLQuestManager::GetQuestListByType( IN QUEST_TYPE eType, OUT std::map< int, QuestTemplet >& mapQuestTemplet )
{
	mapQuestTemplet.clear();

	if( QT_NORMAL > eType || eType >= QT_END )
	{
		START_LOG( cerr, L"정상적인 퀘스트 타입이 아닙니다." )
			<< BUILD_LOG( static_cast<int>(eType) )
			<< END_LOG;
		return;
	}

	mapQuestTemplet = m_mapQuestTempletByType[eType];
}

bool CXSLQuestManager::CheckValidDungeonID( IN int iSubQuestID )
{
	const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( iSubQuestID );
	if( pSubQuestTemplet == NULL )
	{
		return false;
	}

	bool bResult = false;

	std::set<SEnum::DUNGEON_ID>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setDungeonID.begin();
	for( ; sit != pSubQuestTemplet->m_ClearCondition.m_setDungeonID.end() ; ++sit )
	{
		if( (*sit) > 0 )
		{
			bResult = true;
			continue;
		}
	}

	return bResult;
}

bool CXSLQuestManager::CheckValidVillageMapID( IN int iSubQuestID )
{
	const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( iSubQuestID );
	if( pSubQuestTemplet == NULL )
	{
		return false;
	}

	bool bResult = false;

	std::set<int>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.begin();
	for( ; sit != pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.end() ; ++sit )
	{
		if( (*sit) > 0 )
		{
			bResult = true;
			continue;
		}
	}

	return bResult;
}

bool CXSLQuestManager::CheckValidBattleFiledID( IN int iSubQuestID )
{
	const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( iSubQuestID );
	if( pSubQuestTemplet == NULL )
	{
		return false;
	}

	bool bResult = false;

	std::set<int>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.begin();
	for( ; sit != pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.end() ; ++sit )
	{
		if( (*sit) > 0 )
		{
			bResult = true;
			continue;
		}
	}

	return bResult;
}

bool CXSLQuestManager::IsExistDungeonInSubQuest( IN int iSubQuestID, IN const int& iDungeonID, IN bool bUpperDifficulty )
{
	const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( iSubQuestID );
	if( pSubQuestTemplet == NULL )
	{
		return false;
	}

	if( bUpperDifficulty == true )
	{
		int iUserDungeon = iDungeonID / 10; // 플레이 중인 던전 얻기
		int iUserDiff = iDungeonID % 10;	// 플레이 중인 던전 얻기

		std::set<SEnum::DUNGEON_ID>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setDungeonID.begin();
		for( ; sit != pSubQuestTemplet->m_ClearCondition.m_setDungeonID.end() ; ++sit )
		{
			int iScriptDungeon = (*sit) / 10;	// 스크립트 던전 얻기
			int iScriptDiff = (*sit) % 10;		// 스크립트 난이도 얻기

			if( iUserDungeon == iScriptDungeon && iScriptDiff <= iUserDiff )
			{
				return true;
			}
		}
	}
	else
	{
		std::set<SEnum::DUNGEON_ID>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setDungeonID.find( static_cast<SEnum::DUNGEON_ID>(iDungeonID) );
		if( sit != pSubQuestTemplet->m_ClearCondition.m_setDungeonID.end() )
		{
			return true;
		}
	}

	return false;
}

bool CXSLQuestManager::IsExistVillageInSubQuest( IN int iSubQuestID, IN const int& iMapID )
{
	const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( iSubQuestID );
	if( pSubQuestTemplet == NULL )
	{
		return false;
	}

	std::set<int>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.find( iMapID );
	if( sit != pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.end() )
	{
		return true;
	}

	return false;
}

bool CXSLQuestManager::IsExistBattleFiledIInSubQuest( IN int iSubQuestID, IN const int& iMapID )
{
	const SubQuestTemplet* pSubQuestTemplet = GetSubQuestTemplet( iSubQuestID );
	if( pSubQuestTemplet == NULL )
	{
		return false;
	}

	std::set<int>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.find( iMapID );
	if( sit != pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.end() )
	{
		return true;
	}

	return false;
}

bool CXSLQuestManager::IsEpicAndNormalQuest( IN int iQuestID )
{
	const QuestTemplet* pQuestTemplet = GetQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"QuestTemplet 포인터 이상.!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;

		return false;
	}

	if( pQuestTemplet->m_eQuestType == QT_EPIC || pQuestTemplet->m_eQuestType == QT_NORMAL )
		return true;

	return false;
}
#endif SERV_REFORM_QUEST
//}} 

//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
const CXSLQuestManager::QuestTemplet*	CXSLQuestManager::GetAccountQuestTemplet( int iQuestID )
{
	std::map< int, QuestTemplet >::const_iterator mit = m_mapAccountQuestTemplet.find( iQuestID );
	if( mit != m_mapAccountQuestTemplet.end() )
		return &mit->second;

	return NULL;
}

void CXSLQuestManager::GetAccountQuestTempletList( OUT std::vector<QuestTemplet*> &vecAccountQuestTemplet )
{
	vecAccountQuestTemplet = m_vecAccountQuestTemplet;
}

bool CXSLQuestManager::AddAccountQuestTemplet_LUA()
{
	QuestTemplet kQuestTemplet;
	KLuaManager luaManager( GetLuaState() );

	std::wstring wstrErrLua;

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_iID",				kQuestTemplet.m_iID,			0, wstrErrLua, goto LoadFail; );

#ifdef SERV_NAVER_CHANNELING
    LUA_GET_VALUE_ENUM(	luaManager, L"m_ePublisher", kQuestTemplet.m_ePublisher, KNexonAccountInfo::CHANNELING_ENUM, KNexonAccountInfo::CE_NEXON_ACCOUNT );
#endif SERV_NAVER_CHANNELING
	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 12. 18  최육사	서버군 확장
	int iServerGroupID = 0;
	LUA_GET_VALUE(					luaManager, L"m_iServerGroupID",	iServerGroupID,					-1 );
	
#ifdef EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID != -1 )
#else EXTEND_SERVER_GROUP_MASK
	if( iServerGroupID != SEnum::SGI_INVALID )
#endif EXTEND_SERVER_GROUP_MASK
	{
		if( SEnum::IsValidServerGroupID( iServerGroupID ) == false )
		{
			START_LOG( cerr, L"정상적인 서버군ID값이 아닙니다." )
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( kQuestTemplet.m_iID );
			return false;
		}

		if( KBaseServer::GetKObj()->GetServerGroupID() != iServerGroupID )
		{
			START_LOG( clog2, L"서버군이 다르므로 해당 퀘스트는 파싱하지 않습니다." )
				<< BUILD_LOG( iServerGroupID )
				<< BUILD_LOG( kQuestTemplet.m_iID );
			return true;
		}
	}
	//}}
	//////////////////////////////////////////////////////////////////////////	

	LUA_GET_VALUE_RETURN_ENUM_ERR(	luaManager, L"m_eQuestType",		kQuestTemplet.m_eQuestType,		QUEST_TYPE,	QT_NORMAL, wstrErrLua, goto LoadFail; );

	LUA_GET_VALUE_RETURN_ENUM_ERR(	luaManager, L"m_eStartNPCID",		kQuestTemplet.m_eStartNPCID,	CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE,	wstrErrLua, goto LoadFail; );
	LUA_GET_VALUE_RETURN_ENUM_ERR(	luaManager, L"m_eEndNPCID",			kQuestTemplet.m_eEndNPCID,		CXSLUnitManager::NPC_UNIT_ID,	CXSLUnitManager::NUI_NONE,	wstrErrLua, goto LoadFail; );

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_wstrTitle",			kQuestTemplet.m_wstrTitle,		L"",	wstrErrLua, goto LoadFail; );
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_iFairLevel",		kQuestTemplet.m_iFairLevel,		0,		wstrErrLua, goto LoadFail; );

	LUA_GET_VALUE_RETURN_ENUM_ERR(	luaManager, L"m_eRepeatType",		kQuestTemplet.m_eRepeatType,	QUEST_REPEAT_TYPE,	QRT_NORMAL,	wstrErrLua, goto LoadFail; );
	
	//{{ 2010. 01. 29  최육사	일일퀘스트
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_bIsPcBang",			kQuestTemplet.m_bIsPcBang,		false,	wstrErrLua, goto LoadFail; );
	//}}

	//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_bIsTimeEvent",		kQuestTemplet.m_bIsTimeEvent,	false,	wstrErrLua, goto LoadFail; );
	//}}

	if( luaManager.BeginTable( L"m_vecAcceptRewardItem" ) == S_OK )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == S_OK )
		{
			ITEM_DATA itemData;
			LUA_GET_VALUE(	luaManager, L"m_iItemID",	itemData.m_iItemID,		0 );
			LUA_GET_VALUE(	luaManager, L"m_iQuantity",	itemData.m_iQuantity,	0 );

			if( 0 != itemData.m_iItemID  &&  0 != itemData.m_iQuantity )
			{
				kQuestTemplet.m_mapAcceptRewardItem.insert( std::make_pair( itemData.m_iItemID, itemData.m_iQuantity ) );
			}

			index++;
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_wstrMainText",			kQuestTemplet.m_wstrMainText,			L"",	wstrErrLua, goto LoadFail; );
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_wstrThanksText",		kQuestTemplet.m_wstrThanksText,			L"",	wstrErrLua, goto LoadFail; );
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"m_wstrDissClearText",		kQuestTemplet.m_wstrDissClearText,		L"",	wstrErrLua, goto LoadFail; );

	LUA_GET_VALUE(		luaManager, L"m_iEpisodeGroupID",	kQuestTemplet.m_iEpisodeGroupID,	0 );
	LUA_GET_VALUE(		luaManager, L"m_iPlayLevel",		kQuestTemplet.m_iPlayLevel,			0 );

	if( luaManager.BeginTable( L"m_iAfterQuestID" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
				kQuestTemplet.m_vecAfterQuestID.push_back( buf );
			index++;
		}

		luaManager.EndTable();
	}

	LUA_GET_VALUE(		luaManager, L"m_wstrEpisodeGroupTitle",		kQuestTemplet.m_wstrEpisodeGroupTitle,	L"" );
	LUA_GET_VALUE(		luaManager, L"m_wstrStartScene",			kQuestTemplet.m_wstrStartScene,			L"" );
	LUA_GET_VALUE(		luaManager, L"m_wstrEndScene",				kQuestTemplet.m_wstrEndScene,			L"" );

	LUA_GET_VALUE(		luaManager, L"m_iRandomGroupID",	kQuestTemplet.m_iRandomGroupID,		0 );
	LUA_GET_VALUE(		luaManager, L"m_fRandomRate",		kQuestTemplet.m_fRandomRate,		0.f );

	// 여기는 계정 퀘스트 이므로 무조건 true로 함
	LUA_GET_VALUE(		luaManager, L"m_bIsAccount",	kQuestTemplet.m_bIsAccountQuest,		true );
	
	if( LoadQuestCondition( luaManager, kQuestTemplet.m_Condition ) == false )
		goto LoadFail;

	if( luaManager.BeginTable( L"m_vecSubQuest" ) == S_OK )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf > 0 )
				kQuestTemplet.m_vecSubQuest.push_back( buf );
			index++;
		}

		luaManager.EndTable();
	}

	//{{ dmlee 2008.11.28 서브퀘스트가 없는 경우 예외 처리
	if( true == kQuestTemplet.m_vecSubQuest.empty() )
		goto LoadFail;
	//}} dmlee 2008.11.28 서브퀘스트가 없는 경우 예외 처리

	if( luaManager.BeginTable( L"m_vecSubQuest_Group" ) == S_OK )
	{
		int index = 1; 
		while( luaManager.BeginTable( index ) == S_OK )
		{
			int i		= 1; 
			int buf		= -1;
			while( luaManager.GetValue( i, buf ) == S_OK )
			{
				if( buf > 0 )
				{
					std::map< int, std::vector<int> >::iterator mit = kQuestTemplet.m_mapSubQuestGroup.find( index );
					if( mit == kQuestTemplet.m_mapSubQuestGroup.end() ) // 기존 그룹이 없다면
					{
						std::vector<int> vecTemp;
						vecTemp.push_back( buf );
						kQuestTemplet.m_mapSubQuestGroup.insert( std::make_pair( index, vecTemp ) );
					}
					else
					{
						BOOST_TEST_FOREACH( int, iSubQuestID, mit->second )
						{
							if( buf == iSubQuestID )
							{
								START_LOG( cerr, L"동일한 서브 퀘스트가 그룹핑 되어 있다." )
									<< BUILD_LOG( kQuestTemplet.m_iID )
									<< BUILD_LOG( iSubQuestID )
									<< BUILD_LOG( buf )
									<< END_LOG;
								goto LoadFail;
							}
						}
						mit->second.push_back( buf );
					}
				}					
				i++;
			}
			luaManager.EndTable();

			++index;
		}
		luaManager.EndTable();
	}

	if( LoadReward( luaManager, L"Reward", kQuestTemplet.m_Reward ) == false )
		goto LoadFail;

	if( luaManager.BeginTable( "SelectReward" ) == S_OK )
	{
		LUA_GET_VALUE(	luaManager, L"m_iSelectionCount",	kQuestTemplet.m_SelectReward.m_iSelectionCount,		1 );

		if( luaManager.BeginTable( L"m_vecSelectItem" ) == S_OK )
		{
			int index = 1; 
			while( luaManager.BeginTable( index ) == S_OK )
			{
				ITEM_DATA itemData;
				LUA_GET_VALUE(	luaManager, L"m_iItemID",	itemData.m_iItemID,		0 );
				LUA_GET_VALUE(	luaManager, L"m_iQuantity",	itemData.m_iQuantity,	0 );
				LUA_GET_VALUE(	luaManager, L"m_iPeriod",	itemData.m_iPeriod,		0 );
				//{{ 2008. 8. 25  최육사	퀘스트보상 소켓옵션
				LUA_GET_VALUE(  luaManager, L"m_sSocketOption", itemData.m_iSocketOption1, 0 );
				//}}

				if( 0 != itemData.m_iItemID && 0 != itemData.m_iQuantity )
				{
					kQuestTemplet.m_SelectReward.m_vecSelectItem.push_back( itemData );
				}

				index++;
				luaManager.EndTable();
			}
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}

	// 퀘스트가 숨겨지는 아이템 리스트 얻기
	if( luaManager.BeginTable( "HIDE_QUEST_ITEM" ) == S_OK )
	{
		int index	= 1; 
		int iValue	= -1;
		while( luaManager.GetValue( index, iValue ) == S_OK )
		{
			if( iValue > 0 )
			{
				kQuestTemplet.m_setHideItemID.insert( iValue );
			}

			index++;
		}

		luaManager.EndTable();
	}

	if( m_mapAccountQuestTemplet.find( kQuestTemplet.m_iID ) != m_mapAccountQuestTemplet.end() )
		goto LoadFail;

	m_mapAccountQuestTemplet.insert( std::make_pair( kQuestTemplet.m_iID, kQuestTemplet ) );

	{
		//# 계정퀘스트 리스트 제공을 위해 담아 놓는다.
		std::map< int, QuestTemplet >::iterator mit;
		mit = m_mapAccountQuestTemplet.find(kQuestTemplet.m_iID);

		if( mit != m_mapAccountQuestTemplet.end() )
		{
			std::vector<QuestTemplet*>::iterator vit = m_vecAccountQuestTemplet.begin();

			//# 실시간 로딩시에 지장이 없기 위해서 있는 것은 지우고 새로 넣어줌
			while( vit != m_vecAccountQuestTemplet.end() )
			{
				if( kQuestTemplet.m_iID == (*vit)->m_iID )
				{
					m_vecAccountQuestTemplet.erase(vit);
					break;
				}
				++vit;
			}

			m_vecAccountQuestTemplet.push_back(&(mit->second));
		}
		else
		{
			START_LOG( cerr, L"[계정관련] 방금넣은 계정 퀘스트 데이터가 없음." )
				<< BUILD_LOG( kQuestTemplet.m_iID )
				<< END_LOG;
		}
	}

	return true;

LoadFail:
	START_LOG( cerr, L"::: Account Quest Templet Load Fail :::" )		
		<< BUILD_LOG( kQuestTemplet.m_iID )
		<< BUILD_LOG( kQuestTemplet.m_wstrTitle )
		<< BUILD_LOG( wstrErrLua )
		<< END_LOG;

	return false;
}

//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
bool CXSLQuestManager::AddAccountQuestDate_LUA()
{
	AccountQuestDateInfo kAccountQuestDateInfo;

	KLuaManager luaManager( GetLuaState() );

	std::wstring wstrErrLua;

	int iAccountQuestID = 0;
	std::wstring wstrBegin;
	std::wstring wstrEnd;

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"AccountQuestID",				iAccountQuestID,			0, wstrErrLua, goto LoadFail; );

	// 존재하는 계정 퀘스트인지 확인한다.
	const CXSLQuestManager::QuestTemplet*	pQuestTemplet = GetAccountQuestTemplet( iAccountQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 계정 QUEST 입니다." )
			<< BUILD_LOG( iAccountQuestID )
			<< BUILD_LOG( wstrErrLua )
			<< END_LOG;
		return false;
	}

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"BeginDate",			wstrBegin,		L"",	wstrErrLua, goto LoadFail; );
	if( KncUtil::ConvertStringToCTime( wstrBegin, kAccountQuestDateInfo.m_tBegin ) == false )
	{
		START_LOG( cerr, L"계정 QUEST 유효 기간 시작 시간이 잘못되었습니다." )
			<< BUILD_LOG( iAccountQuestID )
			<< BUILD_LOG( wstrBegin )
			<< BUILD_LOG( wstrErrLua )
			<< END_LOG;
		return false;
	}

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"EndDate",				wstrEnd,		L"",	wstrErrLua, goto LoadFail; );
	if( KncUtil::ConvertStringToCTime( wstrEnd, kAccountQuestDateInfo.m_tEnd ) == false )
	{
		START_LOG( cerr, L"계정 QUEST 유효 기간 종료 시간이 잘못되었습니다." )
			<< BUILD_LOG( iAccountQuestID )
			<< BUILD_LOG( wstrEnd )
			<< BUILD_LOG( wstrErrLua )
			<< END_LOG;
		return false;
	}

	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"DayOfWeek",			kAccountQuestDateInfo.m_iDayOfWeek,		0,	wstrErrLua, goto LoadFail; );
	if( kAccountQuestDateInfo.m_iDayOfWeek < 1 || kAccountQuestDateInfo.m_iDayOfWeek > 7 )
	{
		START_LOG( cerr, L"계정 QUEST 요일 정보가 잘못되었습니다." )
			<< BUILD_LOG( iAccountQuestID )
			<< BUILD_LOG( kAccountQuestDateInfo.m_iDayOfWeek )
			<< BUILD_LOG( wstrErrLua )
			<< END_LOG;
		return false;
	}

	// 정보를 정상적으로 읽어 왔으면 컨테이너에 넣자
	// 중보 정보가 있는지 확인
	{
		std::map< int, AccountQuestDateInfo >::iterator mit = m_mapAccountQuestDayOfWeekInfo.find( iAccountQuestID );
		if( mit != m_mapAccountQuestDayOfWeekInfo.end() )
		{
			START_LOG( cerr, L"계정 QUEST 날짜 정보가 중복되었습니다." )
				<< BUILD_LOG( iAccountQuestID )
				<< BUILD_LOG( wstrBegin )
				<< BUILD_LOG( wstrEnd )
				<< BUILD_LOG( kAccountQuestDateInfo.m_iDayOfWeek )
				<< END_LOG;
			return false;
		}
	}

	m_mapAccountQuestDayOfWeekInfo.insert( std::make_pair( iAccountQuestID, kAccountQuestDateInfo ) );

	START_LOG( cout, L"계정 QUEST 날짜 정보가 정상적으로 입력 되었습니다.." )
		<< BUILD_LOG( iAccountQuestID )
		<< END_LOG;

	return true;

LoadFail:
	START_LOG( cerr, L"::: Account Quest Date Load Fail :::" )		
		<< BUILD_LOG( iAccountQuestID )
		<< BUILD_LOG( wstrErrLua )
		<< END_LOG;
	return false;
}

bool CXSLQuestManager::IsValidAccountQuestDate( IN int iAccountQuestID, IN CTime& tNowTime )
{
	std::map< int, AccountQuestDateInfo >::iterator mit = m_mapAccountQuestDayOfWeekInfo.find( iAccountQuestID );
	if( mit == m_mapAccountQuestDayOfWeekInfo.end() )
	{
		START_LOG( clog, L"오류 아님. 없을 수 있음. 계정 QUEST 날짜 정보가 없습니다." )
			<< BUILD_LOG( iAccountQuestID )
			<< END_LOG;
		return true;
	}

	if( mit->second.m_tBegin > tNowTime )
	{
		START_LOG( clog, L"계정 QUEST 의 유효 날짜가 되지 않았습니다." )
			<< BUILD_LOG( iAccountQuestID )
			<< BUILD_LOG( mit->second.m_tBegin.GetYear() )
			<< BUILD_LOG( mit->second.m_tBegin.GetMonth() )
			<< BUILD_LOG( mit->second.m_tBegin.GetDay() )
			<< BUILD_LOG( mit->second.m_tBegin.GetHour() )
			<< BUILD_LOG( mit->second.m_tBegin.GetMinute() )
			<< BUILD_LOG( mit->second.m_tBegin.GetSecond() )
			<< END_LOG;

		return false;
	}
	if( mit->second.m_tEnd < tNowTime )
	{
		START_LOG( clog, L"계정 QUEST 의 유효 날짜가 지났습니다." )
			<< BUILD_LOG( iAccountQuestID )
			<< BUILD_LOG( mit->second.m_tEnd.GetYear() )
			<< BUILD_LOG( mit->second.m_tEnd.GetMonth() )
			<< BUILD_LOG( mit->second.m_tEnd.GetDay() )
			<< BUILD_LOG( mit->second.m_tEnd.GetHour() )
			<< BUILD_LOG( mit->second.m_tEnd.GetMinute() )
			<< BUILD_LOG( mit->second.m_tEnd.GetSecond() )
			<< END_LOG;

		return false;
	}

	if( mit->second.m_iDayOfWeek != tNowTime.GetDayOfWeek() )
	{
		START_LOG( clog, L"요일이 맞지 않습니다." )
			<< BUILD_LOG( iAccountQuestID )
			<< BUILD_LOG( mit->second.m_tEnd.GetYear() )
			<< BUILD_LOG( mit->second.m_tEnd.GetMonth() )
			<< BUILD_LOG( mit->second.m_tEnd.GetDay() )
			<< BUILD_LOG( mit->second.m_tEnd.GetHour() )
			<< BUILD_LOG( mit->second.m_tEnd.GetMinute() )
			<< BUILD_LOG( mit->second.m_tEnd.GetSecond() )
			<< END_LOG;

		return false;
	}

	return true;
}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
//}
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}
